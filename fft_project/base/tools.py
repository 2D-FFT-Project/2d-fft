import logging
import subprocess
import sys

_log = logging.getLogger(__name__)


def prepare_logger(level=logging.INFO):
    logging.basicConfig(format='[%(levelname)-4s]  %(message)s', level=level)
