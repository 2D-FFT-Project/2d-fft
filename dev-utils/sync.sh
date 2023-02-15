#!/bin/bash

BASEDIR=$(dirname $0)

#Creating enviroment
python3 -m venv venv

# Starting enviroment
source ./venv/bin/activate
# To deactivate run 'deactivate' in shell

# Syncing python packages
pip3 install -r dev-utils/requirements.txt
python3 -m pip install -e .