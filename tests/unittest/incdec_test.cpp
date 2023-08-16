#include "stdafx.h"
#include "incdec_test.h"


void IncDec_Test::run()
{
    initTestCase();
    test_prefixInc();
    cleanupTestCase();
}

void IncDec_Test::initTestCase()
{

}

void IncDec_Test::cleanupTestCase()
{

}

void IncDec_Test::test_prefixInc()
{
    const u32string code1 = U"a = 3; b = ++a;";
    EScript engine;
    engine.eval(code1);
    auto mainTable = engine.unit()->block()->symbolTable();
    auto a = mainTable->find(U"a");
    auto record = engine.getObjectRecord(a);
    assert(record->type == SymbolType::Integer);
    assert(Compare::equals_int64(4, record->data));

    auto b = mainTable->find(U"b");
    record = engine.getObjectRecord(b);
    assert(record->type == SymbolType::Integer);
    assert(Compare::equals_int64(4, record->data));
}
