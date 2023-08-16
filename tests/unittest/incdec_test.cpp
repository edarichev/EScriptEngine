#include "stdafx.h"
#include "incdec_test.h"


void IncDec_Test::run()
{
    initTestCase();
    test_prefixInc();
    test_postfixInc();
    test_prefixDec();
    test_postfixDec();
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
    engine.setShowDisassembleListing(false);
    engine.setShowTCode(false);
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

    const u32string code2 = U"b = ++6;";
    engine.setShowDisassembleListing(false);
    engine.setShowTCode(false);
    engine.eval(code2);

    b = mainTable->find(U"b");
    record = engine.getObjectRecord(b);
    assert(record->type == SymbolType::Integer);
    assert(Compare::equals_int64(7, record->data));
}

void IncDec_Test::test_postfixInc()
{
    const u32string code1 = U"a = 3; b = a++;";
    EScript engine;
    engine.setShowDisassembleListing(true);
    engine.setShowTCode(true);
    engine.eval(code1);
    auto mainTable = engine.unit()->block()->symbolTable();
    auto a = mainTable->find(U"a");
    auto record = engine.getObjectRecord(a);
    assert(record->type == SymbolType::Integer);
    assert(Compare::equals_int64(4, record->data));
    auto b = mainTable->find(U"b");
    record = engine.getObjectRecord(b);
    assert(record->type == SymbolType::Integer);
    assert(Compare::equals_int64(3, record->data));

    const u32string code2 = U"b = 6++;";
    engine.setShowDisassembleListing(false);
    engine.setShowTCode(false);
    engine.eval(code2);

    b = mainTable->find(U"b");
    record = engine.getObjectRecord(b);
    assert(record->type == SymbolType::Integer);
    assert(Compare::equals_int64(6, record->data));
}

void IncDec_Test::test_prefixDec()
{
    const u32string code1 = U"a = 3; b = --a;";
    EScript engine;
    engine.setShowDisassembleListing(false);
    engine.setShowTCode(false);
    engine.eval(code1);
    auto mainTable = engine.unit()->block()->symbolTable();
    auto a = mainTable->find(U"a");
    auto record = engine.getObjectRecord(a);
    assert(record->type == SymbolType::Integer);
    assert(Compare::equals_int64(2, record->data));

    auto b = mainTable->find(U"b");
    record = engine.getObjectRecord(b);
    assert(record->type == SymbolType::Integer);
    assert(Compare::equals_int64(2, record->data));

    const u32string code2 = U"b = --6;";
    engine.setShowDisassembleListing(false);
    engine.setShowTCode(false);
    engine.eval(code2);

    b = mainTable->find(U"b");
    record = engine.getObjectRecord(b);
    assert(record->type == SymbolType::Integer);
    assert(Compare::equals_int64(5, record->data));
}

void IncDec_Test::test_postfixDec()
{
    const u32string code1 = U"a = 3; b = a--;";
    EScript engine;
    engine.setShowDisassembleListing(true);
    engine.setShowTCode(true);
    engine.eval(code1);
    auto mainTable = engine.unit()->block()->symbolTable();
    auto a = mainTable->find(U"a");
    auto record = engine.getObjectRecord(a);
    assert(record->type == SymbolType::Integer);
    assert(Compare::equals_int64(2, record->data));
    auto b = mainTable->find(U"b");
    record = engine.getObjectRecord(b);
    assert(record->type == SymbolType::Integer);
    assert(Compare::equals_int64(3, record->data));

    const u32string code2 = U"b = 6--;";
    engine.setShowDisassembleListing(false);
    engine.setShowTCode(false);
    engine.eval(code2);

    b = mainTable->find(U"b");
    record = engine.getObjectRecord(b);
    assert(record->type == SymbolType::Integer);
    assert(Compare::equals_int64(6, record->data));
}
