from libcpp.vector cimport vector


cdef extern from "fft.cpp":
    pass

cdef extern from "fft.h" namespace "fft":
    cdef cppclass fft_impl:
        fft_impl() except +
        void smth()
        vector[int] multiply(vector[int] a, vector[int] b)
