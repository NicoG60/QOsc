INCLUDEPATH += $$PWD/../include
macx: LIBS += -L$$OUT_PWD/../../src/lib -llibQOsc.a
win32: LIBS += -L$$OUT_PWD/../../src/lib -llibQOsc
