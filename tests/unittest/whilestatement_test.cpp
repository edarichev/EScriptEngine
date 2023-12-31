#include "stdafx.h"
#include "whilestatement_test.h"
#include "compare.h"

using namespace escript;
using namespace std;


void WhileStatement_Test::run()
{
    initTestCase();
    test_while();
    cleanupTestCase();
}

void WhileStatement_Test::initTestCase()
{

}

void WhileStatement_Test::cleanupTestCase()
{

}

void WhileStatement_Test::test_while()
{
    const u32string code1 = U"i = 0; while (i < 10) {i = i + 1;}";
    EScript engine;
    engine.eval(code1);
    auto mainTable = engine.unit()->block()->symbolTable();
    auto i = mainTable->find(U"i");
    auto record = engine.getObjectRecord(i);
    assert(record->type == SymbolType::Integer);
    assert(Compare::equals_int64(10, record->data));

    const u32string code2 = U"i = 0; while (i < 10) { if (i > 3) break; i = i + 1; }";
    engine.eval(code2);
    mainTable = engine.unit()->block()->symbolTable();
    i = mainTable->find(U"i");
    record = engine.getObjectRecord(i);
    assert(record->type == SymbolType::Integer);
    assert(Compare::equals_int64(4, record->data));

    const u32string code3 = U"i = 0; while (i < 10) break;";
    engine.eval(code3);
    mainTable = engine.unit()->block()->symbolTable();
    i = mainTable->find(U"i");
    record = engine.getObjectRecord(i);
    assert(record->type == SymbolType::Integer);
    assert(Compare::equals_int64(0, record->data));

    const u32string code4 = U"i = 0; while (i < 10) { i = i + 1; if (i < 3) continue; break; }";
    engine.eval(code4);
    mainTable = engine.unit()->block()->symbolTable();
    i = mainTable->find(U"i");
    record = engine.getObjectRecord(i);
    assert(record->type == SymbolType::Integer);
    assert(Compare::equals_int64(3, record->data));
}
