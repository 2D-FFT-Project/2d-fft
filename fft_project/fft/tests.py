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
        fft_inst = fft.FFT()
        a, b = np.array([1, 2, 3]), np.array([3, 2, 1])
        result = np.array(fft_inst.multiply(list(a), list(b)))
        expected = fft.slow.np_mult(a, b)
        logging.debug(result)
        logging.debug(expected)
        assert np.array_equal(np.array(result), expected)

    @base.testing.test
    def c_impl_perf_test():
        array_size = 5000
        array_a = np.random.randint(100, size=array_size)
        array_b = np.random.randint(100, size=array_size)
        logging.debug('Running...')
        start_time = time.time()
        res1 = fft.slow.slow_mult(array_a, array_b)
        slow_time = time.time()
        res2 = fft.FFT().multiply(array_a, array_b)
        fast_time = time.time()
        res3 = fft.slow.np_mult(array_a, array_b)
        numpy_time = time.time()
        slow, fast, numpy = (
            slow_time - start_time,
            fast_time - slow_time,
            numpy_time - fast_time,
        )
        logging.debug(f'Slow: {slow}s')
        logging.debug(f'Numpy: {numpy}s')
        logging.debug(f'Fast: {fast}s')
        assert np.array_equal(res1, res2)
        assert np.array_equal(res2, res3)
        logging.debug(f'Equal results check succeed!')
        assert fast <= slow
        logging.debug(f'Fast <= Slow check succeed!')

    @base.testing.test
    def c_impl_stress_test():
        low, high = -100, 100
        count, size = 100, 2

        def stress(array_size):
            array_a = np.random.randint(low=low, high=high, size=array_size)
            array_b = np.random.randint(low=low, high=high, size=array_size)
            res_impl = fft.FFT().multiply(array_a, array_b)
            res_numpy = np.polymul(array_a, array_b)

            if not np.array_equal(res_impl, res_numpy):
                logging.warning(f'Failed stress test data:')
                logging.warning(f'a: {array_a}, b: {array_b}')
                logging.warning(f'impl: {res_impl}')
                logging.warning(f'numpy: {res_numpy}')
                raise Exception('Failed stress test!')

        logging.debug('Stress tests specification: ')
        logging.debug(
            f'Tests count: {count}, Array size: {size}, '
            f'Elements range: [{low}; {high})'
        )
        logging.debug('Running...')
        for i in range(count):
            stress(array_size=size)


if __name__ == '__main__':
    base.prepare_logger(level=logging.DEBUG)
    tests_list = [
        fft_tests.slow_test,
        fft_tests.c_impl_stress_test,
        fft_tests.c_impl_perf_test,
        fft_tests.fft_cpp_impl_test,
    ]
    base.testing.run_tests(tests_list)
