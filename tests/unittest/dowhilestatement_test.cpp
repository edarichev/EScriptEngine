#include "stdafx.h"
#include "dowhilestatement_test.h"


void DoWhileStatement_Test::run()
{
    initTestCase();
    test_dowhile();
    cleanupTestCase();
}



void DoWhileStatement_Test::initTestCase()
{

}



void DoWhileStatement_Test::cleanupTestCase()
{

}



void DoWhileStatement_Test::test_dowhile()
{
    const u32string code1 = U"i = 0; do { i = i + 1; } while (i < 10);";
    EScript engine;
    engine.eval(code1);
    auto mainTable = engine.unit()->block()->symbolTable();
    auto i = mainTable->find(U"i");
    auto record = engine.getObjectRecord(i);
    assert(record->type == SymbolType::Integer);
    assert(Compare::equals_int64(10, record->data));

    const u32string code2 = U"i = 0; do { i = i + 1; if (i > 5) break; } while (true);";
    engine.eval(code2);
    mainTable = engine.unit()->block()->symbolTable();
    i = mainTable->find(U"i");
    record = engine.getObjectRecord(i);
    assert(record->type == SymbolType::Integer);
    assert(Compare::equals_int64(6, record->data));
}


