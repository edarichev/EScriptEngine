CONFIG -= qt

TEMPLATE = lib
DEFINES += ESCRIPT_LIBRARY

CONFIG += c++17
CONFIG += PRECOMPILE_HEADER

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    src/arrayobject.cpp \
    src/assembler.cpp \
    src/automationobject.cpp \
    src/block.cpp \
    src/console.cpp \
    src/datetimeobject.cpp \
    src/engineerrors.cpp \
    src/escript.cpp \
    src/functionobject.cpp \
    src/icodeemitter.cpp \
    src/keyword.cpp \
    src/lexer.cpp \
    src/machine.cpp \
    src/mathobject.cpp \
    src/numberobject.cpp \
    src/objectrecord.cpp \
    src/operand.cpp \
    src/parser.cpp \
    src/processor.cpp \
    src/pvalue.cpp \
    src/stackvalue.cpp \
    src/stdafx.cpp \
    src/storage.cpp \
    src/stringcontainer.cpp \
    src/stringobject.cpp \
    src/symbol.cpp \
    src/symboltable.cpp \
    src/tcode.cpp \
    src/token.cpp \
    src/translator.cpp \
    src/types.cpp \
    src/unit.cpp

HEADERS += \
    EScript_global.h \
    arrayobject.h \
    assembler.h \
    automationobject.h \
    block.h \
    console.h \
    datetimeobject.h \
    engineerrors.h \
    escript.h \
    escriptengine.h \
    functionobject.h \
    icodeemitter.h \
    keyword.h \
    lexer.h \
    machine.h \
    mathobject.h \
    numberobject.h \
    objectrecord.h \
    opcode.h \
    operand.h \
    parser.h \
    processor.h \
    pvalue.h \
    stackvalue.h \
    src/stdafx.h \
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

PRECOMPILED_HEADER = src/stdafx.h

# Default rules for deployment.
unix {
    target.path = /usr/lib
}
!isEmpty(target.path): INSTALLS += target

DISTFILES += \
    ../LICENSE \
    ../README.md \
    expressions.g
