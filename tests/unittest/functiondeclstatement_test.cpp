#include "stdafx.h"
#include "functiondeclstatement_test.h"



void FunctionDeclStatement_Test::run()
{
    initTestCase();
    test_functionDeclOnly();
    test_functionCall();
    cleanupTestCase();
}

void FunctionDeclStatement_Test::initTestCase()
{

}

void FunctionDeclStatement_Test::cleanupTestCase()
{

}

void FunctionDeclStatement_Test::test_functionDeclOnly()
{
    const u32string code1 = U"y = 0; function myFunc(x) { y = x*x; }; myFunc(12);";
    EScript engine;
    engine.eval(code1);
    auto mainTable = engine.unit()->block()->symbolTable();
    auto y = mainTable->find(U"y");
    assert(y != nullptr);
    auto record = engine.getObjectRecord(y);
    assert(record != nullptr);
    assert(record->type == SymbolType::Integer);
    assert(Compare::equals_int64(12*12, record->data));

    const u32string code2 = U"y = 0; function myFunc(x) { y = x*x; return y; }; z = myFunc(12);";
    engine.setShowDisassembleListing(false);
    engine.setShowTCode(false);
    engine.eval(code2);
    mainTable = engine.unit()->block()->symbolTable();
    auto z = mainTable->find(U"z");
    assert(z != nullptr);
    record = engine.getObjectRecord(z);
    assert(record != nullptr);
    assert(record->type == SymbolType::Integer);
    assert(Compare::equals_int64(12*12, record->data));

    const u32string code3 = U"y = 5; function myFunc(x) { y = x*x; return y; }; z = myFunc(y);";
    engine.eval(code3);
    mainTable = engine.unit()->block()->symbolTable();
    z = mainTable->find(U"z");
    assert(z != nullptr);
    record = engine.getObjectRecord(z);
    assert(record != nullptr);
    assert(record->type == SymbolType::Integer);
    assert(Compare::equals_int64(25, record->data));
}

void FunctionDeclStatement_Test::test_functionCall()
{
    const u32string code1 =
U"y = 0;"
"function fn1(x) { return x + 2; } function myFunc(x) { y = x*x; return y; }; z=myFunc(fn1(12));";
    EScript engine;
    engine.eval(code1);
    auto mainTable = engine.unit()->block()->symbolTable();
    auto z = mainTable->find(U"z");
    assert(z != nullptr);
    auto record = engine.getObjectRecord(z);
    assert(record != nullptr);
    assert(record->type == SymbolType::Integer);
    assert(Compare::equals_int64(196, record->data));
}
