import logging
import subprocess
import sys

_log = logging.getLogger(__name__)


def prepare_logger(level=logging.INFO):
    logging.basicConfig(format='[%(levelname)-4s]  %(message)s', level=level)


def run_sub(args, env=None, cwd=None, show_logs=True):
    """Running subprocess with real-time logs"""
    p = subprocess.Popen(
        args,
        env=env,
        cwd=cwd,
        stdout=subprocess.PIPE,
        universal_newlines=True,
        encoding='utf-8',
    )
    output = ''
    for stdout_line in iter(p.stdout.readline, ''):
        output += stdout_line
        if not show_logs:
            continue
        sys.stdout.write(stdout_line)
        sys.stdout.flush()
    p.stdout.close()
    return_code = p.wait()
    if return_code:
        raise subprocess.CalledProcessError(return_code, args)
    return output
