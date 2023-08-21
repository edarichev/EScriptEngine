#include "stdafx.h"
#include "dotoperation_test.h"


void DotOperation_Test::run()
{
    test_dotParenthExpression();
    test_dotArrayDeclExpression();
    test_dotArrayItemRefExpression();
    test_dotStringLiteralExpression();
    test_dotFunctionCallExpression();
}

void DotOperation_Test::initTestCase()
{

}

void DotOperation_Test::cleanupTestCase()
{

}

void DotOperation_Test::test_dotParenthExpression()
{
    const std::u32string code1 = UR"(
x = (["hello, ", "world"])[0].length; // == 7
)";
    EScript engine;
    engine.setShowDisassembleListing(false);
    engine.setShowTCode(false);
    engine.eval(code1);

    auto mainTable = engine.unit()->block()->symbolTable();
    auto x = mainTable->find(U"x");
    auto record = engine.getObjectRecord(x);
    assert(record->type == SymbolType::Integer);
    assert(Compare::equals_int64(7, record->data));
}

void DotOperation_Test::test_dotArrayDeclExpression()
{
    const std::u32string code1 = UR"(
x = ["hello, ", "world"][1].length; // == 5
)";
    EScript engine;
    engine.setShowDisassembleListing(false);
    engine.setShowTCode(false);
    engine.eval(code1);

    auto mainTable = engine.unit()->block()->symbolTable();
    auto x = mainTable->find(U"x");
    auto record = engine.getObjectRecord(x);
    assert(record->type == SymbolType::Integer);
    assert(Compare::equals_int64(5, record->data));
}

void DotOperation_Test::test_dotArrayItemRefExpression()
{

}

void DotOperation_Test::test_dotStringLiteralExpression()
{

}

void DotOperation_Test::test_dotFunctionCallExpression()
{
    const std::u32string code1 = UR"(
function fn() { return "Hello"; }
x = fn().length;
)";
    EScript engine;
    engine.setShowDisassembleListing(false);
    engine.setShowTCode(false);
    engine.eval(code1);

    auto mainTable = engine.unit()->block()->symbolTable();
    auto x = mainTable->find(U"x");
    auto record = engine.getObjectRecord(x);
    assert(record->type == SymbolType::Integer);
    assert(Compare::equals_int64(5, record->data));
}
