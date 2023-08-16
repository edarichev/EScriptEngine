#include "stdafx.h"
#include "logical_test.h"

void Logical_Test::run()
{
    initTestCase();
    test_logAnd();
    test_logOr();
    cleanupTestCase();
}

void Logical_Test::initTestCase()
{

}

void Logical_Test::cleanupTestCase()
{

}

void Logical_Test::test_logAnd()
{
    const u32string code1 = U"a = true; b = true; c = a && b;";
    EScript engine;
    engine.setShowDisassembleListing(false);
    engine.setShowTCode(false);
    engine.eval(code1);
    auto mainTable = engine.unit()->block()->symbolTable();
    auto c = mainTable->find(U"c");
    auto record = engine.getObjectRecord(c);
    assert(record->type == SymbolType::Boolean);
    assert(Compare::equals_int64(true && true, record->data));

    const u32string code2 = U"a = true; b = false; c = a && b;";
    engine.eval(code2);
    c = mainTable->find(U"c");
    record = engine.getObjectRecord(c);
    assert(record->type == SymbolType::Boolean);
    assert(Compare::equals_int64(true && false, record->data));

    const u32string code3 = U"a = false; b = true; c = a && b;";
    engine.eval(code3);
    c = mainTable->find(U"c");
    record = engine.getObjectRecord(c);
    assert(record->type == SymbolType::Boolean);
    assert(Compare::equals_int64(false && true, record->data));

    const u32string code4 = U"a = false; b = false; c = a && b;";
    engine.eval(code4);
    c = mainTable->find(U"c");
    record = engine.getObjectRecord(c);
    assert(record->type == SymbolType::Boolean);
    assert(Compare::equals_int64(false && false, record->data));
}

void Logical_Test::test_logOr()
{
    const u32string code1 = U"a = true; b = true; c = a || b;";
    EScript engine;
    engine.setShowDisassembleListing(false);
    engine.setShowTCode(false);
    engine.eval(code1);
    auto mainTable = engine.unit()->block()->symbolTable();
    auto c = mainTable->find(U"c");
    auto record = engine.getObjectRecord(c);
    assert(record->type == SymbolType::Boolean);
    assert(Compare::equals_int64(true || true, record->data));

    const u32string code2 = U"a = true; b = false; c = a || b;";
    engine.eval(code2);
    c = mainTable->find(U"c");
    record = engine.getObjectRecord(c);
    assert(record->type == SymbolType::Boolean);
    assert(Compare::equals_int64(true || false, record->data));

    const u32string code3 = U"a = false; b = true; c = a || b;";
    engine.eval(code3);
    c = mainTable->find(U"c");
    record = engine.getObjectRecord(c);
    assert(record->type == SymbolType::Boolean);
    assert(Compare::equals_int64(false || true, record->data));

    const u32string code4 = U"a = false; b = false; c = a || b;";
    engine.eval(code4);
    c = mainTable->find(U"c");
    record = engine.getObjectRecord(c);
    assert(record->type == SymbolType::Boolean);
    assert(Compare::equals_int64(false || false, record->data));
}
