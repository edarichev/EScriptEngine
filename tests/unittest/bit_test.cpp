#include "stdafx.h"
#include "bit_test.h"



void Bit_Test::run()
{
    initTestCase();
    test_bitAnd();
    test_bitOr();
    test_bitXor();
    test_unaryBitNot();
    test_complexBitOr();
    test_complexBitAnd();
    test_complexBitXor();
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

void Bit_Test::test_unaryBitNot()
{
    const u32string code1 = U"a = 2363; b = ~a;";
    EScript engine;
    engine.setShowDisassembleListing(false);
    engine.setShowTCode(false);
    engine.eval(code1);
    auto mainTable = engine.unit()->block()->symbolTable();
    auto b = mainTable->find(U"b");
    auto record = engine.getObjectRecord(b);
    assert(record->type == SymbolType::Integer);
    assert(Compare::equals_int64(~2363, record->data));
}

void Bit_Test::test_complexBitOr()
{
    const u32string code1 = UR"(
a = 2363;
b = 1411085;
c = 5645;
d = a | b | c;
)";
    EScript engine;
    engine.eval(code1);
    auto mainTable = engine.unit()->block()->symbolTable();
    auto d = mainTable->find(U"d");
    auto record = engine.getObjectRecord(d);
    assert(record->type == SymbolType::Integer);
    assert(Compare::equals_int64(2363 | 1411085 | 5645, record->data));
}

void Bit_Test::test_complexBitAnd()
{
    const u32string code1 = UR"(
a = 2363;
b = 1411085;
c = 5645;
d = a & b & c;
)";
    EScript engine;
    engine.eval(code1);
    auto mainTable = engine.unit()->block()->symbolTable();
    auto d = mainTable->find(U"d");
    auto record = engine.getObjectRecord(d);
    assert(record->type == SymbolType::Integer);
    assert(Compare::equals_int64(2363 & 1411085 & 5645, record->data));
}

void Bit_Test::test_complexBitXor()
{
    const u32string code1 = UR"(
a = 2363;
b = 1411085;
c = 5645;
d = a ^ b ^ c;
)";
    EScript engine;
    engine.eval(code1);
    auto mainTable = engine.unit()->block()->symbolTable();
    auto d = mainTable->find(U"d");
    auto record = engine.getObjectRecord(d);
    assert(record->type == SymbolType::Integer);
    assert(Compare::equals_int64(2363 ^ 1411085 ^ 5645, record->data));
}
