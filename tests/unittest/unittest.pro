TEMPLATE = app
CONFIG += console c++17
CONFIG -= app_bundle
CONFIG -= qt
CONFIG += PRECOMPILE_HEADER

SOURCES += \
        algo_test.cpp \
        array_test.cpp \
        assembler_test.cpp \
        assign_test.cpp \
        automation_test.cpp \
        bit_test.cpp \
        boolean_test.cpp \
        callscriptfunction_test.cpp \
        codegen_test.cpp \
        codeunit_test.cpp \
        compare.cpp \
        console_test.cpp \
        conversions_test.cpp \
        dotoperation_test.cpp \
        dowhilestatement_test.cpp \
        forreadme_test.cpp \
        forstatement_test.cpp \
        functiondeclstatement_test.cpp \
        functions_test.cpp \
        ifelsestatement_test.cpp \
        incdec_test.cpp \
        lexer_test.cpp \
        logical_test.cpp \
        main.cpp \
        math_test.cpp \
        null_test.cpp \
        number_test.cpp \
        opcode_test.cpp \
        pvalue_test.cpp \
        shift_test.cpp \
        simpleexpression_test.cpp \
        stdafx.cpp \
        string_test.cpp \
        switchcase_test.cpp \
        variabledecl_test.cpp \
        whilestatement_test.cpp

HEADERS += \
    algo_test.h \
    array_test.h \
    assembler_test.h \
    assign_test.h \
    automation_test.h \
    bit_test.h \
    boolean_test.h \
    callscriptfunction_test.h \
    codegen_test.h \
    codeunit_test.h \
    compare.h \
    console_test.h \
    conversions_test.h \
    dotoperation_test.h \
    dowhilestatement_test.h \
    forreadme_test.h \
    forstatement_test.h \
    functiondeclstatement_test.h \
    functions_test.h \
    ifelsestatement_test.h \
    incdec_test.h \
    lexer_test.h \
    logical_test.h \
    math_test.h \
    null_test.h \
    number_test.h \
    opcode_test.h \
    pvalue_test.h \
    shift_test.h \
    simpleexpression_test.h \
    stdafx.h \
    string_test.h \
    switchcase_test.h \
    variabledecl_test.h \
    whilestatement_test.h

PRECOMPILED_HEADER = stdafx.h

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../../EScript/release/ -lEScript
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../../EScript/debug/ -lEScript
else:unix: LIBS += -L$$OUT_PWD/../../EScript/ -lEScript

INCLUDEPATH += $$PWD/../../EScript
DEPENDPATH += $$PWD/../../EScript

