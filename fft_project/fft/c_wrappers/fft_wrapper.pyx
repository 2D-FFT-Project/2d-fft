from libcpp.complex cimport complex as cpp_complex
from libcpp.vector cimport vector

from . cimport fft


def fft2d(a: vector[vector[cpp_complex[double]]], n: int, root: cpp_complex[double]) -> vector[vector[cpp_complex[double]]]:
    fft.fft2d(a, n, root)
    return a
