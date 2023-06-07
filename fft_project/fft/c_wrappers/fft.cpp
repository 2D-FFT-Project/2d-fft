#include "fft.h"

#include <iostream>

#if defined(USE_CILK)
#include <cilk/cilk.h>
#define SPAWN cilk_spawn
#define SYNC cilk_sync
#else
#if defined(USE_THREAD)
#include <thread>
#ifndef P_THRESHOLD
#define P_THRESHOLD 128
#endif
#endif
#define SPAWN
#define SYNC
#endif

namespace base {
using std::string, std::cout;
std::string getenv(string key, string null_value = "") {
  char *value = std::getenv(key.c_str());
  if (value == nullptr) return null_value;
  return string(value);
};

template <class T>
inline void print_matrix(T* V, int N, int M) {
  for (int i = 0; i < N; i++) {
    for (int j = 0; j < M; j++) {
      cout << V[i * M + j] << " ";
    }
    cout << '\n';
  }
}
} // namespace base


namespace fft {
#define forn(i, n) for (int i = 0; i < (int)(n); i++)

void _fft2d(
    fft_type *__restrict__ V,
    size_t N,
    size_t rowsize,
    fft_type *__restrict__ W,
    int step,
    int use_threads
  ) {
  // base case {
  if (N == 1) return;
  if (N == 2) {
#define Y(y, x) (V[(y)*rowsize + (x)])
    auto x00 = Y(0, 0);
    auto x10 = Y(1, 0);
    auto x01 = Y(0, 1);
    auto x11 = Y(1, 1);
    Y(0, 0) = x00 + x10 + x01 + x11;
    Y(0, 1) = x00 + x10 - x01 - x11;
    Y(1, 0) = x00 - x10 + x01 - x11;
    Y(1, 1) = x00 - x10 - x01 + x11;
    return;
  }
  // } base case 

  int n = N >> 1;

#define X(y, x, i, j) (V[((y)*n + (i)) * rowsize + ((x)*n) + j])
#define params n, rowsize, W, (step << 1), use_threads

#if defined(USE_THREAD)
  // TODO(gyzavyalov): work-stealing
  if (n <= P_THRESHOLD || !use_threads) {
    _fft2d(&X(0, 0, 0, 0), params);
    _fft2d(&X(0, 1, 0, 0), params);
    _fft2d(&X(1, 0, 0, 0), params);
    _fft2d(&X(1, 1, 0, 0), params);  
  } else {
    std::thread t1([V, W, step, n, rowsize, use_threads](){ _fft2d(&X(0, 0, 0, 0), params); });
    std::thread t2([V, W, step, n, rowsize, use_threads](){ _fft2d(&X(0, 1, 0, 0), params); });
    std::thread t3([V, W, step, n, rowsize, use_threads](){ _fft2d(&X(1, 0, 0, 0), params); });
                                                            _fft2d(&X(1, 1, 0, 0), params);
    t1.join(), t2.join(), t3.join();
  }
#else
  SPAWN _fft2d(&X(0, 0, 0, 0), params);
  SPAWN _fft2d(&X(0, 1, 0, 0), params);
  SPAWN _fft2d(&X(1, 0, 0, 0), params);
        _fft2d(&X(1, 1, 0, 0), params);
  SYNC;
#endif

  for (int i = 0; i < n; i++) {
    for (int j = 0; j < n; j++) {
      auto x00 = X(0, 0, i, j);
      auto x10 = X(1, 0, i, j) * W[step * i];
      auto x01 = X(0, 1, i, j) * W[step * j];
      auto x11 = X(1, 1, i, j) * W[step * (i + j)];
      X(0, 0, i, j) = x00 + x10 + x01 + x11;
      X(0, 1, i, j) = x00 + x10 - x01 - x11;
      X(1, 0, i, j) = x00 - x10 + x01 - x11;
      X(1, 1, i, j) = x00 - x10 - x01 + x11;
    }
  }
}


void _plan(
    fft_type *__restrict__ V,
    size_t N,
    size_t M,
    size_t rowsize,
    fft_type *__restrict__ W,
    int step_i,
    int step_j,
    int use_threads
  ) {
  if (N == M) _fft2d(V, N, rowsize, W, step_i, use_threads);
  else if (N > M) {
    int n = N >> 1;
#define Y(y, i, j) (V[((y)*n + (i)) * rowsize + j])
#define params n, M, rowsize, W, (step_i << 1), step_j, use_threads
    _plan(&Y(0, 0, 0), params);
    _plan(&Y(1, 0, 0), params);
    
    forn (i, n) {
      forn (j, M) {
        auto y00 = Y(0, i, j);
        auto y10 = Y(1, i, j) * W[i * step_i];
        Y(0, i, j) = y00 + y10;
        Y(1, i, j) = y00 - y10;
      }
    }
  } else {
    int m = M >> 1;
#define X(x, i, j) (V[(i) * rowsize + ((x)*m) + j])
#define params N, m, rowsize, W, step_i, (step_j << 1), use_threads
    _plan(&X(0, 0, 0), params);
    _plan(&X(1, 0, 0), params);
    
    forn (i, N) {
      forn (j, m) {
        auto x00 = X(0, i, j);
        auto x10 = X(1, i, j) * W[j * step_j];
        X(0, i, j) = x00 + x10;
        X(1, i, j) = x00 - x10;
      }
    }
  }
}

void fft2d(fft_type *V, int N, int M, int use_threads, int inverse) {
#if defined(CXX_MEASURE_TIME)
  struct timespec start, end;
  clock_gettime(CLOCK_MONOTONIC, &start);
#endif
  assert(__builtin_popcount(N) == 1 && "dim[0] has to be a power of 2");
  assert(__builtin_popcount(M) == 1 && "dim[1] has to be a power of 2");

  // Welcome to shitty code : )
  auto revbits = [](size_t *v, size_t n) {
    int lg_n = log2(n);
    forn(i, n) {
      int revi = 0;
      forn(l, lg_n) revi |= ((i >> l) & 1) << (lg_n - l - 1);
      v[i] = revi;
    }
  };

  size_t *rev_n = new size_t[N], *rev_m = new size_t[M];
  revbits(rev_n, N), revbits(rev_m, M);  

  forn(i, N) {
    int rev_i = rev_n[i];
    forn(j, M) {
      if ((i < rev_i) || ((i == rev_i) && (j < rev_m[j])))
        std::swap(V[i * M + j], V[rev_i * M + rev_m[j]]);
    }
  }

  int mxdim = std::max(N, M);
  const int lg_dim = log2(mxdim);
  auto W = new fft_type[mxdim];
  auto rooti = std::polar(1., (inverse ? 2 : -2) * fft::pi / mxdim);

  auto cur_root = rooti;
  W[0] = 1;
  forn (i, mxdim - 1)
    W[i + 1] = W[i] * cur_root;
  

  _plan(
    V, N, M, M,
    W,
    (1ll << int(lg_dim - log2(N))), (1ll << int(lg_dim - log2(M))),
    use_threads
  );
  if (inverse) forn (i, N) forn (j, M) V[i * M + j] /= N * M;
  delete [] rev_n;
  delete [] rev_m;
  delete [] W;
#if defined(CXX_MEASURE_TIME)
  clock_gettime(CLOCK_MONOTONIC, &end);
  printf("2d fft in cxx: %0.9fs\n",
         (end.tv_sec - start.tv_sec) + 1e-9 * (end.tv_nsec - start.tv_nsec));
#endif
}
} // namespace fft

int main(int argc, char **argv) {
  using namespace std;
  int use_threads = 0;
  const uint32_t N = stoi(string(argv[1]));
  assert(__builtin_popcount(N) == 1);
  auto matrix = new fft::fft_type[N * N];
  fft::fft2d(matrix, N, N, use_threads, 0);
}
