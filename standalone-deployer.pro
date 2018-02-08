CONFIG += warn_on
CONFIG += debug_and_release

TEMPLATE = subdirs

CONFIG += ordered

SUBDIRS += _logger
SUBDIRS += _settings
SUBDIRS += _io
SUBDIRS += loader
SUBDIRS += updater
SUBDIRS += installer
