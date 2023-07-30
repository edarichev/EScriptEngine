#include "stdafx.h"
#include "simpleexpression_test.h"
#include "escript.h"

using namespace escript;
using namespace std;

void SimpleExpression_Test::run()
{
    initTestCase();
    test_simpleAssign();
    cleanupTestCase();
}

void SimpleExpression_Test::initTestCase()
{

}

void SimpleExpression_Test::cleanupTestCase()
{

}

void SimpleExpression_Test::test_simpleAssign()
{
    const u32string code = U"i = 3 + 2;";
    EScript engine;
    engine.eval(code);
    const u32string code2 = U"i = 3 + 2*3;";
    engine.eval(code2);
}
