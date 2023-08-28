#include "stdafx.h"
#include "nco_test.h"



void NCO_Test::run()
{
    initTestCase();
    test_nco();
    cleanupTestCase();
}

void NCO_Test::initTestCase()
{

}

void NCO_Test::cleanupTestCase()
{

}

void NCO_Test::test_nco()
{
    const std::u32string code1 = UR"(
var x; // == null
a = x ?? 123;
b = x ?? null;
)";
    EScript engine;
    engine.eval(code1);
    auto mainTable = engine.unit()->block()->symbolTable();
    auto a = mainTable->find(U"a");
    auto record = engine.getObjectRecord(a);
    assert(record->type == SymbolType::Integer);
    assert(Compare::equals_int64(123, record->data));

    auto b = mainTable->find(U"b");
    record = engine.getObjectRecord(b);
    assert(record->type == SymbolType::Null);
}
