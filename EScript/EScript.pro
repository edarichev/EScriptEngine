CONFIG -= qt

TEMPLATE = lib
DEFINES += ESCRIPT_LIBRARY

CONFIG += c++17
CONFIG += PRECOMPILE_HEADER

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    array.cpp \
    assembler.cpp \
    automationobject.cpp \
    block.cpp \
    escript.cpp \
    function.cpp \
    icodeemitter.cpp \
    keyword.cpp \
    lexer.cpp \
    machine.cpp \
    objectrecord.cpp \
    operand.cpp \
    parser.cpp \
    processor.cpp \
    pvalue.cpp \
    stdafx.cpp \
    storage.cpp \
    stringcontainer.cpp \
    stringobject.cpp \
    symbol.cpp \
    symboltable.cpp \
    tcode.cpp \
    token.cpp \
    translator.cpp \
    unit.cpp

HEADERS += \
    EScript_global.h \
    array.h \
    assembler.h \
    automationobject.h \
    block.h \
    escript.h \
    escriptengine.h \
    function.h \
    icodeemitter.h \
    keyword.h \
    lexer.h \
    machine.h \
    objectrecord.h \
    opcode.h \
    operand.h \
    parser.h \
    processor.h \
    pvalue.h \
    stackvalue.h \
    stdafx.h \
    storage.h \
    stringcontainer.h \
    stringobject.h \
    symbol.h \
    symboltable.h \
    tcode.h \
    token.h \
    translator.h \
    types.h \
    unit.h

PRECOMPILED_HEADER = stdafx.h

# Default rules for deployment.
unix {
    target.path = /usr/lib
}
!isEmpty(target.path): INSTALLS += target

DISTFILES += \
    expressions.g
