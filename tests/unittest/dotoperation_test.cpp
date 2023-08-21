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

}

void DotOperation_Test::test_dotArrayDeclExpression()
{

}

void DotOperation_Test::test_dotArrayItemRefExpression()
{
    const std::u32string code1 = UR"(
x = ["hello", "world"][0].length; // == 5
)";
}

void DotOperation_Test::test_dotStringLiteralExpression()
{

}

void DotOperation_Test::test_dotFunctionCallExpression()
{

}
