#include "stdafx.h"
#include "assign_test.h"


void Assign_Test::run()
{
    initTestCase();
    test_plusAssign();
    test_minusAssign();
    test_multiplyAssign();
    test_divideAssign();
    test_BitAndAssign();
    test_BitOrAssign();
    test_BitXorAssign();
    test_LShiftAssign();
    test_LRhiftAssign();
    test_RShiftZeroAssign();
    test_modAssign();
    cleanupTestCase();
}

void Assign_Test::initTestCase()
{

}

void Assign_Test::cleanupTestCase()
{

}

void Assign_Test::test_plusAssign()
{
    const u32string code1 = U"a = 2363; b = 36; a += b + 345;";
    EScript engine;
    engine.setShowDisassembleListing(false);
    engine.setShowTCode(false);
    engine.eval(code1);
    auto mainTable = engine.unit()->block()->symbolTable();
    auto a = mainTable->find(U"a");
    auto record = engine.getObjectRecord(a);
    assert(record->type == SymbolType::Integer);
    assert(Compare::equals_int64(2363+345+36, record->data));
}

void Assign_Test::test_minusAssign()
{
    const u32string code1 = U"a = 2363; b = 36; a -= b + 345;";
    EScript engine;
    engine.setShowDisassembleListing(false);
    engine.setShowTCode(false);
    engine.eval(code1);
    auto mainTable = engine.unit()->block()->symbolTable();
    auto a = mainTable->find(U"a");
    auto record = engine.getObjectRecord(a);
    assert(record->type == SymbolType::Integer);
    assert(Compare::equals_int64(2363-(345+36), record->data));
}

void Assign_Test::test_divideAssign()
{
    const u32string code1 = U"a = 2363; b = 36; a /= b + 345;";
    EScript engine;
    engine.setShowDisassembleListing(false);
    engine.setShowTCode(false);
    engine.eval(code1);
    auto mainTable = engine.unit()->block()->symbolTable();
    auto a = mainTable->find(U"a");
    auto record = engine.getObjectRecord(a);
    assert(record->type == SymbolType::Integer);
    assert(Compare::equals_int64(2363/(345+36), record->data));
}

void Assign_Test::test_multiplyAssign()
{
    const u32string code1 = U"a = 2363; b = 36; a *= b + 345;";
    EScript engine;
    engine.setShowDisassembleListing(false);
    engine.setShowTCode(false);
    engine.eval(code1);
    auto mainTable = engine.unit()->block()->symbolTable();
    auto a = mainTable->find(U"a");
    auto record = engine.getObjectRecord(a);
    assert(record->type == SymbolType::Integer);
    assert(Compare::equals_int64(2363*(345+36), record->data));
}

void Assign_Test::test_BitAndAssign()
{
    const u32string code1 = U"a = 2363; b = 36; a &= b + 345;";
    EScript engine;
    engine.setShowDisassembleListing(false);
    engine.setShowTCode(false);
    engine.eval(code1);
    auto mainTable = engine.unit()->block()->symbolTable();
    auto a = mainTable->find(U"a");
    auto record = engine.getObjectRecord(a);
    assert(record->type == SymbolType::Integer);
    assert(Compare::equals_int64(2363&(345+36), record->data));
}

void Assign_Test::test_BitOrAssign()
{
    const u32string code1 = U"a = 2363; b = 36; a |= b + 345;";
    EScript engine;
    engine.setShowDisassembleListing(false);
    engine.setShowTCode(false);
    engine.eval(code1);
    auto mainTable = engine.unit()->block()->symbolTable();
    auto a = mainTable->find(U"a");
    auto record = engine.getObjectRecord(a);
    assert(record->type == SymbolType::Integer);
    assert(Compare::equals_int64(2363|(345+36), record->data));
}

void Assign_Test::test_BitXorAssign()
{
    const u32string code1 = U"a = 2363; b = 36; a ^= b + 345;";
    EScript engine;
    engine.setShowDisassembleListing(false);
    engine.setShowTCode(false);
    engine.eval(code1);
    auto mainTable = engine.unit()->block()->symbolTable();
    auto a = mainTable->find(U"a");
    auto record = engine.getObjectRecord(a);
    assert(record->type == SymbolType::Integer);
    assert(Compare::equals_int64(2363^(345+36), record->data));
}

void Assign_Test::test_LShiftAssign()
{
    const u32string code1 = U"a = 2363; b = 3; a <<= b;";
    EScript engine;
    engine.setShowDisassembleListing(false);
    engine.setShowTCode(false);
    engine.eval(code1);
    auto mainTable = engine.unit()->block()->symbolTable();
    auto a = mainTable->find(U"a");
    auto record = engine.getObjectRecord(a);
    assert(record->type == SymbolType::Integer);
    assert(Compare::equals_int64(2363<<3, record->data));
}

void Assign_Test::test_LRhiftAssign()
{
    const u32string code1 = U"a = 2363; b = 3; a >>= b;";
    EScript engine;
    engine.setShowDisassembleListing(false);
    engine.setShowTCode(false);
    engine.eval(code1);
    auto mainTable = engine.unit()->block()->symbolTable();
    auto a = mainTable->find(U"a");
    auto record = engine.getObjectRecord(a);
    assert(record->type == SymbolType::Integer);
    assert(Compare::equals_int64(2363>>3, record->data));
}

void Assign_Test::test_RShiftZeroAssign()
{
    const u32string code1 = U"a = -1; b = 3; a >>>= b;";
    EScript engine;
    engine.setShowDisassembleListing(false);
    engine.setShowTCode(false);
    engine.eval(code1);
    auto mainTable = engine.unit()->block()->symbolTable();
    auto a = mainTable->find(U"a");
    auto record = engine.getObjectRecord(a);
    assert(record->type == SymbolType::Integer);
    assert(Compare::equals_int64(((uint64_t)-1)>>3, record->data));
}

void Assign_Test::test_modAssign()
{
    const u32string code1 = U"a = 157; b = 7; a %= b;";
    EScript engine;
    engine.setShowDisassembleListing(false);
    engine.setShowTCode(false);
    engine.eval(code1);
    auto mainTable = engine.unit()->block()->symbolTable();
    auto a = mainTable->find(U"a");
    auto record = engine.getObjectRecord(a);
    assert(record->type == SymbolType::Integer);
    assert(Compare::equals_int64(157%7, record->data));
}
