#include "stdafx.h"
#include "ternaryconditionaloperator_test.h"


void TernaryConditionalOperator_Test::run()
{
    initTestCase();
    test_ternarySimple();
    test_ternaryComplex();
    cleanupTestCase();
}

void TernaryConditionalOperator_Test::initTestCase()
{

}

void TernaryConditionalOperator_Test::cleanupTestCase()
{

}

void TernaryConditionalOperator_Test::test_ternarySimple()
{
    const std::u32string code1 = UR"(
var x; // == null
a = x == null ? 123 : 456;
b = x != null ? 5 : 7;
)";
    EScript engine;
    engine.setShowTCode(false);
    engine.setShowDisassembleListing(false);
    engine.eval(code1);
    auto mainTable = engine.unit()->block()->symbolTable();
    auto a = mainTable->find(U"a");
    auto record = engine.getObjectRecord(a);
    assert(record->type == SymbolType::Integer);
    assert(Compare::equals_int64(123, record->data));

    auto b = mainTable->find(U"b");
    record = engine.getObjectRecord(b);
    assert(record->type == SymbolType::Integer);
    assert(Compare::equals_int64(7, record->data));
}

void TernaryConditionalOperator_Test::test_ternaryComplex()
{
    const std::u32string code1 = UR"(
var x = 45; // == null
y = 34;
z = 14;
a = x > y && y > z ? x + y : y + z;
)";
    EScript engine;
    engine.setShowTCode(false);
    engine.setShowDisassembleListing(false);
    engine.eval(code1);
    auto mainTable = engine.unit()->block()->symbolTable();
    auto a = mainTable->find(U"a");
    auto record = engine.getObjectRecord(a);
    assert(record->type == SymbolType::Integer);
    assert(Compare::equals_int64(45+34, record->data));
}
