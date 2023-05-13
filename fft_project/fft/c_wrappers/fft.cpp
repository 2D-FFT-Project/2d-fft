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
} // namespace fft

int main(int argc, char **argv) {
  using namespace std;
  const uint32_t N = stoi(string(argv[1]));
  assert(__builtin_popcount(N) == 1);
  fft::fft_matrix array2d(N, std::vector<fft::fft_type>(N));
  {
    forn(i, N) forn(j, N) array2d[i][j] = i * j;
    cout << "Base matrix: \n";
    base::print_matrix(array2d);
  }
  {
    fft::fft2d(array2d, N, std::polar(1., 2 * fft::pi / N));
    cout << "Result:" << '\n';
    base::print_matrix(array2d);
  }
}
