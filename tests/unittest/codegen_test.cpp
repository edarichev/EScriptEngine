#include "stdafx.h"
#include "codegen_test.h"

using namespace escript;
using namespace std;


void CodeGen_Test::run()
{
    initTestCase();
    test_simpleExpression();
    cleanupTestCase();
}

void CodeGen_Test::initTestCase()
{

}

void CodeGen_Test::cleanupTestCase()
{

}

void CodeGen_Test::test_simpleExpression()
{
    EScript engine;
    const u32string code1 = U"x = 10;";
    engine.eval(code1);
}
