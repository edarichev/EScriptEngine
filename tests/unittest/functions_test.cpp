#include "stdafx.h"
#include "functions_test.h"


void Functions_Test::run()
{
    initTestCase();
    test_byReference();
    test_recursionFactorial();
    cleanupTestCase();
}

void Functions_Test::initTestCase()
{

}

void Functions_Test::cleanupTestCase()
{

}

void Functions_Test::test_byReference()
{
    const u32string code1 = U"a = [6,5,4]; function fnref(x) { x[0] = 8; } fnref(a); y = a[0];";
    EScript engine;
    engine.setShowDisassembleListing(false);
    engine.setShowTCode(false);
    engine.eval(code1);
    auto mainTable = engine.unit()->block()->symbolTable();
    auto y = mainTable->find(U"y");
    auto record = engine.getObjectRecord(y);
    assert(record->type == SymbolType::Integer);
    assert(Compare::equals_int64(8, record->data));
}

void Functions_Test::test_recursionFactorial()
{
    const u32string code1 = U"function factorial(i) { if (i == 0) return 1; return i * factorial(i - 1); } y = factorial(10);";
    EScript engine;
    engine.setShowDisassembleListing(false);
    engine.setShowTCode(false);
    //stringstream ss;
    //engine.setOutStream(ss);
    engine.eval(code1);
    auto mainTable = engine.unit()->block()->symbolTable();
    auto y = mainTable->find(U"y");
    auto record = engine.getObjectRecord(y);
    assert(record->type == SymbolType::Integer);
    assert(Compare::equals_int64(3628800, record->data));
}
