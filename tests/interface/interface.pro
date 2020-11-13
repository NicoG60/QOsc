QT += testlib network

CONFIG += qt console warn_on depend_includepath testcase
CONFIG -= app_bundle

TEMPLATE = app

SOURCES +=  tst_interface.cpp

include(../libs.pri)
