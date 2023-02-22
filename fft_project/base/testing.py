import logging
import traceback

from fft_project.base import tools


def unit_test(test_func):
    def attributed_test():
        test_name = test_func.__name__
        logging.info(f'TEST: {test_name}')
        try:
            test_func()
            logging.info('SUCCEED!\n')
            return (True, test_name)
        except Exception:
            traceback.print_exc()
            logging.info('FAILED!\n')
            return (False, test_name)

    return attributed_test


def run_tests(tests, ignore: list = []):
    count = len(tests)
    failed = []
    logging.info(f'Executing {count} tests')
    for test in tests:
        (result, test_name) = test()
        if not result and test_name not in ignore:
            failed.append(test_name)
    succeed = count - len(failed)
    logging.info(f'{len(failed)} tests failed')
    if succeed == count:
        logging.info(f'SUCCEED!')
    else:
        logging.info('List of failed tests:')
        for test_name in failed:
            logging.info(test_name)
        logging.info(f'FAILED!')
        exit(1)
