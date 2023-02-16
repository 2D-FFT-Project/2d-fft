# isort: skip_file
import os

from setuptools import setup, find_packages
from Cython.Build import cythonize

ignore_dirs = ['./venv']


def find_pyx(path='.'):
    cython_files = []
    for root, dirs, filenames in os.walk(path):
        ignore_current = False
        for dir in ignore_dirs:
            ignore_current |= root.startswith(dir)
        if ignore_current:
            continue
        for fname in filenames:
            if fname.endswith('.pyx'):
                cython_files.append(os.path.join(root, fname))
    print(f'Cythonizing: {cython_files}')
    return cython_files


setup(
    name='fft_project',
    packages=find_packages(),
    ext_modules=cythonize(find_pyx(), language_level=3, annotate=True),
    zip_safe=False,
)
