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
    test_parenth();
    test_integerExpressions1();
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
    EScript engine1;
    const u32string code = U"i = 3 + 2;";
    engine1.eval(code);

    EScript engine2;
    const u32string code2 = U"i = 3 + 2*3;";
    engine2.eval(code2);

    EScript engine3;
    const u32string code3 = U"x = y = 4;";
    engine3.eval(code3);

}

void SimpleExpression_Test::test_unaryPlusMinus()
{
    EScript engine1;
    const u32string code1 = U"x = -7 + 8 * -3;";
    engine1.eval(code1);

    EScript engine2;
    const u32string code2 = U"i = 0; x = i + -7;";
    engine2.eval(code2);

    EScript engine3;
    const u32string code3 = U"i = 0; x = -i + -7;";
    engine3.eval(code3);

    EScript engine4;
    //cout << "Unary plus:\n";
    const u32string code4 = U"x = +7 + +8 * -3;";
    engine4.eval(code4);
    auto x = engine4.unit()->block()->symbolTable()->find(U"x");
    auto record = engine4.getObjectRecord(x);
    assert((int64_t)record->data == -17);

    EScript engine5;
    const u32string code5 = U"i = 0; x = +i + -7;";
    engine5.eval(code5);

    EScript engine6;
    const u32string code6 = U"i = 0; x = -i + +i + -7;";
    engine6.eval(code6);
    //cout << "end of Unary plus\n";
}

void SimpleExpression_Test::test_parenth()
{
    //cout << "No Parenth:\n";
    EScript engine1;
    const u32string code1 = U"x = 7 + 8 * 3 + 2 + 5 * 4;";
    engine1.eval(code1);

    EScript engine2;
    //cout << "Parenth\'s:\n";
    const u32string code2 = U"x = ((7 + 8) * 3) + (2 + 5) * 4;";
    engine2.eval(code2);
    //cout << "End of Parenth\'s\n";
}

void SimpleExpression_Test::test_integerExpressions1()
{
    EScript engine1;
    const u32string code1 = U"x = 1 - 2 + 3*4 - 5 / 2 + 6;";
    engine1.eval(code1);
    auto x = engine1.unit()->block()->symbolTable()->find(U"x");
    auto record = engine1.getObjectRecord(x);
    assert((int64_t)record->data == 1 - 2 + 3*4 - 5 / 2 + 6);
}
