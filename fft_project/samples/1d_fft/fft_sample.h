#ifndef FFT_H
#define FFT_H

#include <vector>
#include <complex>

namespace fft_sample {
  using std::vector;
  typedef std::complex<double> fft_type;
  vector<fft_type> fft(const vector<double> &v);
}

#endif
