import inspect
import logging
import os

from fft_project import base
from fft_project.base import testing


class base_tests:
    @testing.unit_test
    def subprocessTest():
        logging.debug(f'TEST: {inspect.currentframe().f_code.co_name}')
        test_str = 'abc\ncde'
        out = base.run_sub(['echo', test_str], show_logs=True)
        logging.debug(f'Expected:\n{test_str}')
        logging.debug(f'Got:\n{out[0:-1]}')
        assert out[0:-1] == test_str


def main():
    tests_list = [base_tests.subprocessTest]
    testing.run_tests(tests_list)


if __name__ == '__main__':
    base.prepare_logger(level=logging.DEBUG)
    main()
