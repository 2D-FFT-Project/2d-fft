cdef extern from "fft.cpp":
    pass

cdef extern from "fft.h" namespace "fft":
    cdef cppclass fft_impl:
        fft_impl() except +
        void smth()
