#!/bin/bash

FFT_PROJECT_DIR=$PWD

#Creating enviroment
python3 -m venv venv

# Starting enviroment
source ./venv/bin/activate
# To deactivate run 'deactivate' in shell

# Syncing python packages
pip3 install -r dev-utils/requirements.txt
pre-commit install

python3 -m pip install -e .

# Project functions
fft_build() {
 python3 -m pip install -e $FFT_PROJECT_DIR
}
