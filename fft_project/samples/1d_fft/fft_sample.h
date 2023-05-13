#ifndef FFT_H
#define FFT_H

#include <complex>
#include <vector>

namespace fft_sample {
using std::vector, std::pair;

typedef std::complex<double> fft_type;
typedef vector<vector<fft_type>> fft_matrix;
vector<fft_type> fft(const vector<double> &v);
} // namespace fft_sample

#endif
