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
    const u32string code1 = U"a = [3,6,9,]; y = a[0]; z = a[1]; t = a[2];";
    EScript engine;
    engine.eval(code1);
    auto mainTable = engine.unit()->block()->symbolTable();
    auto a = mainTable->find(U"a");
    assert(a != nullptr);
    auto record = engine.getObjectRecord(a);
    assert(record != nullptr);
    assert(record->type == SymbolType::Array);
    Array *arr = (Array*)record->data;
    assert(arr->get(0).intValue == 3);
    assert(arr->get(1).intValue == 6);
    assert(arr->get(2).intValue == 9);

    auto y = mainTable->find(U"y");
    assert(y != nullptr);
    record = engine.getObjectRecord(y);
    assert(record != nullptr);
    assert(record->type == SymbolType::Integer);
    assert(Compare::equals_int64(3, record->data));

    auto z = mainTable->find(U"z");
    assert(z != nullptr);
    record = engine.getObjectRecord(z);
    assert(record != nullptr);
    assert(record->type == SymbolType::Integer);
    assert(Compare::equals_int64(6, record->data));

    auto t = mainTable->find(U"t");
    assert(t != nullptr);
    record = engine.getObjectRecord(t);
    assert(record != nullptr);
    assert(record->type == SymbolType::Integer);
    assert(Compare::equals_int64(9, record->data));
}
