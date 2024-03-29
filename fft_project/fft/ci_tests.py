import logging
import random

import numpy as np

from fft_project import base
from fft_project import fft

VERBOSE = False
SIZE = 1000


def _test_random_matrix(n, m, i):
    result = np.zeros((n, m), dtype=np.complex128)
    result.real = np.random.rand(n, m)
    expected = np.fft.fft2(result)
    _ = fft.fft2d(result, return_copy=False)
    expected = abs(expected)
    result = abs(result)
    correctness = np.allclose(result, expected, atol=0.05)
    if VERBOSE:
        print(f'Test #{i}: {"Correct" if correctness else "Wrong"}')
    assert correctness


class ci_tests:
    @base.testing.test
    def square_stress():
        for i in range(SIZE):
            n = 2 ** random.randint(1, 10)
            _test_random_matrix(n, n, i)

    @base.testing.test
    def rect_stress():
        for i in range(SIZE):
            _test_random_matrix(
                2 ** random.randint(1, 10), 2 ** random.randint(1, 10), i
            )


if __name__ == '__main__':
    base.prepare_logger(level=logging.DEBUG)
    tests_list = [
        ci_tests.square_stress,
        ci_tests.rect_stress,
    ]
    base.testing.run_tests(tests_list)
