#include "fft.h"
#include <iostream>
namespace fft {
#define forn(i, n) for (int i = 0; i < (int)(n); i++)


fft_type* pad_matrix(fft_type *Matrix, int N, int M) {
  int new_size = std::max(N, M);
  while (__builtin_popcount(new_size) != 1) {
      new_size <<= 1; 
  }
  auto new_matrix = new fft_type[new_size * new_size];
  std::fill(new_matrix, new_matrix + new_size * new_size, 0);
  for(int i = 0; i < N; i++)
    for(int j = 0; j < M; j++)
      new_matrix[i * new_size + j] = Matrix[i * M + j];
  return new_matrix;
}
void _fft2d(fft_type *__restrict__ M, fft_type *__restrict__ W, int N,
            int rowsize, fft_type root) {
  if (N == 1) return;
  if (N == 2) {
#define Y(y, x) (M[(y)*rowsize + (x)])
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


  int n = N >> 1;
#define X(y, x, i, j) (M[((y)*n + (i)) * rowsize + ((x)*n) + j])
  _fft2d(&X(0, 0, 0, 0), W, n, rowsize, root * root);
  _fft2d(&X(0, 1, 0, 0), W, n, rowsize, root * root);
  _fft2d(&X(1, 0, 0, 0), W, n, rowsize, root * root);
  _fft2d(&X(1, 1, 0, 0), W, n, rowsize, root * root);

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
void fft2d(fft_type *M, int N, int M_size) {
#if defined(CXX_MEASURE_TIME)
  struct timespec start, end;
  clock_gettime(CLOCK_MONOTONIC, &start);
#endif
  const int logn = log2(N);

  auto root = std::polar(1., 2 * fft::pi / N);
  auto rev = new int[N];
  auto W = new fft_type[N >> 1];

  forn(i, N) {
    int revi = 0;
    forn(l, logn) revi |= ((i >> l) & 1) << (logn - l - 1);
    rev[i] = revi;
  }

  int nextPowerOfTwo = std::pow(2, std::ceil(std::log2(std::max(N, M_size))));
  if (N != nextPowerOfTwo || M_size != nextPowerOfTwo) {
    fft_type *new_M = pad_matrix(M, N, M_size);
    delete[] M;
    M = new_M;
    N = nextPowerOfTwo;
    M_size = nextPowerOfTwo;
  }

  forn(i, N) {
    int rev_i = rev[i];
    forn(j, N) {
      if ((i < rev_i) || ((i == rev_i) && (j < rev[j]))) {
        std::swap(M[i * N + j], M[rev_i * N + rev[j]]);
      }
    }
  }

  _fft2d(M, W, N, N, root);
#if defined(CXX_MEASURE_TIME)
  clock_gettime(CLOCK_MONOTONIC, &end);
  printf("2d fft in cxx: %0.9fs\n",
         (end.tv_sec - start.tv_sec) + 1e-9 * (end.tv_nsec - start.tv_nsec));
#endif
}

} // namespace fft

int main(int argc, char **argv) {
>>>>>>> Stashed changes
  using namespace std;
  typedef complex<double> ftype;
>>>>>>> e2344ab (changed the power getting method to while popcount)

namespace base {
using std::string, std::cout;
std::string getenv(string key, string null_value = "") {
  char *value = std::getenv(key.c_str());
  if (value == nullptr) return null_value;
  return string(value);
};

template <class T>
inline void print_matrix(const std::vector<std::vector<T>> &v) {
  for (int i = 0; i < v.size(); i++) {
    for (int j = 0; j < v[i].size(); j++)
      cout << v[i][j] << ' ';
    cout << '\n';
  }
}
} // namespace base

namespace fft {
#define forn(i, n) for (int i = 0; i < (int)(n); i++)

void _fft2d(fft_type *__restrict__ M, fft_type *__restrict__ W, int N,
            int rowsize, fft_type root) {
  if (N == 1) return;
  if (N == 2) {
#define Y(y, x) (M[(y)*rowsize + (x)])
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

  int n = N >> 1;
#define X(y, x, i, j) (M[((y)*n + (i)) * rowsize + ((x)*n) + j])
  _fft2d(&X(0, 0, 0, 0), W, n, rowsize, root * root);
  _fft2d(&X(0, 1, 0, 0), W, n, rowsize, root * root);
  _fft2d(&X(1, 0, 0, 0), W, n, rowsize, root * root);
  _fft2d(&X(1, 1, 0, 0), W, n, rowsize, root * root);

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

void fft2d(fft_type *M, int N) {
#if defined(CXX_MEASURE_TIME)
  struct timespec start, end;
  clock_gettime(CLOCK_MONOTONIC, &start);
#endif
  const int logn = log2(N);

  auto root = std::polar(1., 2 * fft::pi / N);
  auto rev = new int[N];
  auto W = new fft_type[N >> 1];

  forn(i, N) {
    int revi = 0;
    forn(l, logn) revi |= ((i >> l) & 1) << (logn - l - 1);
    rev[i] = revi;
  }

  forn(i, N) {
    int rev_i = rev[i];
    forn(j, N) {
      if ((i < rev_i) || ((i == rev_i) && (j < rev[j]))) {
        swap(M[i * N + j], M[rev_i * N + rev[j]]);
      }
    }
  }

  _fft2d(M, W, N, N, root);
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
    fft::fft2d(matrix, N);
    // cout << "Result:" << '\n';
    // forn(i, N) {
    //   forn(j, N) cout << matrix[i * N + j] << ' ';
    //   cout << '\n';
    // }
  }
}
