#include "stdafx.h"
#include "ifelsestatement_test.h"
#include "compare.h"

using namespace escript;
using namespace std;

void IfElseStatement_Test::run()
{
    initTestCase();
    test_ifOnly();
    cleanupTestCase();
}

void IfElseStatement_Test::initTestCase()
{

}

void IfElseStatement_Test::cleanupTestCase()
{

}

void IfElseStatement_Test::test_ifOnly()
{
    const u32string code1 = U"i = 0; if (true) i = 6;";
    EScript engine;
    engine.eval(code1);
    auto mainTable = engine.unit()->block()->symbolTable();
    auto i = mainTable->find(U"i");
    auto record = engine.getObjectRecord(i);
    assert(record->type == SymbolType::Integer);
    assert(Compare::equals_int64(6, record->data));

    const u32string code2 = U"i = 0; if (true) i = 6; else i = 5;";
    engine.eval(code2);
    i = mainTable->find(U"i");
    record = engine.getObjectRecord(i);
    assert(record->type == SymbolType::Integer);
    assert(Compare::equals_int64(6, record->data));

    const u32string code3 = U"if (false) i = 6; else i = 5;";
    engine.eval(code3);
    i = mainTable->find(U"i");
    record = engine.getObjectRecord(i);
    assert(record->type == SymbolType::Integer);
    assert(Compare::equals_int64(5, record->data));

    const u32string code4 = U"if (false) i = 6; else if (false) i = 5; else i = 7;";
    engine.eval(code4);
    i = mainTable->find(U"i");
    record = engine.getObjectRecord(i);
    assert(record->type == SymbolType::Integer);
    assert(Compare::equals_int64(7, record->data));
}