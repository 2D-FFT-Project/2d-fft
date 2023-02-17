# distutils: language = c++
# distutils: extra_compile_args = [ "-std=c++17" ]

from libcpp.vector cimport vector

from .fft cimport fft_impl


cdef class FFT:
    cdef fft_impl fft  # Hold a C++ instance which we're wrapping

    def __init__(self):
        self.fft = fft_impl()

    def multiply(self, a: vector[int], b: vector[int]) -> vector[int]:
        return self.fft.multiply(a, b)

    def smth(self):
        return self.fft.smth()
