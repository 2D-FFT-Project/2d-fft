#ifndef FFT_H
#define FFT_H
#include <complex>
#include <vector>

namespace fft {
using std::vector, std::pair, std::complex;

const double pi = acos(-1);
typedef complex<double> fft_type;
typedef vector<vector<fft_type>> fft_matrix;

void fft2d(vector<vector<complex<double>>> &v, int N, complex<double> root);
} // namespace fft

#endif
