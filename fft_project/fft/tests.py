import inspect
import logging
import time

import numpy as np

from fft_project import base
from fft_project import fft


class fft_tests:
    def slow_test():
        logging.debug(f'TEST: {inspect.currentframe().f_code.co_name}')
        result = fft.slow.slow_mult(np.array([1, 2, 3]), np.array([3, 2, 1]))
        result2 = fft.slow.np_mult(np.array([1, 2, 3]), np.array([3, 2, 1]))
        logging.debug(result)
        logging.debug(result2)
        assert np.array_equal(result, result2)
        assert np.array_equal(result2, np.array([3, 8, 14, 8, 3]))
        logging.debug('Test succeed!\n')

    def fft_c_impl_dummy_test():
        logging.debug(f'TEST: {inspect.currentframe().f_code.co_name}')
        fft_inst = fft.FFT()
        fft_inst.smth()
        logging.debug('Test succeed!\n')

    def fft_cpp_impl_test():
        logging.debug(f'TEST: {inspect.currentframe().f_code.co_name}')
        fft_inst = fft.FFT()
        a, b = np.array([1, 2, 3]), np.array([3, 2, 1])
        result = np.array(fft_inst.multiply(list(a), list(b)))
        expected = fft.slow.np_mult(a, b)
        logging.debug(result)
        logging.debug(expected)
        assert np.array_equal(np.array(result), expected)
        logging.debug('Test succeed!\n')

    def slow_perf_test():
        logging.debug(f'TEST: {inspect.currentframe().f_code.co_name}')
        expected_time = 9
        array_a = np.random.randint(100, size=5000)
        array_b = np.random.randint(100, size=5000)
        logging.debug('Running...')
        start_time = time.time()
        fft.slow.slow_mult(array_a, array_b)
        execution_time = time.time() - start_time
        logging.debug(f'Execution time: {execution_time}s')
        logging.debug(f'Expected time: {expected_time}s')
        assert execution_time <= expected_time
        logging.debug('Test succeed!\n')


if __name__ == '__main__':
    base.prepare_logger(level=logging.DEBUG)
    fft_tests.slow_test()
    fft_tests.fft_cpp_impl_test()
    fft_tests.slow_perf_test()
    fft_tests.fft_c_impl_dummy_test()
