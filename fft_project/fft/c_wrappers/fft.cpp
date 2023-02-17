#include <iostream>

#include <cmath>
#include <complex>

#include "fft.h"

namespace fft {
  using namespace std;
  typedef complex<double> ftype;

  const double pi = acos(-1);

  fft_impl::fft_impl() {}

  vector<int> fft_impl::multiply(
    vector<int> a,
    vector<int> b
  ) {
    int len = a.size() + b.size();
    while (a.size() < len) a.push_back(0);
    while (b.size() < len) b.push_back(0);
    vector<ftype> A = fft_impl::evaluate(a);
    vector<ftype> B = fft_impl::evaluate(b);

    for (int i = 0; i < int(A.size()); i++)
      A[i] *= B[i];
    auto x = fft_impl::interpolate(A);
    while (x.back() == 0) x.pop_back();
    return x;
  }

  vector<ftype> fft_impl::evaluate(vector<int> p) {
    while (__builtin_popcount(p.size()) != 1)
      p.push_back(0);
    vector<ftype> np;
    for (int x : p) np.push_back(ftype(x, 0));
    return fft_impl::fft(np, polar(1., 2 * pi / p.size()));
  }

  vector<int> fft_impl::interpolate(vector<ftype> p) {
    int n = p.size();
    auto inv = fft_impl::fft(p, polar(1., -2 * pi / n));
    vector<int> res(n);
    for (int i = 0; i < n; i++)
      res[i] = round(real(inv[i]) / n);
    return res;
  }

  vector<ftype> fft_impl::fft(vector<ftype> p, ftype wn) {
    int n = (int) p.size();
    if (n == 1)
      return p;

    vector<ftype> a(n / 2), b(n / 2);
    for (int i = 0; i < n / 2; i++) {
      a[i] = p[2 * i];
      b[i] = p[2 * i + 1];
    }

    a = fft_impl::fft(a, wn * wn);
    b = fft_impl::fft(b, wn * wn);

    ftype w = 1;
    for (int i = 0; i < n / 2; i++) {
      p[i] = a[i] + w * b[i];
      p[i + n / 2] = a[i] - w * b[i];
      w *= wn;
    }
    return p;
  }
}
