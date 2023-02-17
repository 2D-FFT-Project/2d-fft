import numpy as np

from fft_project import base


def slow_mult(array_a: np.array, array_b: np.array) -> np.array:
    result = [0] * (len(array_a) + len(array_b) - 1)
    for i, ai in np.ndenumerate(array_a):
        for j, bj in np.ndenumerate(array_b):
            result[i[0] + j[0]] += ai * bj
    return np.array(result)


def np_mult(array_a: np.array, array_b: np.array) -> np.array:
    return np.polymul(array_a, array_b)
