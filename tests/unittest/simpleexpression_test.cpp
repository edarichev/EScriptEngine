#include "stdafx.h"
#include "simpleexpression_test.h"
#include "escript.h"
#include "compare.h"

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
    test_sequentialRun();
    test_braceSimple();
    test_mod();
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



void SimpleExpression_Test::test_realAssign1()
{
    EScript engine1;
    const u32string code1 = U"x = 1.5;";
    engine1.eval(code1);
    auto x = engine1.unit()->block()->symbolTable()->find(U"x");
    auto record = engine1.getObjectRecord(x);
    assert(Compare::equals_double(1.5, record->data));

}

void SimpleExpression_Test::test_realAssignChangeTypeToReal()
{
    EScript engine1;
    const u32string code1 = U"x = 5; x = 4.7;";
    engine1.eval(code1);
    auto x = engine1.unit()->block()->symbolTable()->find(U"x");
    auto record = engine1.getObjectRecord(x);
    assert(record->type == SymbolType::Real);
    assert(Compare::equals_double(4.7, record->data));
}

void SimpleExpression_Test::test_realAssignChangeTypeToInt()
{
    EScript engine1;
    const u32string code1 = U"x = 8.5; x = 7;";
    engine1.eval(code1);
    auto x = engine1.unit()->block()->symbolTable()->find(U"x");
    auto record = engine1.getObjectRecord(x);
    assert(record->type == SymbolType::Integer);
    assert(Compare::equals_int64(7, record->data));
}

void SimpleExpression_Test::test_realAssignChangeTypeToRealVar()
{
    EScript engine1;
    const u32string code1 = U"x = 5; y = 4.7; x = y;";
    engine1.eval(code1);
    auto x = engine1.unit()->block()->symbolTable()->find(U"x");
    auto record = engine1.getObjectRecord(x);
    assert(record->type == SymbolType::Real);
    assert(Compare::equals_double(4.7, record->data));
}

void SimpleExpression_Test::test_realAssignChangeTypeToIntVar()
{
    EScript engine1;
    const u32string code1 = U"x = 8.5; y = 7; x = y;";
    engine1.eval(code1);
    auto x = engine1.unit()->block()->symbolTable()->find(U"x");
    auto record = engine1.getObjectRecord(x);
    assert(record->type == SymbolType::Integer);
    assert(Compare::equals_int64(7, record->data));
}

void SimpleExpression_Test::test_chTypeBinaryOp1()
{
    EScript engine1;
    const u32string code1 = U"x = 11; x = x + 2.3;";
    engine1.eval(code1);
    auto x = engine1.unit()->block()->symbolTable()->find(U"x");
    auto record = engine1.getObjectRecord(x);
    assert(record->type == SymbolType::Real);
    assert(Compare::equals_double(11+2.3, record->data));
}

void SimpleExpression_Test::test_chTypeBinaryOp2()
{
    EScript engine1;
    const u32string code1 = U"x = 2.7; y = 17; x = x + y;";
    engine1.eval(code1);
    auto x = engine1.unit()->block()->symbolTable()->find(U"x");
    auto record = engine1.getObjectRecord(x);
    assert(record->type == SymbolType::Real);
    assert(Compare::equals_double(2.7+17, record->data));
}

void SimpleExpression_Test::test_sequentialRun()
{
    EScript engine;
    const u32string code1 = U"x = 45;";
    engine.eval(code1);
    auto x = engine.unit()->block()->symbolTable()->find(U"x");
    auto record = engine.getObjectRecord(x);
    assert(record->type == SymbolType::Integer);
    assert(Compare::equals_int64(45, record->data));
    // добавим второй блок
    const u32string code2 = U"y = x + 45;";
    engine.eval(code2);
    auto y = engine.unit()->block()->symbolTable()->find(U"y");
    auto recordY = engine.getObjectRecord(y);
    assert(recordY->type == SymbolType::Integer);
    assert(Compare::equals_int64(90, recordY->data));
    // добавим второй блок, сменим тип
    const u32string code3 = U"z = y + 3.3;";
    engine.eval(code3);
    auto z = engine.unit()->block()->symbolTable()->find(U"z");
    auto recordZ = engine.getObjectRecord(z);
    assert(recordZ->type == SymbolType::Real);
    assert(Compare::equals_double(90+3.3, recordZ->data));
}

void SimpleExpression_Test::test_braceSimple()
{
    EScript engine;
    const u32string code1 = U"{x = 45;}";
    engine.eval(code1);
    // добавим ещё выражение с подблоками и одинаковым именем переменной
    const u32string code2 = U"{{}{x = 54;}}";
    engine.eval(code2);
    auto mainBlock = engine.unit()->block();
    // это блок модуля
    auto x = mainBlock->symbolTable()->find(U"x");
    assert(x == nullptr); // x не должно быть в глобальной области
    // это - сам блок, полученный из выражения "{x = 45;}"
    // его таблица символов перемещается в mainBlock
    auto codeBlock1 = mainBlock->blocks()[0];
    // а это - подблок, который определяется фигурными скобками
    // в выражениии {x = 45;}
    auto subBlock1OfCodeBlock1 = codeBlock1->blocks()[0];
    // в нём находится локальная переменная x
    x = subBlock1OfCodeBlock1->symbolTable()->find(U"x");
    assert(x != nullptr); // он должен быть в первом дочернем блоке
    auto record = engine.getObjectRecord(x);
    assert(record->type == SymbolType::Integer);
    assert(Compare::equals_int64(45, record->data));
    // второй блок - результат исполнения code2
    auto codeBlock2 = mainBlock->blocks()[1];
    // пустого блока не должно быть, он пропущен
    auto subBlock1OfCodeBlock2 = codeBlock2->blocks()[0]->blocks()[0];
    // в нём находится локальная переменная, тоже с именем x, но == 54
    x = subBlock1OfCodeBlock2->symbolTable()->find(U"x");
    assert(x != nullptr);
    record = engine.getObjectRecord(x);
    assert(record->type == SymbolType::Integer);
    assert(Compare::equals_int64(54, record->data));
}

void SimpleExpression_Test::test_mod()
{
    const u32string code1 = U"a = 157; b = 7; c = a % b;";
    EScript engine;
    engine.setShowDisassembleListing(false);
    engine.setShowTCode(false);
    engine.eval(code1);
    auto mainTable = engine.unit()->block()->symbolTable();
    auto c = mainTable->find(U"c");
    auto record = engine.getObjectRecord(c);
    assert(record->type == SymbolType::Integer);
    assert(Compare::equals_int64(157 % 7, record->data));
}
