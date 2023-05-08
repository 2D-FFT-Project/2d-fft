#include <iostream>

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

  template<class T>
  inline void print(const std::vector<T> &v) {
    for (int i = 0; i < v.size(); i++)
      cout << v[i] << ' ';
    cout << '\n';
  }
}

namespace fft_sample {
  const double pi = acos(-1);

  void partial_fft(vector<fft_type> &v, fft_type root) {
    int n = v.size();
    if (n == 1) return;
    vector<vector<fft_type>> parts(2, vector<fft_type>(n / 2));
    forn (i, n) parts[i & 1][i / 2] = v[i];
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
    if (inverse) for (auto &x: array) x = x.real() / n;
    return array;
  }

  void transpose(vector<vector<fft_type>> &v) {
    int n = v.size();
    forn (i, n) forn (j, n / 2) swap(v[i][j], v[j][i]);
  }

  vector<vector<fft_type>> fft2(const vector<vector<fft_type>> &v, bool inverse) {
    int n = v.size();
    auto array = v;
    forn (i, n) partial_fft(array[i], std::polar(1., (inverse ? -2 : 2) * pi / n));
    if (inverse) forn(i, n) for (auto &x: array[i]) x = x.real() / n;
    transpose(array);
    forn (i, n) partial_fft(array[i], std::polar(1., (inverse ? -2 : 2) * pi / n));
    if (inverse) forn(i, n) for (auto &x: array[i]) x = x.real() / n;
    transpose(array);
    return array;
  } 
}

int main(int argc, char **argv) {
  const int multid_fft = stoi(base::getenv("MULTID", "0"));
  if (multid_fft) {
    std::vector<std::vector<fft_sample::fft_type>> array2d = {
      {{   1, 0}, {  1, 0}},
      {{-2.5, 0}, {3.7, 0}}
    };

    auto res = fft_sample::fft2(array2d, false);
    for (auto x: res) {
      base::print(x);
    }
    cout << '\n';
    res = fft_sample::fft2(res, true);
    for (auto x: res) {
      base::print(x);
    }
    return 0;
  } else {
    std::vector<fft_sample::fft_type> array;
    for (double x : {0, 1, 2, 3}) array.push_back({x, 0.0});
    auto res = fft_sample::fft(array, false);
    base::print(res);
    res = fft_sample::fft(res, true);
    base::print(res);
  }
}
