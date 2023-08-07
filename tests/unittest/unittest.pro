TEMPLATE = app
CONFIG += console c++17
CONFIG -= app_bundle
CONFIG -= qt
CONFIG += PRECOMPILE_HEADER

SOURCES += \
        assembler_test.cpp \
        automation_test.cpp \
        boolean_test.cpp \
        codegen_test.cpp \
        codeunit_test.cpp \
        compare.cpp \
        ifelsestatement_test.cpp \
        lexer_test.cpp \
        main.cpp \
        opcode_test.cpp \
        simpleexpression_test.cpp \
        stdafx.cpp

HEADERS += \
    assembler_test.h \
    automation_test.h \
    boolean_test.h \
    codegen_test.h \
    codeunit_test.h \
    compare.h \
    ifelsestatement_test.h \
    lexer_test.h \
    opcode_test.h \
    simpleexpression_test.h \
    stdafx.h

PRECOMPILED_HEADER = stdafx.h

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../../EScript/release/ -lEScript
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../../EScript/debug/ -lEScript
else:unix: LIBS += -L$$OUT_PWD/../../EScript/ -lEScript

INCLUDEPATH += $$PWD/../../EScript
DEPENDPATH += $$PWD/../../EScript

