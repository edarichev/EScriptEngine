#include "stdafx.h"
#include "array_test.h"


void Array_Test::run()
{
    initTestCase();
    test_arrayAccess();
    test_arrayItemSet();
    test_arrayItemGet();
    test_arrayItemSwap();
    test_arrayItemAdd();
    test_arrayIfElse();
    test_arrayStringKeys();
    test_arrayStringValues();
    test_arrayRealKeys();
    test_arrayRealValues();
    test_callAccessOfFuncExpression();
    test_callAccessOfParenthExpression();
    test_callAccessOfArrayDeclExpression();
    test_array2D();
    cleanupTestCase();
}

void Array_Test::initTestCase()
{

}

void Array_Test::cleanupTestCase()
{

}

void Array_Test::test_arrayAccess()
{
    const u32string code1 = U"a = [3,6,9,]; y = a[0]; z = a[1]; t = a[2];";
    EScript engine;
    engine.eval(code1);
    auto mainTable = engine.unit()->block()->symbolTable();
    auto a = mainTable->find(U"a");
    assert(a != nullptr);
    auto record = engine.getObjectRecord(a);
    assert(record != nullptr);
    assert(record->type == SymbolType::Array);
    Array *arr = (Array*)record->data;
    assert(arr->get(0).intValue == 3);
    assert(arr->get(1).intValue == 6);
    assert(arr->get(2).intValue == 9);

    auto y = mainTable->find(U"y");
    assert(y != nullptr);
    record = engine.getObjectRecord(y);
    assert(record != nullptr);
    assert(record->type == SymbolType::Integer);
    assert(Compare::equals_int64(3, record->data));

    auto z = mainTable->find(U"z");
    assert(z != nullptr);
    record = engine.getObjectRecord(z);
    assert(record != nullptr);
    assert(record->type == SymbolType::Integer);
    assert(Compare::equals_int64(6, record->data));

    auto t = mainTable->find(U"t");
    assert(t != nullptr);
    record = engine.getObjectRecord(t);
    assert(record != nullptr);
    assert(record->type == SymbolType::Integer);
    assert(Compare::equals_int64(9, record->data));
}

void Array_Test::test_arrayItemSet()
{
    const u32string code1 = U"a = [3,6,9,]; a[1] = 88; t = a[1];";
    EScript engine;
    engine.eval(code1);
    auto mainTable = engine.unit()->block()->symbolTable();
    auto a = mainTable->find(U"a");
    assert(a != nullptr);
    auto record = engine.getObjectRecord(a);
    assert(record != nullptr);
    assert(record->type == SymbolType::Array);
    Array *arr = (Array*)record->data;
    assert(arr->get(0).intValue == 3);
    assert(arr->get(1).intValue == 88);
    assert(arr->get(2).intValue == 9);

    auto t = mainTable->find(U"t");
    assert(t != nullptr);
    record = engine.getObjectRecord(t);
    assert(record != nullptr);
    assert(record->type == SymbolType::Integer);
    assert(Compare::equals_int64(88, record->data));
}

void Array_Test::test_arrayItemGet()
{
    const u32string code1 = U"a = [3,6,9,]; t = a[1];";
    EScript engine;
    engine.eval(code1);
    auto mainTable = engine.unit()->block()->symbolTable();
    auto t = mainTable->find(U"t");
    assert(t != nullptr);
    auto record = engine.getObjectRecord(t);
    assert(record != nullptr);
    assert(record->type == SymbolType::Integer);
    assert(Compare::equals_int64(6, record->data));
}

void Array_Test::test_arrayItemSwap()
{
    const u32string code1 = U"a = [3,6,9,]; i = 1; j = 2; t = a[i]; a[i] = a[j]; a[j] = t;";
    EScript engine;
    engine.eval(code1);
    auto mainTable = engine.unit()->block()->symbolTable();
    auto t = mainTable->find(U"t");
    assert(t != nullptr);
    auto record = engine.getObjectRecord(t);
    assert(record != nullptr);
    assert(record->type == SymbolType::Integer);
    assert(Compare::equals_int64(6, record->data));
}
void Array_Test::test_arrayItemAdd()
{
    const string prog1 = R"(
a = [11,17];
var x = 0;
x = x + a[1];
)";
    const std::u32string code1 = to_u32string(prog1);
    EScript engine;
    engine.setShowDisassembleListing(false);
    engine.setShowTCode(false);
    engine.eval(code1);

    auto mainTable = engine.unit()->block()->symbolTable();
    auto x = mainTable->find(U"x");
    auto record = engine.getObjectRecord(x);
    assert(record->type == SymbolType::Integer);
    assert(Compare::equals_int64(17, record->data));
}

void Array_Test::test_arrayIfElse()
{
    const std::string prog1 = R"(
arr = [1, 3, 5, 7, 8, 9];
v = 5;
function fx(arr, x) {
    if(arr[1] > x)
        return 2;
    else
        return 1;
}
i = fx(arr, v);

    )";
    const std::u32string code1 = to_u32string(prog1);
    EScript engine;
    stringstream ss;
    engine.setOutStream(ss);
    engine.setShowDisassembleListing(false);
    engine.setShowTCode(false);
    engine.eval(code1);
    auto mainTable = engine.unit()->block()->symbolTable();
    auto i = mainTable->find(U"i");
    auto record = engine.getObjectRecord(i);
    mainTable = engine.unit()->block()->symbolTable();
    assert(record->type == SymbolType::Integer);
    assert(Compare::equals_int64(1, record->data));
}

void Array_Test::test_arrayStringKeys()
{
    const string prog1 = R"(
a = [];
a["key1"] = 123;
var x = a["key1"];

)";
    const std::u32string code1 = to_u32string(prog1);
    EScript engine;
    engine.setShowDisassembleListing(false);
    engine.setShowTCode(false);
    engine.eval(code1);

    auto mainTable = engine.unit()->block()->symbolTable();
    auto x = mainTable->find(U"x");
    auto record = engine.getObjectRecord(x);
    assert(record->type == SymbolType::Integer);
    assert(Compare::equals_int64(123, record->data));
}

void Array_Test::test_arrayStringValues()
{
    const string prog1 = R"(
a = [];
a[0] = "hello";
var x = a[0];

)";
    const std::u32string code1 = to_u32string(prog1);
    EScript engine;
    engine.setShowDisassembleListing(false);
    engine.setShowTCode(false);
    engine.eval(code1);

    auto mainTable = engine.unit()->block()->symbolTable();
    auto x = mainTable->find(U"x");
    auto record = engine.getObjectRecord(x);
    assert(record->type == SymbolType::String);
    assert(U"hello" == ((StringObject*)record->data)->uString());
}

void Array_Test::test_arrayRealKeys()
{
    const string prog1 = R"(
a = [];
a[10.5] = "hello";
var x = a[10.5];

)";
    const std::u32string code1 = to_u32string(prog1);
    EScript engine;
    engine.setShowDisassembleListing(false);
    engine.setShowTCode(false);
    engine.eval(code1);

    auto mainTable = engine.unit()->block()->symbolTable();
    auto x = mainTable->find(U"x");
    auto record = engine.getObjectRecord(x);
    assert(record->type == SymbolType::String);
    assert(U"hello" == ((StringObject*)record->data)->uString());
}

void Array_Test::test_arrayRealValues()
{
    const string prog1 = R"(
a = [];
a[10.5] = 14.5;
var x = a[10.5];

)";
    const std::u32string code1 = to_u32string(prog1);
    EScript engine;
    engine.setShowDisassembleListing(false);
    engine.setShowTCode(false);
    engine.eval(code1);

    auto mainTable = engine.unit()->block()->symbolTable();
    auto x = mainTable->find(U"x");
    auto record = engine.getObjectRecord(x);
    assert(record->type == SymbolType::Real);
    assert(Compare::equals_double(14.5, record->data));
}

void Array_Test::test_callAccessOfFuncExpression()
{
    const u32string code1 = UR"(
function fa() {
    a = [1,2,3,4,5];
    return a;
}

x = fa()[2];
)";
    EScript engine;
    engine.setShowDisassembleListing(false);
    engine.setShowTCode(false);
    engine.eval(code1);

    auto mainTable = engine.unit()->block()->symbolTable();
    auto x = mainTable->find(U"x");
    auto record = engine.getObjectRecord(x);
    assert(record->type == SymbolType::Integer);
    assert(Compare::equals_int64(3, record->data));
}

void Array_Test::test_callAccessOfParenthExpression()
{
    const u32string code1 = UR"(
function fa() {
    a = [1,2,3,4,5];
    return a;
}

x = (fa())[2];
)";
    EScript engine;
    engine.setShowDisassembleListing(false);
    engine.setShowTCode(false);
    engine.eval(code1);

    auto mainTable = engine.unit()->block()->symbolTable();
    auto x = mainTable->find(U"x");
    auto record = engine.getObjectRecord(x);
    assert(record->type == SymbolType::Integer);
    assert(Compare::equals_int64(3, record->data));
}

void Array_Test::test_callAccessOfArrayDeclExpression()
{
    const u32string code1 = UR"(
x = [1,2,3,4,5][2];
)";
    EScript engine;
    engine.setShowDisassembleListing(false);
    engine.setShowTCode(false);
    engine.eval(code1);

    auto mainTable = engine.unit()->block()->symbolTable();
    auto x = mainTable->find(U"x");
    auto record = engine.getObjectRecord(x);
    assert(record->type == SymbolType::Integer);
    assert(Compare::equals_int64(3, record->data));
}

void Array_Test::test_array2D()
{
    const u32string code1 = UR"(
a = [[1,2,3], [4,5,6]];
x = a[1][1];
)";
    EScript engine;
    engine.setShowDisassembleListing(false);
    engine.setShowTCode(false);
    engine.eval(code1);

    auto mainTable = engine.unit()->block()->symbolTable();
    auto x = mainTable->find(U"x");
    auto record = engine.getObjectRecord(x);
    assert(record->type == SymbolType::Integer);
    assert(Compare::equals_int64(5, record->data));
}
