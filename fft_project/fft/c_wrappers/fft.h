#ifndef FFT_H
#define FFT_H
#include <complex>
#include <vector>

namespace fft {
using std::vector, std::pair, std::complex;

const double pi = acos(-1);
typedef complex<double> fft_type;
typedef fft_type *fft_matrix;

void fft2d(complex<double> *V, int N, int M, int use_threads);
} // namespace fft

#endif
