#include "stdafx.h"
#include "boolean_test.h"
#include "compare.h"

using namespace escript;
using namespace std;


void Boolean_Test::run()
{
    initTestCase();
    test_trueFalse();
    test_less();
    test_lessOrEqual();
    test_greater();
    test_greaterOrEqual();
    test_equal();
    test_arrayItemsLess();
    test_compareToNaN();
    cleanupTestCase();
}

void Boolean_Test::initTestCase()
{

}

void Boolean_Test::cleanupTestCase()
{

}

void Boolean_Test::test_trueFalse()
{
    const u32string code1 = U"i = true;";
    EScript engine;
    engine.eval(code1);
    auto mainTable = engine.unit()->block()->symbolTable();
    auto i = mainTable->find(U"i");
    auto record = engine.getObjectRecord(i);
    assert(record->type == SymbolType::Boolean);
    assert(Compare::equals_bool(true, record->data));

    const u32string code2 = U"j = false;";
    engine.eval(code2);
    auto j = mainTable->find(U"j");
    record = engine.getObjectRecord(j);
    assert(record->type == SymbolType::Boolean);
    assert(Compare::equals_bool(false, record->data));

    const u32string code3 = U"j = i;";
    engine.eval(code3);
    j = mainTable->find(U"j");
    record = engine.getObjectRecord(j);
    assert(record->type == SymbolType::Boolean);
    assert(Compare::equals_bool(true, record->data));
}

void Boolean_Test::test_less()
{
    const u32string code1 = U"i = 0 < 1;";
    EScript engine;
    engine.eval(code1);
    auto mainTable = engine.unit()->block()->symbolTable();
    auto i = mainTable->find(U"i");
    auto record = engine.getObjectRecord(i);
    assert(record->type == SymbolType::Boolean);
    assert(Compare::equals_bool(true, record->data));

    const u32string code2 = U"i = 0; y = i < 1;";
    engine.eval(code2);
    mainTable = engine.unit()->block()->symbolTable();
    auto y = mainTable->find(U"y");
    record = engine.getObjectRecord(y);
    assert(record->type == SymbolType::Boolean);
    assert(Compare::equals_bool(true, record->data));
}

void Boolean_Test::test_lessOrEqual()
{
    const u32string code1 = U"i = 0 <= 1;";
    EScript engine;
    engine.eval(code1);
    auto mainTable = engine.unit()->block()->symbolTable();
    auto i = mainTable->find(U"i");
    auto record = engine.getObjectRecord(i);
    assert(record->type == SymbolType::Boolean);
    assert(Compare::equals_bool(true, record->data));

    const u32string code2 = U"i = 0; y = i <= 1;";
    engine.eval(code2);
    mainTable = engine.unit()->block()->symbolTable();
    auto y = mainTable->find(U"y");
    record = engine.getObjectRecord(y);
    assert(record->type == SymbolType::Boolean);
    assert(Compare::equals_bool(true, record->data));
}

void Boolean_Test::test_greater()
{
    const u32string code1 = U"i = 0 > 1;";
    EScript engine;
    engine.eval(code1);
    auto mainTable = engine.unit()->block()->symbolTable();
    auto i = mainTable->find(U"i");
    auto record = engine.getObjectRecord(i);
    assert(record->type == SymbolType::Boolean);
    assert(Compare::equals_bool(false, record->data));

    const u32string code2 = U"i = 0; y = i > 1;";
    engine.eval(code2);
    mainTable = engine.unit()->block()->symbolTable();
    auto y = mainTable->find(U"y");
    record = engine.getObjectRecord(y);
    assert(record->type == SymbolType::Boolean);
    assert(Compare::equals_bool(false, record->data));
}

void Boolean_Test::test_greaterOrEqual()
{
    const u32string code1 = U"i = 0 >= 1;";
    EScript engine;
    engine.eval(code1);
    auto mainTable = engine.unit()->block()->symbolTable();
    auto i = mainTable->find(U"i");
    auto record = engine.getObjectRecord(i);
    assert(record->type == SymbolType::Boolean);
    assert(Compare::equals_bool(false, record->data));

    const u32string code2 = U"i = 0; y = i >= 1;";
    engine.eval(code2);
    mainTable = engine.unit()->block()->symbolTable();
    auto y = mainTable->find(U"y");
    record = engine.getObjectRecord(y);
    assert(record->type == SymbolType::Boolean);
    assert(Compare::equals_bool(false, record->data));
}

void Boolean_Test::test_equal()
{
    const u32string code1 = U"i = 0 == 0;";
    EScript engine;
    engine.eval(code1);
    auto mainTable = engine.unit()->block()->symbolTable();
    auto i = mainTable->find(U"i");
    auto record = engine.getObjectRecord(i);
    assert(record->type == SymbolType::Boolean);
    assert(Compare::equals_bool(true, record->data));

    const u32string code2 = U"i = 0; y = i == 1;";
    engine.eval(code2);
    mainTable = engine.unit()->block()->symbolTable();
    auto y = mainTable->find(U"y");
    record = engine.getObjectRecord(y);
    assert(record->type == SymbolType::Boolean);
    assert(Compare::equals_bool(false, record->data));

    const u32string code3 = U"i = 0; y = i == 0;";
    engine.eval(code3);
    mainTable = engine.unit()->block()->symbolTable();
    y = mainTable->find(U"y");
    record = engine.getObjectRecord(y);
    assert(record->type == SymbolType::Boolean);
    assert(Compare::equals_bool(true, record->data));

    const u32string code4 = U"y = 10 == 0;";
    engine.eval(code4);
    mainTable = engine.unit()->block()->symbolTable();
    y = mainTable->find(U"y");
    record = engine.getObjectRecord(y);
    assert(record->type == SymbolType::Boolean);
    assert(Compare::equals_bool(false, record->data));

    const u32string code5 = U"a = 0; y = 10 == a;";
    engine.eval(code5);
    mainTable = engine.unit()->block()->symbolTable();
    y = mainTable->find(U"y");
    record = engine.getObjectRecord(y);
    assert(record->type == SymbolType::Boolean);
    assert(Compare::equals_bool(false, record->data));
}

void Boolean_Test::test_arrayItemsLess()
{
    const u32string code1 = U"a = [3,7,11]; i = 1; j = 2; x = a[i]; b = false; if (a[i] < a[j]) b = true;";
    EScript engine;
    engine.eval(code1);
    auto mainTable = engine.unit()->block()->symbolTable();
    auto x = mainTable->find(U"x");
    auto record = engine.getObjectRecord(x);
    assert(record->type == SymbolType::Integer);
    assert(Compare::equals_int64(7, record->data));

    auto b = mainTable->find(U"b");
    record = engine.getObjectRecord(b);
    assert(record->type == SymbolType::Boolean);
    assert(Compare::equals_bool(true, record->data));
}

void Boolean_Test::test_compareToNaN()
{
    const u32string code1 = UR"(
x = NaN;
b = x == NaN;
)";
    EScript engine;
    engine.eval(code1);
    auto mainTable = engine.unit()->block()->symbolTable();
    auto x = mainTable->find(U"x");
    auto record = engine.getObjectRecord(x);
    assert(record->type == SymbolType::Real);
    assert(Compare::equals_double(std::numeric_limits<double>::signaling_NaN(), record->data));

    auto b = mainTable->find(U"b");
    record = engine.getObjectRecord(b);
    assert(record->type == SymbolType::Boolean);
    assert(Compare::equals_bool(true, record->data));
}

