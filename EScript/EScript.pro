CONFIG -= qt

TEMPLATE = lib
DEFINES += ESCRIPT_LIBRARY

CONFIG += c++17
CONFIG += PRECOMPILE_HEADER

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    block.cpp \
    escript.cpp \
    icodeemitter.cpp \
    keyword.cpp \
    lexer.cpp \
    opcode.cpp \
    parser.cpp \
    stdafx.cpp \
    symbol.cpp \
    symboltable.cpp \
    token.cpp \
    unit.cpp

HEADERS += \
    EScript_global.h \
    block.h \
    escript.h \
    icodeemitter.h \
    keyword.h \
    lexer.h \
    opcode.h \
    parser.h \
    stdafx.h \
    symbol.h \
    symboltable.h \
    token.h \
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
