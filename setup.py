# isort: skip_file
import os
import numpy

from setuptools import setup, find_packages
from Cython.Build import cythonize
from distutils.extension import Extension

ignore_dirs = ['./venv']

CXX_FLAGS = [
    '-O3',
    '-std=c++17',
    '-Wno-sign-compare',
    '-ffast-math',
    '-DUSE_THREAD',
    '-Wno-macro-redefined',
    '-DNPY_NO_DEPRECATED_API',
]

CXX_INCLUDE = ['fft_project/fft/c_wrappers', numpy.get_include()]

EXTENSIONS = [
    Extension(
        name='fft_project.fft.c_wrappers.fft_wrapper',
        sources=['fft_project/fft/c_wrappers/fft_wrapper.pyx'],
        language='c++',
        include_dirs=CXX_INCLUDE,
        extra_compile_args=CXX_FLAGS,
    )
]

setup(
    name='fft_project',
    install_requires=[
        'numpy>=1.19.0',
        'cython>=0.29.21',
    ],
    setup_requires=[
        'numpy>=1.19.0',
        'cython>=0.29.21',
    ],
    version='1.0.0',
    description='A package for 2D FFT computations',
    packages=find_packages(),
    ext_modules=cythonize(EXTENSIONS, language_level=3, annotate=True),
    zip_safe=False,
)
