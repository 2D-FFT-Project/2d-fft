#include <bitset>
#include <iostream>
#include <stdint.h>

#include "fft_sample.h"

using std::string, std::cout;

#define forn(i, n) for (int i = 0; i < (int)(n); i++)

namespace base {
using std::string;
std::string getenv(string key, string null_value = "") {
  char *value = std::getenv(key.c_str());
  if (value == nullptr) return null_value;
  return string(value);
};

template <class T> inline void print(const std::vector<T> &v) {
  for (int i = 0; i < v.size(); i++)
    cout << v[i] << ' ';
  cout << '\n';
}

template <class T>
inline void print_matrix(const std::vector<std::vector<T>> &v) {
  for (int i = 0; i < v.size(); i++) {
    for (int j = 0; j < v[i].size(); j++)
      cout << v[i][j] << ' ';
    cout << '\n';
  }
}
} // namespace base

namespace fft_sample {
const double pi = acos(-1);

// FFT with allocations
void partial_fft(vector<fft_type> &v, fft_type root) {
  int n = v.size();
  if (n == 1) return;
  vector<vector<fft_type>> parts(2, vector<fft_type>(n / 2));
  forn(i, n) parts[i & 1][i / 2] = v[i];
  partial_fft(parts[0], root * root);
  partial_fft(parts[1], root * root);
  fft_type w = 1;
  for (int i = 0; i < n / 2; i++, w *= root) {
    v[i] = parts[0][i] + w * parts[1][i];
    v[i + n / 2] = parts[0][i] - w * parts[1][i];
  }
}

vector<fft_type> fft(const vector<fft_type> &v, bool inverse) {
  int n = v.size();
  auto array = v;
  partial_fft(array, std::polar(1., (inverse ? -2 : 2) * pi / n));
  if (inverse)
    for (auto &x : array)
      x = x.real() / n;
  return array;
}

void transpose(fft_matrix &v) {
  int n = v.size();
  forn(i, n) forn(j, n / 2) swap(v[i][j], v[j][i]);
}

// FFT without allocations
void no_alloc_partial_fft(fft_type *v, int n, fft_type root) {
  if (n == 1) return;
  no_alloc_partial_fft(v, n / 2, root * root);
  no_alloc_partial_fft(v + n / 2, n / 2, root * root);
  fft_type w = 1;
  for (int i = 0; i < n / 2; i++, w *= root) {
    auto t = w * v[i + n / 2];
    v[i + n / 2] = v[i] - t;
    v[i] = v[i] + t;
  }
}

void no_alloc_fft(fft_type *v, int n, bool inverse = false) {
  const int logn = log2(n);
  for (int i = 0; i < n; i++) {
    int k = 0;
    for (int l = 0; l < logn; l++)
      k |= ((i >> l & 1) << (logn - l - 1));
    if (i < k) std::swap(v[i], v[k]);
  }

  no_alloc_partial_fft(v, n, std::polar(1., (inverse ? -2 : 2) * pi / n));
  if (inverse) forn(i, n) v[i] = v[i].real() / n;
}

// 2D Cooley-Tukey FFT
void fft2d(fft_matrix &v, int N, fft_type root) {
  if (N == 1) return;
  const int n = N >> 1;
  // TODO(gyzavyalov): Remove allocations
  vector matricies(2, vector(2, fft_matrix(n, vector<fft_type>(n))));
  forn(i, N) forn(j, N) matricies[i & 1][j & 1][i / 2][j / 2] = v[i][j];

  fft2d(matricies[0][0], n, root * root);
  fft2d(matricies[0][1], n, root * root);
  fft2d(matricies[1][0], n, root * root);
  fft2d(matricies[1][1], n, root * root);

  fft_type wi = 1;
  for (int i = 0; i < n; i++, wi *= root) {
    fft_type wj = 1;
    for (int j = 0; j < n; j++, wj *= root) {
      // clang-format off
        v[i    ][j    ] = matricies[0][0][i][j] + matricies[1][0][i][j] * wi + matricies[0][1][i][j] * wj + matricies[1][1][i][j] * wi * wj;
        v[i + n][j    ] = matricies[0][0][i][j] - matricies[1][0][i][j] * wi + matricies[0][1][i][j] * wj - matricies[1][1][i][j] * wi * wj;
        v[i    ][j + n] = matricies[0][0][i][j] + matricies[1][0][i][j] * wi - matricies[0][1][i][j] * wj - matricies[1][1][i][j] * wi * wj;
        v[i + n][j + n] = matricies[0][0][i][j] - matricies[1][0][i][j] * wi - matricies[0][1][i][j] * wj + matricies[1][1][i][j] * wi * wj;
      // clang-format on
    }
  }
}
} // namespace fft_sample

int main(int argc, char **argv) {
  const int multid_fft = stoi(base::getenv("MULTID", "0"));
  if (multid_fft) {
    const int N = 4;
    std::vector array2d(N, std::vector<fft_sample::fft_type>(N));
    {
      forn(i, N) forn(j, N) array2d[i][j] = i * j;
      cout << "Base matrix: \n";
      base::print_matrix(array2d);
    }
    {
      fft_sample::fft2d(array2d, N, std::polar(1., 2 * fft_sample::pi / N));
      cout << "Result:" << '\n';
      base::print_matrix(array2d);
    }
  } else {
    int n = 8;
    fft_sample::fft_type *v = new fft_sample::fft_type[n]{
        {0, 0}, {1, 0}, {2, 0}, {3, 0}, {5, 0}, {6, 0}, {7, 0}, {8, 0},
    };
    std::vector<fft_sample::fft_type> array(v, v + n);

    fft_sample::no_alloc_fft(v, n);
    forn(i, n) cout << v[i] << ' ';
    cout << '\n';
    delete[] v;

    auto res = fft_sample::fft(array, false);
    base::print(res);
  }
}
