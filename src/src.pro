QT = core gui network

TEMPLATE = lib
DEFINES += QOSC_LIBRARY

CONFIG += c++11

TARGET = QOSC

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    $$PWD/qoscbundle.cpp \
    $$PWD/qoscinterface.cpp \
    $$PWD/qoscmessage.cpp \
    $$PWD/qoscmethod.cpp \
    $$PWD/qoscpacket.cpp \
    $$PWD/qoschelper.cpp

include($$PWD/../include/include.pri)

OBJECTS_DIR = tmp
MOC_DIR = tmp
DESTDIR = lib
