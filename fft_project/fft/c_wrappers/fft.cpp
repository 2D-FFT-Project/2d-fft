#include "fft.h"

#include <iostream>

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

void _fft2d(fft_type *M, int N, int rowsize, fft_type root) {
  if (N == 1) return;
  int n = N / 2;

#define X(y, x, i, j) (M[((y)*n + (i)) * rowsize + ((x)*n) + j])

  _fft2d(&X(0, 0, 0, 0), n, rowsize, root * root);
  _fft2d(&X(0, 1, 0, 0), n, rowsize, root * root);
  _fft2d(&X(1, 0, 0, 0), n, rowsize, root * root);
  _fft2d(&X(1, 1, 0, 0), n, rowsize, root * root);


  auto W = new fft_type[n];
  W[0] = 1;
  forn (i, n - 1) W[i + 1] = W[i] * root;
  for (int i = 0; i < n; i++) {
    for (int j = 0; j < n; j++) {
      auto x10 = X(1, 0, i, j) * W[i];
      auto x01 = X(0, 1, i, j) * W[j];
      auto x11 = X(1, 1, i, j) * W[i] * W[j];
      X(1, 0, i, j) = X(0, 0, i, j) - x10 + x01 - x11;
      X(0, 1, i, j) = X(0, 0, i, j) + x10 - x01 - x11;
      X(1, 1, i, j) = X(0, 0, i, j) - x10 - x01 + x11;

      X(0, 0, i, j) = X(0, 0, i, j) + x10 + x01 + x11;
    }
  }
}

void fft2d(fft_type *M, int N) {
#if defined(CXX_MEASURE_TIME)
  struct timespec start, end;
  clock_gettime(CLOCK_MONOTONIC, &start);
#endif
  auto root = std::polar(1., 2 * fft::pi / N);

  const int logn = log2(N);

  auto rev = new int[N];
  forn (i, N) {
    int revi = 0;
    forn (l, logn)
      revi |= ((i >> l) & 1) << (logn - l - 1);
    rev[i] = revi;
  }

  forn (i, N) {
    forn (j, N) {
      if ((i < rev[i]) || ((i == rev[i]) && (j < rev[j]))) {
        swap(M[i * N + j], M[rev[i] * N + rev[j]]);
      }
    }
  }
  _fft2d(M, N, N, root);
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
    forn(i, N) forn(j, N) matrix[i * N + j] = 2 * i + j;
    forn(i, N) {
      forn(j, N) cout << matrix[i * N + j] << ' ';
      cout << '\n';
    }
  }
  {
    cout << string(8, '-') << '\n';
    fft::fft2d(matrix, N);
    cout << "Result:" << '\n';
    forn(i, N) {
      forn(j, N) cout << matrix[i * N + j] << ' ';
      cout << '\n';
    }
  }
}
