#include "stdafx.h"
#include "forstatement_test.h"

#include "compare.h"

using namespace escript;
using namespace std;

void ForStatement_Test::run()
{
    initTestCase();
    test_for();
    cleanupTestCase();
}

void ForStatement_Test::initTestCase()
{

}

void ForStatement_Test::cleanupTestCase()
{

}

void ForStatement_Test::test_for()
{
    const u32string code1 = U"for (i = 0; i < 10; i = i + 1) {i = i + 1;}";
    EScript engine;
    engine.eval(code1);
    auto mainTable = engine.unit()->block()->symbolTable();
    auto i = mainTable->find(U"i");
    auto record = engine.getObjectRecord(i);
    assert(record->type == SymbolType::Integer);
    assert(Compare::equals_int64(10, record->data));

    const u32string code2 = U"x = 1; for (i = 1; i < 10; i = i + 1) {x = x * i;}";
    engine.eval(code2);
    mainTable = engine.unit()->block()->symbolTable();
    auto x = mainTable->find(U"x");
    record = engine.getObjectRecord(x);
    assert(record->type == SymbolType::Integer);
    assert(Compare::equals_int64(1*2*3*4*5*6*7*8*9, record->data));

    const u32string code3 = U"for (i = 0, j = 0; i < 10; i = i + 1, j = j + 1);";
    engine.eval(code3);
    mainTable = engine.unit()->block()->symbolTable();
    auto j = mainTable->find(U"j");
    record = engine.getObjectRecord(j);
    assert(record->type == SymbolType::Integer);
    assert(Compare::equals_int64(10, record->data));

    const u32string code4 = U"i = 0; j = 0; for (; i < 10; i = i + 1, j = j + 1);";
    engine.eval(code4);
    mainTable = engine.unit()->block()->symbolTable();
    j = mainTable->find(U"j");
    record = engine.getObjectRecord(j);
    assert(record->type == SymbolType::Integer);
    assert(Compare::equals_int64(10, record->data));

    const u32string code5 = U"i = 0; j = 0; for (; i < 10;) { i = i + 1; j = j + 1; }";
    engine.eval(code5);
    mainTable = engine.unit()->block()->symbolTable();
    j = mainTable->find(U"j");
    record = engine.getObjectRecord(j);
    assert(record->type == SymbolType::Integer);
    assert(Compare::equals_int64(10, record->data));

    const u32string code6 = U"i = 0; for (; i < 10;) {if (i == 5) break; i = i + 1; }";
    engine.eval(code6);
    mainTable = engine.unit()->block()->symbolTable();
    i = mainTable->find(U"i");
    record = engine.getObjectRecord(i);
    assert(record->type == SymbolType::Integer);
    assert(Compare::equals_int64(5, record->data));

    const u32string code7 = U"i = 0; for (;;) {if (i == 5) break; i = i + 1; }";
    engine.eval(code7);
    mainTable = engine.unit()->block()->symbolTable();
    i = mainTable->find(U"i");
    record = engine.getObjectRecord(i);
    assert(record->type == SymbolType::Integer);
    assert(Compare::equals_int64(5, record->data));

    const u32string code8 = U"i = 0; for (;i < 10;) {if (i == 5) { i = 12345; continue; } i = i + 1; }";
    engine.eval(code8);
    mainTable = engine.unit()->block()->symbolTable();
    i = mainTable->find(U"i");
    record = engine.getObjectRecord(i);
    assert(record->type == SymbolType::Integer);
    assert(Compare::equals_int64(12345, record->data));

    const u32string code9 = U"j = 0; i = 0; for (;i < 10; i = i + 1) {if (i == 6) { j = 8; continue; } i = i + 1; }";
    engine.eval(code9);
    mainTable = engine.unit()->block()->symbolTable();
    j = mainTable->find(U"j");
    record = engine.getObjectRecord(j);
    assert(record->type == SymbolType::Integer);
    assert(Compare::equals_int64(8, record->data));
}
