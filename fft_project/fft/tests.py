import logging
import time

import numpy as np

import fft_project
from fft_project import base


def random_matrix(n, m):
    logging.info(f'Matrix shape: {n}x{m}')
    result = np.zeros((n, m), dtype=np.complex128)
    result.real = np.random.rand(n, m)
    return result


class fft_tests:
    @base.testing.test
    def fft_cpp_impl_test():
        result = random_matrix(4096, 4096 * 2)
        start = time.time()
        expected = np.fft.fft2(result)
        numpy_time = time.time() - start
        logging.info(f'Numpy: {numpy_time}')
        start = time.time()
        result = fft_project.fft2d(result, return_copy=True, use_threads=False)
        our_time = time.time() - start
        logging.info(f'Our: {our_time}')
        logging.info(f'{our_time / numpy_time * 100:.2f}% of Numpy time!')
        expected = abs(expected)
        result = abs(result)
        correctness = np.allclose(result, expected, atol=0.05)
        logging.info(f'Is the result correct: {correctness}')
        assert correctness

    @base.testing.test
    def inverse_test():
        a = random_matrix(4096 * 2, 4096)
        transformed = fft_project.fft2d(a, return_copy=True, use_threads=True)
        start = time.time()
        inversed_numpy = np.fft.ifft2(transformed)
        numpy_time = time.time() - start
        logging.info(f'Numpy: {numpy_time}')
        inversed_our = transformed
        start = time.time()
        inversed_our = fft_project.fft2d(
            inversed_our, return_copy=True, use_threads=False, inverse=True
        )
        our_time = time.time() - start
        logging.info(f'Our: {our_time}')
        logging.info(f'{our_time / numpy_time * 100:.2f}% of Numpy time!')
        inversed_numpy = abs(inversed_numpy)
        inversed_our = abs(inversed_our)
        correctness = np.allclose(inversed_our, inversed_numpy, atol=0.05)
        logging.info(f'Is the result correct: {correctness}')
        assert correctness


if __name__ == '__main__':
    base.prepare_logger(level=logging.DEBUG)
    tests_list = [
        fft_tests.fft_cpp_impl_test,
        fft_tests.inverse_test,
    ]
    base.testing.run_tests(tests_list)
