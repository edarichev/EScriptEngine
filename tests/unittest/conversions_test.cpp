#include "stdafx.h"
#include "conversions_test.h"


void Conversions_Test::run()
{
    initTestCase();
    test_convertToRealPlus();
    test_convertToRealMinus();
    test_convertToRealMultiply();
    test_convertToRealDivide();
    test_convertToString();
    cleanupTestCase();
}

void Conversions_Test::initTestCase()
{

}

void Conversions_Test::cleanupTestCase()
{

}

void Conversions_Test::test_convertToRealPlus()
{
    const u32string code1 = U"a = 123; b = 23.5; c = a + b;";
    EScript engine;
    engine.eval(code1);
    auto mainTable = engine.unit()->block()->symbolTable();
    auto c = mainTable->find(U"c");
    auto record = engine.getObjectRecord(c);
    assert(record->type == SymbolType::Real);
    assert(Compare::equals_double(123+23.5, record->data));

    const u32string code0 = U"a = 22.7; b = 25; c = a + b;";
    engine.eval(code0);
    c = mainTable->find(U"c");
    record = engine.getObjectRecord(c);
    assert(record->type == SymbolType::Real);
    assert(Compare::equals_double(22.7+25, record->data));

    const u32string code2 = U"a = true; b = 23.5; c = a + b;";
    engine.eval(code2);
    c = mainTable->find(U"c");
    record = engine.getObjectRecord(c);
    assert(record->type == SymbolType::Real);
    assert(Compare::equals_double(1+23.5, record->data));

    const u32string code3 = U"a = false; b = 23.5; c = a + b;";
    engine.eval(code3);
    c = mainTable->find(U"c");
    record = engine.getObjectRecord(c);
    assert(record->type == SymbolType::Real);
    assert(Compare::equals_double(0+23.5, record->data));

    const u32string code4 = U"a = 17.5; b = true; c = a + b;";
    engine.eval(code4);
    c = mainTable->find(U"c");
    record = engine.getObjectRecord(c);
    assert(record->type == SymbolType::Real);
    assert(Compare::equals_double(17.5+1, record->data));

    const u32string code5 = U"a = 78.9; b = false; c = a + b;";
    engine.eval(code5);
    c = mainTable->find(U"c");
    record = engine.getObjectRecord(c);
    assert(record->type == SymbolType::Real);
    assert(Compare::equals_double(78.9+0, record->data));

    const u32string code6 = U"a = 78.9; b = 17.6; c = a + b;";
    engine.eval(code6);
    c = mainTable->find(U"c");
    record = engine.getObjectRecord(c);
    assert(record->type == SymbolType::Real);
    assert(Compare::equals_double(78.9+17.6, record->data));
}

void Conversions_Test::test_convertToRealMinus()
{
    const u32string code1 = U"a = 123; b = 23.5; c = a - b;";
    EScript engine;
    engine.eval(code1);
    auto mainTable = engine.unit()->block()->symbolTable();
    auto c = mainTable->find(U"c");
    auto record = engine.getObjectRecord(c);
    assert(record->type == SymbolType::Real);
    assert(Compare::equals_double(123-23.5, record->data));

    const u32string code0 = U"a = 22.7; b = 25; c = a - b;";
    engine.eval(code0);
    c = mainTable->find(U"c");
    record = engine.getObjectRecord(c);
    assert(record->type == SymbolType::Real);
    assert(Compare::equals_double(22.7-25, record->data));

    const u32string code2 = U"a = true; b = 23.5; c = a - b;";
    engine.eval(code2);
    c = mainTable->find(U"c");
    record = engine.getObjectRecord(c);
    assert(record->type == SymbolType::Real);
    assert(Compare::equals_double(1-23.5, record->data));

    const u32string code3 = U"a = false; b = 23.5; c = a - b;";
    engine.eval(code3);
    c = mainTable->find(U"c");
    record = engine.getObjectRecord(c);
    assert(record->type == SymbolType::Real);
    assert(Compare::equals_double(0-23.5, record->data));

    const u32string code4 = U"a = 17.5; b = true; c = a - b;";
    engine.eval(code4);
    c = mainTable->find(U"c");
    record = engine.getObjectRecord(c);
    assert(record->type == SymbolType::Real);
    assert(Compare::equals_double(17.5-1, record->data));

    const u32string code5 = U"a = 78.9; b = false; c = a - b;";
    engine.eval(code5);
    c = mainTable->find(U"c");
    record = engine.getObjectRecord(c);
    assert(record->type == SymbolType::Real);
    assert(Compare::equals_double(78.9-0, record->data));

    const u32string code6 = U"a = 78.9; b = 17.6; c = a - b;";
    engine.eval(code6);
    c = mainTable->find(U"c");
    record = engine.getObjectRecord(c);
    assert(record->type == SymbolType::Real);
    assert(Compare::equals_double(78.9-17.6, record->data));
}

void Conversions_Test::test_convertToRealMultiply()
{
    const u32string code1 = U"a = 123; b = 23.5; c = a * b;";
    EScript engine;
    engine.eval(code1);
    auto mainTable = engine.unit()->block()->symbolTable();
    auto c = mainTable->find(U"c");
    auto record = engine.getObjectRecord(c);
    assert(record->type == SymbolType::Real);
    assert(Compare::equals_double(123*23.5, record->data));

    const u32string code0 = U"a = 22.7; b = 25; c = a * b;";
    engine.eval(code0);
    c = mainTable->find(U"c");
    record = engine.getObjectRecord(c);
    assert(record->type == SymbolType::Real);
    assert(Compare::equals_double(22.7*25, record->data));

    const u32string code2 = U"a = true; b = 23.5; c = a * b;";
    engine.eval(code2);
    c = mainTable->find(U"c");
    record = engine.getObjectRecord(c);
    assert(record->type == SymbolType::Real);
    assert(Compare::equals_double(1*23.5, record->data));

    const u32string code3 = U"a = false; b = 23.5; c = a * b;";
    engine.eval(code3);
    c = mainTable->find(U"c");
    record = engine.getObjectRecord(c);
    assert(record->type == SymbolType::Real);
    assert(Compare::equals_double(0*23.5, record->data));

    const u32string code4 = U"a = 17.5; b = true; c = a * b;";
    engine.eval(code4);
    c = mainTable->find(U"c");
    record = engine.getObjectRecord(c);
    assert(record->type == SymbolType::Real);
    assert(Compare::equals_double(17.5*1, record->data));

    const u32string code5 = U"a = 78.9; b = false; c = a * b;";
    engine.eval(code5);
    c = mainTable->find(U"c");
    record = engine.getObjectRecord(c);
    assert(record->type == SymbolType::Real);
    assert(Compare::equals_double(78.9*0, record->data));

    const u32string code6 = U"a = 78.9; b = 17.6; c = a * b;";
    engine.eval(code6);
    c = mainTable->find(U"c");
    record = engine.getObjectRecord(c);
    assert(record->type == SymbolType::Real);
    assert(Compare::equals_double(78.9*17.6, record->data));
}

void Conversions_Test::test_convertToRealDivide()
{
    const u32string code1 = U"a = 123; b = 23.5; c = a / b;";
    EScript engine;
    engine.eval(code1);
    auto mainTable = engine.unit()->block()->symbolTable();
    auto c = mainTable->find(U"c");
    auto record = engine.getObjectRecord(c);
    assert(record->type == SymbolType::Real);
    assert(Compare::equals_double(123/23.5, record->data));

    const u32string code0 = U"a = 22.7; b = 25; c = a / b;";
    engine.eval(code0);
    c = mainTable->find(U"c");
    record = engine.getObjectRecord(c);
    assert(record->type == SymbolType::Real);
    assert(Compare::equals_double(22.7/25, record->data));

    const u32string code2 = U"a = true; b = 23.5; c = a / b;";
    engine.eval(code2);
    c = mainTable->find(U"c");
    record = engine.getObjectRecord(c);
    assert(record->type == SymbolType::Real);
    assert(Compare::equals_double(1/23.5, record->data));

    const u32string code3 = U"a = false; b = 23.5; c = a / b;";
    engine.eval(code3);
    c = mainTable->find(U"c");
    record = engine.getObjectRecord(c);
    assert(record->type == SymbolType::Real);
    assert(Compare::equals_double(0/23.5, record->data));

    const u32string code4 = U"a = 17.5; b = true; c = a / b;";
    engine.eval(code4);
    c = mainTable->find(U"c");
    record = engine.getObjectRecord(c);
    assert(record->type == SymbolType::Real);
    assert(Compare::equals_double(17.5/1, record->data));

    const u32string code6 = U"a = 78.9; b = 17.6; c = a / b;";
    engine.eval(code6);
    c = mainTable->find(U"c");
    record = engine.getObjectRecord(c);
    assert(record->type == SymbolType::Real);
    assert(Compare::equals_double(78.9/17.6, record->data));
}

void Conversions_Test::test_convertToString()
{
    const u32string code1 = U"a = '123' + 45;";
    EScript engine;
    engine.eval(code1);
    auto mainTable = engine.unit()->block()->symbolTable();
    auto a = mainTable->find(U"a");
    auto record = engine.getObjectRecord(a);
    assert(record->type == SymbolType::String);
    assert(U"12345" == ((StringObject*)record->data)->toString());

    const u32string code2 = U"a = 123 + '456';";
    engine.eval(code2);
    a = mainTable->find(U"a");
    record = engine.getObjectRecord(a);
    assert(record->type == SymbolType::String);
    assert(U"123456" == ((StringObject*)record->data)->toString());

    const u32string code3 = U"a = '0123' + '456';";
    engine.eval(code3);
    a = mainTable->find(U"a");
    record = engine.getObjectRecord(a);
    assert(record->type == SymbolType::String);
    assert(U"0123456" == ((StringObject*)record->data)->toString());

    const u32string code4 = U"a = 'f=' + true;";
    engine.eval(code4);
    a = mainTable->find(U"a");
    record = engine.getObjectRecord(a);
    assert(record->type == SymbolType::String);
    assert(U"f=true" == ((StringObject*)record->data)->toString());

    const u32string code5 = U"a = 'b:' + false;";
    engine.eval(code5);
    a = mainTable->find(U"a");
    record = engine.getObjectRecord(a);
    assert(record->type == SymbolType::String);
    assert(U"b:false" == ((StringObject*)record->data)->toString());

    const u32string code6 = U"a = 'f=' + 123.45;";
    engine.eval(code6);
    a = mainTable->find(U"a");
    record = engine.getObjectRecord(a);
    assert(record->type == SymbolType::String);
    assert(U"f=123.45" == ((StringObject*)record->data)->toString());

    const u32string code7 = U"a = 36.6 + '_C';";
    engine.eval(code7);
    a = mainTable->find(U"a");
    record = engine.getObjectRecord(a);
    assert(record->type == SymbolType::String);
    assert(U"36.6_C" == ((StringObject*)record->data)->toString());
}
