from fft_project import fft
from fft_project import base

import logging
import numpy

def main():
  logging.info(
    fft.multimply_matricies(
      numpy.array([1, 2, 3]),
      numpy.array([3, 2, 1])
    )
  )

if __name__ == '__main__':
    base.prepare_logger()
    main()