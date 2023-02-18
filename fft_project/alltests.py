import logging
import os

import base.tests
import fft.tests
from base.tests import base_tests
from fft.tests import fft_tests


def main():
    fft_tests.slow_perf_test()
    fft_tests.fft_c_impl_dummy_test()
    fft_tests.slow_test()
    base_tests.subprocessTest()


if __name__ == '__main__':
    base.prepare_logger(level=logging.DEBUG)
    main()
