#include "fft.h"

#include <iostream>

#if defined(USE_CILK)
#include <cilk/cilk.h>
#define SPAWN cilk_spawn
#define SYNC cilk_sync
#elif defined(USE_THREAD)
#include <thread>
#ifndef P_THRESHOLD
#define P_THRESHOLD 128
#endif
#else
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
    fft_type root
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
  if (n <= 64) {
    _fft2d(&X(0, 0, 0, 0), n, rowsize, root * root);
    _fft2d(&X(0, 1, 0, 0), n, rowsize, root * root);
    _fft2d(&X(1, 0, 0, 0), n, rowsize, root * root);
    _fft2d(&X(1, 1, 0, 0), n, rowsize, root * root);  
  } else {
    std::thread t1([V, n, rowsize, root](){ _fft2d(&X(0, 0, 0, 0), n, rowsize, root * root); });
    std::thread t2([V, n, rowsize, root](){ _fft2d(&X(0, 1, 0, 0), n, rowsize, root * root); });
    std::thread t3([V, n, rowsize, root](){ _fft2d(&X(1, 0, 0, 0), n, rowsize, root * root); });
                                            _fft2d(&X(1, 1, 0, 0), n, rowsize, root * root);
    t1.join(), t2.join(), t3.join();
  }
#else
  SPAWN _fft2d(&X(0, 0, 0, 0), n, rowsize, root * root);
  SPAWN _fft2d(&X(0, 1, 0, 0), n, rowsize, root * root);
  SPAWN _fft2d(&X(1, 0, 0, 0), n, rowsize, root * root);
        _fft2d(&X(1, 1, 0, 0), n, rowsize, root * root);
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
    fft_type *__restrict__ V,
    size_t N,
    size_t M,
    size_t rowsize,
    fft_type rooti,
    fft_type rootj
  ) {
  if (N == M) _fft2d(V, N, rowsize, rooti);
  else if (N > M) { // vertical split
    int n = N >> 1;
#define Y(y, i, j) (V[((y)*n + (i)) * rowsize + j])
    _plan(&Y(0, 0, 0), n, M, rowsize, rooti * rooti, rootj);
    _plan(&Y(1, 0, 0), n, M, rowsize, rooti * rooti, rootj);
    
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
  } else { // horizontal split 
    int m = M >> 1;
#define X(x, i, j) (V[(i) * rowsize + ((x)*m) + j])
    _plan(&X(0, 0, 0), N, m, rowsize, rooti, rootj * rootj);
    _plan(&X(1, 0, 0), N, m, rowsize, rooti, rootj * rootj);
    
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


void fft2d(fft_type *V, int N, int M) {
#if defined(CXX_MEASURE_TIME)
  struct timespec start, end;
  clock_gettime(CLOCK_MONOTONIC, &start);
#endif
  assert(__builtin_popcount(N) == 1 && "dim[0] has to be a power of 2");
  assert(__builtin_popcount(M) == 1 && "dim[1] has to be a power of 2");

  int lg_n = log2(N), lg_m = log2(M);
  auto rev_n = new int[N], rev_m = new int[M];

  // Welcome to shitty code : )
  forn(i, N) {
    int revi = 0;
    forn(l, lg_n) revi |= ((i >> l) & 1) << (lg_n - l - 1);
    rev_n[i] = revi;
  }

  forn(i, M) {
    int revi = 0;
    forn(l, lg_m) revi |= ((i >> l) & 1) << (lg_m - l - 1);
    rev_m[i] = revi;
  }

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
    std::polar(1., 2 * fft::pi / M)
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
  const uint32_t N = stoi(string(argv[1]));
  assert(__builtin_popcount(N) == 1);
  auto matrix = new fft::fft_type[N * N];
  {
      // forn(i, N) forn(j, N) matrix[i * N + j] = 2 * i + j;
      // forn(i, N) {
      //   forn(j, N) cout << matrix[i * N + j] << ' ';
      //   cout << '\n';
      // }
  } {
    // cout << string(8, '-') << '\n';
    fft::fft2d(matrix, N, N);
    // cout << "Result:" << '\n';
    // forn(i, N) {
    //   forn(j, N) cout << matrix[i * N + j] << ' ';
    //   cout << '\n';
    // }
  }
}
