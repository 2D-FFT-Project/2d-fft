from libcpp.complex cimport complex
from libcpp.vector cimport vector


cdef extern from "fft.cpp":
    pass

cdef extern from "fft.h" namespace "fft":
    void fft2d(complex[double]* v, int N, int M, int use_threads) except +
