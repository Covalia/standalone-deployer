CONFIG += warn_on
CONFIG += debug_and_release

TEMPLATE = subdirs

CONFIG += ordered

SUBDIRS += _logger
SUBDIRS += _settings
SUBDIRS += _filesystem
SUBDIRS += loader
SUBDIRS += downloader
SUBDIRS += installer
SUBDIRS += uninstaller
