QT = core gui network core_private

TEMPLATE = lib

CONFIG += c++11
DEFINES += QOSC_LIBRARY

VERSION = 1.0.1
DEFINES += GIT_VERSION=$$system(git rev-parse --short HEAD)
DEFINES += SOFT_VERSION=$$VERSION

TARGET = QOsc

SOURCES += \
    $$PWD/qosc_global.cpp \
    $$PWD/qoscvalue.cpp \
    $$PWD/qoscbundle.cpp \
    $$PWD/qoscinterface.cpp \
    $$PWD/qoscmessage.cpp \
    $$PWD/qoscmethod.cpp

include($$PWD/../include/include.pri)

OBJECTS_DIR = tmp
MOC_DIR = tmp
DESTDIR = lib

HEADERS += \
    $$PWD/qoscbundle_p.h \
    $$PWD/qoscmessage_p.h \
    $$PWD/qoscvalue_p.h
