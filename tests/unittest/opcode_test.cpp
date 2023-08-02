#include "stdafx.h"
#include "opcode_test.h"

#include "tcode.h"

using namespace escript;
using namespace std;


void OpCode_Test::run()
{
    initTestCase();
    test_opCodeWork();
    cleanupTestCase();
}

void OpCode_Test::initTestCase()
{

}

void OpCode_Test::cleanupTestCase()
{

}

void OpCode_Test::test_opCodeWork()
{
    TCode code1;
    code1.operand1Type = SymbolType::Integer;
    code1.operand1.intValue = 15;
    code1.operand2Type = SymbolType::Real;
    code1.operand2.realValue = 16;
    assert(code1.operand1.intValue == 15);
    assert(code1.operand1Type == SymbolType::Integer);
    assert(code1.operand2.realValue == 16);
    assert(code1.operand2Type == SymbolType::Real);
    IntType i1 = 5678;
    IntType i2 = 654123;
    code1.setOperand1(SymbolType::Integer, &i1);
    code1.setOperand2(SymbolType::Integer, &i2);
    assert(code1.operand1.intValue == i1);
    assert(code1.operand1Type == SymbolType::Integer);
    assert(code1.operand2.intValue == i2);
    assert(code1.operand2Type == SymbolType::Integer);
}
