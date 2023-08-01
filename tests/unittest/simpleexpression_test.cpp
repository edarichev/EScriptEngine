#include "stdafx.h"
#include "simpleexpression_test.h"
#include "escript.h"

using namespace escript;
using namespace std;

void SimpleExpression_Test::run()
{
    initTestCase();
    test_simpleAssign();
    test_unaryPlusMinus();
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
    EScript engine;
    const u32string code = U"i = 3 + 2;";
    engine.eval(code);
    const u32string code2 = U"i = 3 + 2*3;";
    engine.eval(code2);
    const u32string code3 = U"x = y = 4;";
    engine.eval(code3);

}

void SimpleExpression_Test::test_unaryPlusMinus()
{
    EScript engine;
    const u32string code1 = U"x = -7 + 8 * -3;";
    engine.eval(code1);
    const u32string code2 = U"i = 0; x = i + -7;";
    engine.eval(code2);
    const u32string code3 = U"i = 0; x = -i + -7;";
    engine.eval(code3);
    cout << "Unary plus:\n";
    const u32string code4 = U"x = +7 + +8 * -3;";
    engine.eval(code4);
    const u32string code5 = U"i = 0; x = +i + -7;";
    engine.eval(code5);
    const u32string code6 = U"i = 0; x = -i + +i + -7;";
    engine.eval(code6);
    cout << "end if Unary plus\n";
}
