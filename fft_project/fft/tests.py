import logging
import time

import numpy as np

from fft_project import base
from fft_project import fft


class fft_tests:
    @base.testing.test
    def slow_test():
        result = fft.slow.slow_mult(np.array([1, 2, 3]), np.array([3, 2, 1]))
        result2 = fft.slow.np_mult(np.array([1, 2, 3]), np.array([3, 2, 1]))
        logging.debug(result)
        logging.debug(result2)
        assert np.array_equal(result, result2)
        assert np.array_equal(result2, np.array([3, 8, 14, 8, 3]))

    @base.testing.test
    def fft_cpp_impl_test():
        n = 4096
        logging.info(f'Matrix shape: {n}x{n}')
        result = np.zeros((n, n), dtype=np.complex128)
        result.real = np.random.rand(n, n)

        start = time.time()
        expected = np.fft.fft2(result)
        print(f'Numpy: {time.time() - start}')
        start = time.time()
        fft.fft2d(result, n, return_copy=False)
        print(f'Our: {time.time() - start}')
        expected = abs(expected)
        result = abs(result)
        assert np.allclose(result, expected, atol=0.05)


if __name__ == '__main__':
    base.prepare_logger(level=logging.DEBUG)
    tests_list = [
        fft_tests.slow_test,
        fft_tests.fft_cpp_impl_test,
    ]
    base.testing.run_tests(tests_list)
