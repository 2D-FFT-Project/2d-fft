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
    fft_type root,
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
#if defined(USE_THREAD)
  if (n <= P_THRESHOLD || !use_threads) {
    _fft2d(&X(0, 0, 0, 0), n, rowsize, root * root, use_threads);
    _fft2d(&X(0, 1, 0, 0), n, rowsize, root * root, use_threads);
    _fft2d(&X(1, 0, 0, 0), n, rowsize, root * root, use_threads);
    _fft2d(&X(1, 1, 0, 0), n, rowsize, root * root, use_threads);  
  } else {
    std::thread t1([V, n, rowsize, root, use_threads](){ _fft2d(&X(0, 0, 0, 0), n, rowsize, root * root, use_threads); });
    std::thread t2([V, n, rowsize, root, use_threads](){ _fft2d(&X(0, 1, 0, 0), n, rowsize, root * root, use_threads); });
    std::thread t3([V, n, rowsize, root, use_threads](){ _fft2d(&X(1, 0, 0, 0), n, rowsize, root * root, use_threads); });
                                                         _fft2d(&X(1, 1, 0, 0), n, rowsize, root * root, use_threads);
    t1.join(), t2.join(), t3.join();
  }
#else
  SPAWN _fft2d(&X(0, 0, 0, 0), n, rowsize, root * root, use_threads);
  SPAWN _fft2d(&X(0, 1, 0, 0), n, rowsize, root * root, use_threads);
  SPAWN _fft2d(&X(1, 0, 0, 0), n, rowsize, root * root, use_threads);
        _fft2d(&X(1, 1, 0, 0), n, rowsize, root * root, use_threads);
  SYNC;
#endif

  auto W = new fft_type[n];
  W[0] = 1;
  forn(i, n - 1) W[i + 1] = W[i] * root;

  for (int i = 0; i < n; i++) {
    for (int j = 0; j < n; j++) {
      auto x00 = X(0, 0, i, j);
      auto x10 = X(1, 0, i, j) * W[i];
      auto x01 = X(0, 1, i, j) * W[j];
      auto x11 = X(1, 1, i, j) * W[i] * W[j];
      X(0, 0, i, j) = x00 + x10 + x01 + x11;
      X(0, 1, i, j) = x00 + x10 - x01 - x11;
      X(1, 0, i, j) = x00 - x10 + x01 - x11;
      X(1, 1, i, j) = x00 - x10 - x01 + x11;
    }
  }
}


void _plan(
    fft_type *V,
    size_t N,
    size_t M,
    size_t rowsize,
    fft_type rooti,
    fft_type rootj,
    int use_threads
  ) {
  // Welcome to shitty code : )
  if (N == M) _fft2d(V, N, rowsize, rooti, use_threads);
  else if (N > M) {
    int n = N >> 1;
#define Y(y, i, j) (V[((y)*n + (i)) * rowsize + j])
    _plan(&Y(0, 0, 0), n, M, rowsize, rooti * rooti, rootj, use_threads);
    _plan(&Y(1, 0, 0), n, M, rowsize, rooti * rooti, rootj, use_threads);
    
    auto W = new fft_type[n];
    W[0] = 1;
    forn(i, n - 1) W[i + 1] = W[i] * rooti;

    forn (i, n) {
      forn (j, M) {
        auto y00 = Y(0, i, j);
        auto y10 = Y(1, i, j) * W[i];
        Y(0, i, j) = y00 + y10;
        Y(1, i, j) = y00 - y10;
      }
    }
  } else {
    int m = M >> 1;
#define X(x, i, j) (V[(i) * rowsize + ((x)*m) + j])
    _plan(&X(0, 0, 0), N, m, rowsize, rooti, rootj * rootj, use_threads);
    _plan(&X(1, 0, 0), N, m, rowsize, rooti, rootj * rootj, use_threads);
    
    auto W = new fft_type[m];
    W[0] = 1;
    forn(i, m - 1) W[i + 1] = W[i] * rootj;

    forn (i, N) {
      forn (j, m) {
        auto x00 = X(0, i, j);
        auto x10 = X(1, i, j) * W[j];
        X(0, i, j) = x00 + x10;
        X(1, i, j) = x00 - x10;
      }
    }
  }
}

void fft2d(fft_type *V, int N, int M, int use_threads) {
#if defined(CXX_MEASURE_TIME)
  struct timespec start, end;
  clock_gettime(CLOCK_MONOTONIC, &start);
#endif
  assert(__builtin_popcount(N) == 1 && "dim[0] has to be a power of 2");
  assert(__builtin_popcount(M) == 1 && "dim[1] has to be a power of 2");
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

  _plan(
    V, N, M, M,
    std::polar(1., 2 * fft::pi / N),
    std::polar(1., 2 * fft::pi / M),
    use_threads
  );
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
  fft::fft2d(matrix, N, N, use_threads);
}
