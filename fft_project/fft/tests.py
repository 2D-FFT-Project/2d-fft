import numpy
import inspect
import logging


from fft_project import fft
from fft_project import base


class fft_tests:    
  def dummy_test():
    logging.debug(f'TEST: {inspect.currentframe().f_code.co_name}')
    result = fft.multimply_matricies(
      numpy.array([1, 2, 3]),
      numpy.array([3, 2, 1])
    )
    logging.debug(result)
    assert(numpy.array_equal(result, numpy.array([3, 4, 3])))
    logging.debug('Test succeed!')


if __name__ == '__main__':
    base.prepare_logger(level=logging.DEBUG)
    fft_tests.dummy_test()
