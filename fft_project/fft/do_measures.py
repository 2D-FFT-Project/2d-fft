import logging
import time
import scipy
import numpy as np
import json
import pprint
from pathlib import Path

import fft_project
from fft_project import base


def random_matrix(n, m):
    logging.info(f'Matrix shape: {n}x{m}')
    result = np.zeros((n, m), dtype=np.complex128)
    result.real = np.random.rand(n, m)
    return result


def measure_our(a, b, sample_size=20):
  results = []
  for _ in range(sample_size):
    result = random_matrix(2 ** a, 2 ** b)
    # start = time.time()
    # expected = scipy.fft.fft2(result)
    # scipy_time = time.time() - start
    # logging.info(f'Scipy: {scipy_time}')
    start = time.time()
    result = fft_project.fft2d(result, return_copy=False, use_threads=False)
    our_time = time.time() - start
    results.append(our_time)
  return results


def measure_scipy(a, b, sample_size=20):
  results = []
  for _ in range(sample_size):
    result = random_matrix(2 ** a, 2 ** b)
    start = time.time()
    expected = scipy.fft.fft2(result)
    scipy_time = time.time() - start
    results.append(scipy_time)
  return results

class fft_tests:
  @base.testing.test
  def fft_cpp_impl_test():
    results_our = {}
    for x in range(6, 14):
      for y in range(x, 14):
        results_our[f'{2 ** x}x{2 ** y}'] = measure_our(x, y)
    Path('results_our').write_text(json.dumps(results_our, indent=2))

  @base.testing.test
  def scipy_cpp_impl_test():
    results_our = {}
    for x in range(6, 14):
      for y in range(x, 14):
        results_our[f'{2 ** x}x{2 ** y}'] = measure_scipy(x, y)
    Path('results_scipy').write_text(json.dumps(results_our, indent=2))


if __name__ == '__main__':
    base.prepare_logger(level=logging.DEBUG)
    tests_list = [
        fft_tests.fft_cpp_impl_test,
        fft_tests.scipy_cpp_impl_test,
    ]
    base.testing.run_tests(tests_list)
