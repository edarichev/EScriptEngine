TEMPLATE = app
CONFIG += console c++17
CONFIG -= app_bundle
CONFIG -= qt
CONFIG += PRECOMPILE_HEADER

SOURCES += \
        lexer_test.cpp \
        main.cpp \
        stdafx.cpp

HEADERS += \
    lexer_test.h \
    stdafx.h

PRECOMPILED_HEADER = stdafx.h

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../../EScript/release/ -lEScript
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../../EScript/debug/ -lEScript
else:unix: LIBS += -L$$OUT_PWD/../../EScript/ -lEScript

INCLUDEPATH += $$PWD/../../EScript
DEPENDPATH += $$PWD/../../EScript

