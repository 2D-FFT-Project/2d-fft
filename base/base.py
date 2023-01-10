import subprocess
import os
import sys
import logging
import inspect


def run_sub(args, env=None, cwd=None, show_logs=True):
    """Running subprocess with real-time logs

    """
    p = subprocess.Popen(args,
                         env=env,
                         cwd=cwd,
                         stdout=subprocess.PIPE,
                         universal_newlines=True,
                         encoding='utf-8')
    output = ''
   
    sys.stdout.flush()
    for stdout_line in iter(p.stdout.readline, ""):
        output += stdout_line
        if show_logs:
            sys.stdout.write(stdout_line)
            sys.stdout.flush()
    out = p.stdout
    p.stdout.close()
    return_code = p.wait()
    if return_code:
        raise subprocess.CalledProcessError(return_code, args)
    return output


_log = logging.getLogger(__name__)


def _default_handlers(stream, logging_level, include_time):
    """Return a list of the default logging handlers to use.

    Args:
      stream: See the configure_logging() docstring.
      include_time: See the configure_logging() docstring.
    """

    # Create the filter.
    def should_log(record):
        """Return whether a logging.LogRecord should be logged."""
        if record.name.startswith('blinkpy.third_party'):
            return False
        return True

    logging_filter = logging.Filter()
    logging_filter.filter = should_log

    # Create the handler.
    handler = logging.StreamHandler(stream)
    if include_time:
        prefix = '%(asctime)s - '
    else:
        prefix = ''

    if logging_level == logging.DEBUG:
        formatter = logging.Formatter(prefix +
                                      '%(name)s: [%(levelname)s] %(message)s')
    else:
        formatter = logging.Formatter(prefix + '%(message)s')

    handler.setFormatter(formatter)
    handler.addFilter(logging_filter)

    return [handler]


def configure_logging(logging_level=None,
                      logger=None,
                      stream=None,
                      handlers=None,
                      include_time=True):
    """Configure logging for standard purposes.

    Returns:
      A list of references to the logging handlers added to the root
      logger.  This allows the caller to later remove the handlers
      using logger.removeHandler.  This is useful primarily during unit
      testing where the caller may want to configure logging temporarily
      and then undo the configuring.

    Args:
      logging_level: The minimum logging level to log.  Defaults to
                     logging.INFO.
      logger: A logging.logger instance to configure.  This parameter
              should be used only in unit tests.  Defaults to the
              root logger.
      stream: A file-like object to which to log used in creating the default
              handlers.  The stream must define an "encoding" data attribute,
              or else logging raises an error.  Defaults to sys.stderr.
      handlers: A list of logging.Handler instances to add to the logger
                being configured.  If this parameter is provided, then the
                stream parameter is not used.
      include_time: Include time information at the start of every log message.
                    Useful for understanding how much time has passed between
                    subsequent log messages.
    """
    # If the stream does not define an "encoding" data attribute, the
    # logging module can throw an error like the following:
    #
    # Traceback (most recent call last):
    #   File "/System/Library/Frameworks/Python.framework/Versions/2.6/...
    #         lib/python2.6/logging/__init__.py", line 761, in emit
    #     self.stream.write(fs % msg.encode(self.stream.encoding))
    # LookupError: unknown encoding: unknown
    if logging_level is None:
        logging_level = logging.INFO
    if logger is None:
        logger = logging.getLogger()
    if stream is None:
        stream = sys.stderr
    if handlers is None:
        handlers = _default_handlers(stream, logging_level, include_time)

    logger.setLevel(logging_level)

    for handler in handlers:
        logger.addHandler(handler)

    _log.debug('Debug logging enabled.')

    return handlers

def prepare_logger(level=logging.DEBUG):
    logging.basicConfig(format='%(levelname)-8s  %(message)s', level=level)