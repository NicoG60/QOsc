QT = core gui network

TEMPLATE = lib
DEFINES += QOSC_LIBRARY

CONFIG += c++11

TARGET = QOSC

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    $$PWD/qosc.cpp \
    qoscarray.cpp \
    qoscbundle.cpp \
    qoscinterface.cpp \
    qoscmessage.cpp \
    qoscmethod.cpp \
    qoscpacket.cpp \
    qosctypehelper.cpp

include(../include.pri)

OBJECTS_DIR = tmp
MOC_DIR = tmp
DESTDIR = lib
