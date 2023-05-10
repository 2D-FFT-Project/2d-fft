#ifndef FFT_H
#define FFT_H
#include <vector>

namespace fft {
using namespace std;
struct fft_impl {
private:
  typedef complex<double> ftype;
  vector<ftype> fft(vector<ftype> p, ftype wn);
  vector<int> interpolate(vector<ftype> p);
  vector<ftype> evaluate(vector<int> p);

public:
  fft_impl();
  vector<int> multiply(vector<int> a, vector<int> b);
};
} // namespace fft

#endif
