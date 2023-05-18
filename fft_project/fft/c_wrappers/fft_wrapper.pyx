from libcpp.complex cimport complex as cpp_complex

import numpy as np

cimport numpy as np

np.import_array()

from . cimport fft


def fft2d(np.ndarray[np.complex128_t, ndim=2] a, int n, bint return_copy=True):
    if not return_copy:
        fft.fft2d((< cpp_complex[double]*> a.data), n)
        return
    cdef np.ndarray[np.complex128_t, ndim = 2] b = a.copy()
    fft.fft2d((< cpp_complex[double]*> b.data), n)
    return b
