import logging
import time
import scipy
import numpy as np

import fft_project
from fft_project import base


def random_matrix(n, m):
    logging.info(f'Matrix shape: {n}x{m}')
    result = np.zeros((n, m), dtype=np.complex64)
    result.real = np.random.rand(n, m)
    return result


class fft_tests:
    @base.testing.test
    def fft_cpp_impl_test():
        result = random_matrix(4096, 4096 * 2)
        start = time.time()
        expected = scipy.fft.fft2(result)
        scipy_time = time.time() - start
        logging.info(f'Scipy: {scipy_time}')
        start = time.time()
        result = fft_project.fft2d(result, return_copy=False, use_threads=False)
        our_time = time.time() - start
        logging.info(f'Our: {our_time}')
        logging.info(f'{our_time / scipy_time * 100:.2f}% of Scipy time!')
        expected = abs(expected)
        result = abs(result)
        correctness = np.allclose(result, expected, atol=0.05)
        logging.info(f'Is the result correct: {correctness}')
        assert correctness

    @base.testing.test
    def inverse_test():
        a = random_matrix(4096 * 2, 4096)
        transformed = fft_project.fft2d(a, return_copy=False, use_threads=True)
        start = time.time()
        inversed_scipy = scipy.fft.ifft2(transformed, workers = -1)
        scipy_time = time.time() - start
        logging.info(f'Scipy: {scipy_time}')
        inversed_our = transformed
        start = time.time()
        inversed_our = fft_project.fft2d(
            inversed_our, return_copy=False, use_threads=True, inverse=True
        )
        our_time = time.time() - start
        logging.info(f'Our: {our_time}')
        logging.info(f'{our_time / scipy_time * 100:.2f}% of Scipy time!')
        inversed_scipy = abs(inversed_scipy)
        inversed_our = abs(inversed_our)
        correctness = np.allclose(inversed_our, inversed_scipy, atol=0.05)
        logging.info(f'Is the result correct: {correctness}')
        assert correctness


if __name__ == '__main__':
    base.prepare_logger(level=logging.DEBUG)
    tests_list = [
        fft_tests.fft_cpp_impl_test,
        fft_tests.inverse_test,
    ]
    base.testing.run_tests(tests_list)
