#include "stdafx.h"
#include "ifelsestatement_test.h"
#include "compare.h"

using namespace escript;
using namespace std;

void IfElseStatement_Test::run()
{
    initTestCase();
    test_ifOnly();
    test_ifWithIfElse();
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
    // пустые инструкции
    const u32string code5 = U"i = 9; if (i == 9); else;";
    engine.eval(code5);
    i = mainTable->find(U"i");
    record = engine.getObjectRecord(i);
    assert(record->type == SymbolType::Integer);
    assert(Compare::equals_int64(9, record->data));
}

void IfElseStatement_Test::test_ifWithIfElse()
{
    const u32string code1 = U"i = 0; if (true) i = 6; if (false) i = 7; else i = 11;";
    EScript engine;
    engine.eval(code1);
    auto mainTable = engine.unit()->block()->symbolTable();
    auto i = mainTable->find(U"i");
    auto record = engine.getObjectRecord(i);
    assert(record->type == SymbolType::Integer);
    assert(Compare::equals_int64(11, record->data));
}
