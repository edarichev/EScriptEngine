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
    test_realAssign1();
    test_realAssignChangeTypeToReal();
    test_realAssignChangeTypeToInt();
    test_realAssignChangeTypeToRealVar();
    test_realAssignChangeTypeToIntVar();
    test_chTypeBinaryOp1();
    test_chTypeBinaryOp2();
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

bool equals_int64(int64_t d1, uint64_t raw)
{
    return d1 == bit_cast<int64_t>(raw);
}

bool equals_double(double d1, double d2)
{
    return abs(d1 - d2) <= numeric_limits<double>::epsilon();
}

bool equals_double(double d1, uint64_t doubleAsUInt64)
{
    double d2 = bit_cast<double>(doubleAsUInt64);
    return abs(d1 - d2) <= numeric_limits<double>::epsilon();
}

void SimpleExpression_Test::test_realAssign1()
{
    EScript engine1;
    const u32string code1 = U"x = 1.5;";
    engine1.eval(code1);
    auto x = engine1.unit()->block()->symbolTable()->find(U"x");
    auto record = engine1.getObjectRecord(x);
    assert(equals_double(1.5, record->data));

}

void SimpleExpression_Test::test_realAssignChangeTypeToReal()
{
    EScript engine1;
    const u32string code1 = U"x = 5; x = 4.7;";
    engine1.eval(code1);
    auto x = engine1.unit()->block()->symbolTable()->find(U"x");
    auto record = engine1.getObjectRecord(x);
    assert(record->type == SymbolType::Real);
    assert(equals_double(4.7, record->data));
}

void SimpleExpression_Test::test_realAssignChangeTypeToInt()
{
    EScript engine1;
    const u32string code1 = U"x = 8.5; x = 7;";
    engine1.eval(code1);
    auto x = engine1.unit()->block()->symbolTable()->find(U"x");
    auto record = engine1.getObjectRecord(x);
    assert(record->type == SymbolType::Integer);
    assert(equals_int64(7, record->data));
}

void SimpleExpression_Test::test_realAssignChangeTypeToRealVar()
{
    EScript engine1;
    const u32string code1 = U"x = 5; y = 4.7; x = y;";
    engine1.eval(code1);
    auto x = engine1.unit()->block()->symbolTable()->find(U"x");
    auto record = engine1.getObjectRecord(x);
    assert(record->type == SymbolType::Real);
    assert(equals_double(4.7, record->data));
}

void SimpleExpression_Test::test_realAssignChangeTypeToIntVar()
{
    EScript engine1;
    const u32string code1 = U"x = 8.5; y = 7; x = y;";
    engine1.eval(code1);
    auto x = engine1.unit()->block()->symbolTable()->find(U"x");
    auto record = engine1.getObjectRecord(x);
    assert(record->type == SymbolType::Integer);
    assert(equals_int64(7, record->data));
}

void SimpleExpression_Test::test_chTypeBinaryOp1()
{
    EScript engine1;
    const u32string code1 = U"x = 11; x = x + 2.3;";
    engine1.eval(code1);
    auto x = engine1.unit()->block()->symbolTable()->find(U"x");
    auto record = engine1.getObjectRecord(x);
    assert(record->type == SymbolType::Real);
    assert(equals_double(11+2.3, record->data));
}

void SimpleExpression_Test::test_chTypeBinaryOp2()
{
    EScript engine1;
    const u32string code1 = U"x = 2.7; y = 17; x = x + y;";
    engine1.eval(code1);
    auto x = engine1.unit()->block()->symbolTable()->find(U"x");
    auto record = engine1.getObjectRecord(x);
    assert(record->type == SymbolType::Real);
    assert(equals_double(2.7+17, record->data));
}
