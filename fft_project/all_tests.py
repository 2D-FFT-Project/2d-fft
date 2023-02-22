import logging
import os

from fft.tests import fft_tests

from fft_project import base
from fft_project.base.tests import base_tests


def main():
    ignore = [
        'c_impl_stress_test',
    ]
    tests_list = [
        fft_tests.c_impl_perf_test,
        fft_tests.c_impl_stress_test,
        fft_tests.fft_cpp_impl_test,
        fft_tests.slow_test,
        base_tests.subprocessTest,
    ]
    base.testing.run_tests(tests_list, ignore)


if __name__ == '__main__':
    base.prepare_logger(level=logging.DEBUG)
    main()
