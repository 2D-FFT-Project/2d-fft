import logging

from fft_project import base
from fft_project.samples import cython_playground 


def main():
  cython_playground.fib(10)


if __name__ == '__main__':
    base.prepare_logger(level=logging.DEBUG)
    main()
