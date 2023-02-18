import logging
import os

from fft.tests import fft_tests

from fft_project import base


def main():
    fft_tests.slow_perf_test()
    fft_tests.fft_c_impl_dummy_test()
    fft_tests.slow_test()
    base_tests.subprocessTest()


if __name__ == '__main__':
    base.prepare_logger(level=logging.DEBUG)
    main()
