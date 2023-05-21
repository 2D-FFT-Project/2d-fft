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
        n = 4096 * 2
        logging.info(f'Matrix shape: {n}x{n}')
        result = np.zeros((n, n), dtype=np.complex128)
        result.real = np.random.rand(n, n)

        start = time.time()
        expected = np.fft.fft2(result)
        numpy_time = time.time() - start
        print(f'Numpy: {numpy_time}')
        start = time.time()
        result = fft.fft2d(result, return_copy=True)
        our_time = time.time() - start
        print(f'Our: {our_time}')
        print(
            f'{our_time / numpy_time * 100:.2f}% of Numpy time!',
        )
        expected = abs(expected)
        result = abs(result)
        correctness = np.allclose(result, expected, atol=0.05)
        print(f'Is the result correct: {correctness}')
        assert correctness


if __name__ == '__main__':
    base.prepare_logger(level=logging.DEBUG)
    tests_list = [
        fft_tests.slow_test,
        fft_tests.fft_cpp_impl_test,
    ]
    base.testing.run_tests(tests_list)
