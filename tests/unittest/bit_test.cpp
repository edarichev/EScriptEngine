#include "stdafx.h"
#include "bit_test.h"



void Bit_Test::run()
{
    initTestCase();
    test_bitAnd();
    test_bitOr();
    test_bitXor();
    cleanupTestCase();
}

void Bit_Test::initTestCase()
{

}

void Bit_Test::cleanupTestCase()
{

}

void Bit_Test::test_bitAnd()
{
    const u32string code1 = U"a = 23563; b = 14085; c = a & b;";
    EScript engine;
    engine.setShowDisassembleListing(false);
    engine.setShowTCode(false);
    engine.eval(code1);
    auto mainTable = engine.unit()->block()->symbolTable();
    auto c = mainTable->find(U"c");
    auto record = engine.getObjectRecord(c);
    assert(record->type == SymbolType::Integer);
    assert(Compare::equals_int64(23563 & 14085, record->data));
}

void Bit_Test::test_bitOr()
{
    const u32string code1 = U"a = 234563; b = 144085; c = a | b;";
    EScript engine;
    engine.setShowDisassembleListing(false);
    engine.setShowTCode(false);
    engine.eval(code1);
    auto mainTable = engine.unit()->block()->symbolTable();
    auto c = mainTable->find(U"c");
    auto record = engine.getObjectRecord(c);
    assert(record->type == SymbolType::Integer);
    assert(Compare::equals_int64(234563 | 144085, record->data));
}

void Bit_Test::test_bitXor()
{
    const u32string code1 = U"a = 2363; b = 1411085; c = a ^ b;";
    EScript engine;
    engine.setShowDisassembleListing(false);
    engine.setShowTCode(false);
    engine.eval(code1);
    auto mainTable = engine.unit()->block()->symbolTable();
    auto c = mainTable->find(U"c");
    auto record = engine.getObjectRecord(c);
    assert(record->type == SymbolType::Integer);
    assert(Compare::equals_int64(2363 ^ 1411085, record->data));
}

