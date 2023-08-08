#include "stdafx.h"
#include "functiondeclstatement_test.h"



void FunctionDeclStatement_Test::run()
{
    initTestCase();
    test_functionDeclOnly();
    cleanupTestCase();
}

void FunctionDeclStatement_Test::initTestCase()
{

}

void FunctionDeclStatement_Test::cleanupTestCase()
{

}

void FunctionDeclStatement_Test::test_functionDeclOnly()
{
    const u32string code1 = U"function myFunc(x) { x = 0; y = x*x; }";
    EScript engine;
    engine.eval(code1);
}
