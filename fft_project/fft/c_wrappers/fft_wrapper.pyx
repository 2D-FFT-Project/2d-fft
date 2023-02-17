# distutils: language = c++

from .fft cimport fft_impl


cdef class FFT:
    cdef fft_impl fft  # Hold a C++ instance which we're wrapping

    def __init__(self):
        self.fft = fft_impl()

    def smth(self):
        return self.fft.smth()
