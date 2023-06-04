from libcpp.complex cimport complex as cpp_complex

import numpy as np

cimport numpy as np

np.import_array()

from . cimport fft


def fft2d(
    np.ndarray[np.complex128_t, ndim=2] a,
    int n,
    int m,
    bint use_threads=True,
    bint inverse=False
):
    fft.fft2d(( < cpp_complex[double]*> a.data), n, m, use_threads, inverse)
    return a
