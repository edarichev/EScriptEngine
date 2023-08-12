#include "stdafx.h"
#include "array_test.h"


void Array_Test::run()
{
    initTestCase();
    test_arrayAccess();
    cleanupTestCase();
}

void Array_Test::initTestCase()
{

}

void Array_Test::cleanupTestCase()
{

}

void Array_Test::test_arrayAccess()
{
    const u32string code1 = U"a = [3,6,9,]; y = a[0];";
    EScript engine;
    engine.eval(code1);
    auto mainTable = engine.unit()->block()->symbolTable();
    auto y = mainTable->find(U"y");
    assert(y != nullptr);
    auto record = engine.getObjectRecord(y);
    assert(record != nullptr);
    assert(record->type == SymbolType::Integer);
    assert(Compare::equals_int64(3, record->data));

}
