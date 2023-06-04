import logging
import os

from fft.ci_tests import ci_tests
from fft.tests import fft_tests

from fft_project import base
from fft_project.base.tests import base_tests


def main():
    tests_list = [
        ci_tests.rect_stress,
        ci_tests.square_stress,
        fft_tests.fft_cpp_impl_test,
        fft_tests.slow_test,
        fft_tests.inverse_test,
    ]
    base.testing.run_tests(tests_list)


if __name__ == '__main__':
    base.prepare_logger(level=logging.DEBUG)
    main()
