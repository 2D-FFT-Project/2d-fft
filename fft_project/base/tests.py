from fft_project.base import *

class BaseTest:    
    def subprocessTests():
        logging.debug(f'{inspect.currentframe().f_code.co_name}')
        cur_dir = os.path.dirname(os.path.realpath(__file__))
        test_str = 'abc\ncde'
        out = run_sub(['echo', test_str], show_logs=False)
        logging.debug(f'Expected:\t {test_str}')
        logging.debug(f'Got:\t {out[0:-1]}')
        assert(out[0:-1] == test_str)


def main():
    BaseTest.subprocessTests()


if __name__ == '__main__':
    prepare_logger()
    main()
