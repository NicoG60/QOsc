INCLUDEPATH += $$PWD/../include
macx: LIBS += -L$$OUT_PWD/../../src/lib -lQOsc
win32: LIBS += -L$$OUT_PWD/../../src/lib -lQOsc
