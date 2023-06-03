import numpy as np

from . import c_wrappers


def fft2d(
    v: np.ndarray, return_copy: bool = False, use_threads: bool = True
) -> np.ndarray:
    """
    Perform a 2D fast Fourier transform on the input array.

    Parameters
    ----------
    a : np.ndarray
        Input array of shape (N, N), where N is a power of 2
    return_copy : bool, optional
        If True, return a copy of the transformed array. If False (default),
        perform the transformation in-place and return the original array.
    use_threads : bool, optional
        If True (default), use multithreading.

    Returns
    -------
    np.ndarray
        Transformed array of shape (N, N). If `return_copy` is True, this
        function returns a new array. Otherwise, the input array is modified
        in-place and returned.

    Raises
    ------
    ValueError
        If the input array is not two-dimensional.

    Notes
    -----
    This function uses the `numpy.ndarray[np.complex128]`
    as an input / output type of matrix

    """
    a = v.astype(np.complex128, copy=True) if return_copy else v
    return c_wrappers.fft2d(a, a.shape[0], a.shape[1], use_threads)
