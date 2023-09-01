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
    test_arrayWith1String();
    test_arrayStringKeys();
    test_arrayStringValues();
    test_arrayRealKeys();
    test_arrayRealValues();
    test_callAccessOfFuncExpression();
    test_callAccessOfParenthExpression();
    test_callAccessOfArrayDeclExpression();
    test_array2DGetItem();
    test_array2DDeclGetItem();
    test_array2DSetItem();
    test_array3DGetItem();
    test_array3DDeclGetItem();
    test_array3DSetItem();
    test_arrayFuncRetArrayGetItem();
    test_arrayLength();
    test_arrayLengthWithProps();
    test_arrayPush();
    test_arrayPushMultiple();
    test_arrayPop();
    test_arrayShift();
    test_arraySpliceRemoveAll();
    test_arraySpliceRemoveAllFrom();
    test_arraySpliceRemoveAllFromEnd();
    test_arraySpliceRemoveNFrom0();
    test_arraySpliceRemoveNFromPos();
    test_arraySpliceRemoveNFromEnd();
    test_arraySpliceInsertAt0();
    test_arraySpliceInsertAtN();
    test_arraySpliceInsertAtEnd();
    test_arraySpliceRemoveNInsertFrom0();
    test_arraySpliceRemoveNInsertFromPos();
    test_arraySpliceRemoveNInsertFromEnd();
    test_arrayReverse();
    test_arrayFill();
    test_arraySort();
    test_arrayUnshift();
    test_arraySlice();
    test_arrayJoin();
    test_arrayFilter();
    test_arrayFirstIndex();
    test_arrayLastIndex();
    test_arrayIncludes();
    test_appendWithSplice();
    test_arrayForEach();
    test_arrayMap();
    test_arraySome();
    test_arrayEvery();
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

void Array_Test::test_arrayWith1String()
{
    const u32string code1 = UR"(
a = ["hello"];
x = a[0];
)";
    EScript engine;
    engine.setShowDisassembleListing(false);
    engine.setShowTCode(false);
    engine.eval(code1);

    auto mainTable = engine.unit()->block()->symbolTable();
    auto x = mainTable->find(U"x");
    auto record = engine.getObjectRecord(x);
    assert(record->type == SymbolType::String);
    assert(U"hello" == ((StringObject*)record->data)->toString());
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
    assert(U"hello" == ((StringObject*)record->data)->toString());
}

void Array_Test::test_arrayRealKeys()
{
    const string prog1 = R"(
a = [];
a[10.5] = "hello";
var x = a[10];

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
    assert(U"hello" == ((StringObject*)record->data)->toString());
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

void Array_Test::test_array2DGetItem()
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

void Array_Test::test_array2DDeclGetItem()
{
    const u32string code1 = UR"(
x = [[1,2,3], [4,5,6]][1][1];
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

void Array_Test::test_array2DSetItem()
{
    const u32string code1 = UR"(
a = [[1,2,3], [4,5,6]];
a[1][2] = 12;
x = a[1][2];
)";
    EScript engine;
    engine.setShowDisassembleListing(false);
    engine.setShowTCode(false);
    engine.eval(code1);

    auto mainTable = engine.unit()->block()->symbolTable();
    auto x = mainTable->find(U"x");
    auto record = engine.getObjectRecord(x);
    assert(record->type == SymbolType::Integer);
    assert(Compare::equals_int64(12, record->data));
}

void Array_Test::test_array3DGetItem()
{
    const u32string code1 = UR"(
a =
[
    [
        [1,2,3],
        [4,5,6]
    ],
    [
        [7,8,9],
        [10,11,12]
    ]
];
x = a[1][1][2];
)";
    EScript engine;
    engine.setShowDisassembleListing(false);
    engine.setShowTCode(false);
    engine.eval(code1);

    auto mainTable = engine.unit()->block()->symbolTable();
    auto x = mainTable->find(U"x");
    auto record = engine.getObjectRecord(x);
    assert(record->type == SymbolType::Integer);
    assert(Compare::equals_int64(12, record->data));
}

void Array_Test::test_array3DDeclGetItem()
{
    const u32string code1 = UR"(
x =
[
    [
        [1,2,3],
        [4,5,6]
    ],
    [
        [7,8,9],
        [10,11,12]
    ]
]
[1][1][2];

)";
    EScript engine;
    engine.setShowDisassembleListing(false);
    engine.setShowTCode(false);
    engine.eval(code1);

    auto mainTable = engine.unit()->block()->symbolTable();
    auto x = mainTable->find(U"x");
    auto record = engine.getObjectRecord(x);
    assert(record->type == SymbolType::Integer);
    assert(Compare::equals_int64(12, record->data));
}

void Array_Test::test_array3DSetItem()
{
    const u32string code1 = UR"(
a =
[
    [
        [1,2,3],
        [4,5,6]
    ],
    [
        [7,8,9],
        [10,11,12]
    ]
];
a[1][0][2] = 512;
x = a[1][0][2];
)";
    EScript engine;
    engine.setShowDisassembleListing(false);
    engine.setShowTCode(false);
    engine.eval(code1);

    auto mainTable = engine.unit()->block()->symbolTable();
    auto x = mainTable->find(U"x");
    auto record = engine.getObjectRecord(x);
    assert(record->type == SymbolType::Integer);
    assert(Compare::equals_int64(512, record->data));
}

void Array_Test::test_arrayFuncRetArrayGetItem()
{
    const std::u32string code1 = UR"(
function fn() { return "Hello"; }
x = fn()[1];
)";
    EScript engine;
    engine.setShowDisassembleListing(false);
    engine.setShowTCode(false);
    engine.eval(code1);

    auto mainTable = engine.unit()->block()->symbolTable();
    auto x = mainTable->find(U"x");
    auto record = engine.getObjectRecord(x);
    assert(record->type == SymbolType::String);
    assert(U"e" == ((StringObject*)record->data)->toString());
}

void Array_Test::test_arrayLength()
{
    const u32string code1 = UR"(a = [3,6,9,]; n = a.length;)";
    EScript engine;
    engine.eval(code1);
    auto mainTable = engine.unit()->block()->symbolTable();
    auto n = mainTable->find(U"n");
    assert(n != nullptr);
    auto record = engine.getObjectRecord(n);
    assert(record != nullptr);
    assert(record->type == SymbolType::Integer);
    assert(Compare::equals_int64(3, record->data));
}

void Array_Test::test_arrayLengthWithProps()
{
    const u32string code1 = UR"(
a = [3,6,9,]; n = a.length;
a[-1] = -1;
a['he'] = 'llo';
)";
    EScript engine;
    engine.eval(code1);
    auto mainTable = engine.unit()->block()->symbolTable();
    auto n = mainTable->find(U"n");
    assert(n != nullptr);
    auto record = engine.getObjectRecord(n);
    assert(record != nullptr);
    assert(record->type == SymbolType::Integer);
    assert(Compare::equals_int64(3, record->data));
}

void Array_Test::test_arrayPush()
{
    const u32string code1 = UR"(
a = [3,6,9,];
a.push(10);
n = a.length;
)";
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
    assert(arr->get(3).intValue == 10);

    auto n = mainTable->find(U"n");
    assert(n != nullptr);
    record = engine.getObjectRecord(n);
    assert(record != nullptr);
    assert(record->type == SymbolType::Integer);
    assert(Compare::equals_int64(4, record->data));
}

void Array_Test::test_arrayPushMultiple()
{
    const u32string code1 = UR"(
a = [3,6,9,];
a.push(10, 11, 12);
n = a.length;
)";
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
    assert(arr->get(3).intValue == 10);
    assert(arr->get(4).intValue == 11);
    assert(arr->get(5).intValue == 12);

    auto n = mainTable->find(U"n");
    assert(n != nullptr);
    record = engine.getObjectRecord(n);
    assert(record != nullptr);
    assert(record->type == SymbolType::Integer);
    assert(Compare::equals_int64(6, record->data));
}

void Array_Test::test_arrayPop()
{
    const u32string code1 = UR"(
a = [3,6,9,];
b = a.pop();
n = a.length;
)";
    EScript engine;
    engine.eval(code1);
    auto mainTable = engine.unit()->block()->symbolTable();

    auto n = mainTable->find(U"n");
    assert(n != nullptr);
    auto record = engine.getObjectRecord(n);
    assert(record != nullptr);
    assert(record->type == SymbolType::Integer);
    assert(Compare::equals_int64(2, record->data));

    auto b = mainTable->find(U"b");
    assert(b != nullptr);
    record = engine.getObjectRecord(b);
    assert(record != nullptr);
    assert(record->type == SymbolType::Integer);
    assert(Compare::equals_int64(9, record->data));
}

void Array_Test::test_arrayShift()
{
    const u32string code1 = UR"(
a = [77,6,9,];
b = a.shift();
n = a.length;
)";
    EScript engine;
    engine.eval(code1);
    auto mainTable = engine.unit()->block()->symbolTable();

    auto n = mainTable->find(U"n");
    assert(n != nullptr);
    auto record = engine.getObjectRecord(n);
    assert(record != nullptr);
    assert(record->type == SymbolType::Integer);
    assert(Compare::equals_int64(2, record->data));

    auto b = mainTable->find(U"b");
    assert(b != nullptr);
    record = engine.getObjectRecord(b);
    assert(record != nullptr);
    assert(record->type == SymbolType::Integer);
    assert(Compare::equals_int64(77, record->data));
}

void Array_Test::test_arraySpliceRemoveAll()
{
    const u32string code1 = UR"(
a = [77,6,9,67,34,89,23,-6,90,11];
b = a.splice(0);
n = a.length;
)";
    EScript engine;
    engine.eval(code1);
    auto mainTable = engine.unit()->block()->symbolTable();

    auto n = mainTable->find(U"n");
    assert(n != nullptr);
    auto record = engine.getObjectRecord(n);
    assert(record != nullptr);
    assert(record->type == SymbolType::Integer);
    assert(Compare::equals_int64(0, record->data));

    auto b = mainTable->find(U"b");
    assert(b != nullptr);
    record = engine.getObjectRecord(b);
    assert(record != nullptr);
    assert(record->type == SymbolType::Array);
    Array *arr = (Array*)record->data;
    assert(arr->get(0).intValue == 77);
    assert(arr->get(1).intValue == 6);
    assert(arr->get(2).intValue == 9);
    assert(arr->get(3).intValue == 67);
    assert(arr->get(4).intValue == 34);
    assert(arr->get(9).intValue == 11);
}

void Array_Test::test_arraySpliceRemoveAllFrom()
{
    const u32string code1 = UR"(
a = [77,6,9,67,34,89,23,-6,90,11];
b = a.splice(3);
n = a.length;
)";
    EScript engine;
    engine.eval(code1);
    auto mainTable = engine.unit()->block()->symbolTable();

    auto n = mainTable->find(U"n");
    assert(n != nullptr);
    auto record = engine.getObjectRecord(n);
    assert(record != nullptr);
    assert(record->type == SymbolType::Integer);
    assert(Compare::equals_int64(3, record->data));

    auto b = mainTable->find(U"b");
    assert(b != nullptr);
    record = engine.getObjectRecord(b);
    assert(record != nullptr);
    assert(record->type == SymbolType::Array);
    Array *arr = (Array*)record->data;
    assert(arr->get(0).intValue == 67);
    assert(arr->get(1).intValue == 34);
    assert(arr->get(2).intValue == 89);
    assert(arr->get(3).intValue == 23);
    assert(arr->get(4).intValue == -6);
    assert(arr->get(6).intValue == 11);
}

void Array_Test::test_arraySpliceRemoveAllFromEnd()
{
    const u32string code1 = UR"(
a = [77,6,9,67,34,89,23,-6,90,11];
b = a.splice(a.length);
n = a.length;
)";
    EScript engine;
    engine.eval(code1);
    auto mainTable = engine.unit()->block()->symbolTable();

    auto n = mainTable->find(U"n");
    assert(n != nullptr);
    auto record = engine.getObjectRecord(n);
    assert(record != nullptr);
    assert(record->type == SymbolType::Integer);
    assert(Compare::equals_int64(10, record->data));

    auto b = mainTable->find(U"b");
    assert(b != nullptr);
    record = engine.getObjectRecord(b);
    assert(record != nullptr);
    assert(record->type == SymbolType::Array);
    Array *arr = (Array*)record->data;
    assert(arr->length() == 0);
}

void Array_Test::test_arraySpliceRemoveNFrom0()
{
    const u32string code1 = UR"(
a = [77,6,9,67,34,89,23,-6,90,11];
b = a.splice(0, 3);
n = a.length;
)";
    EScript engine;
    engine.eval(code1);
    auto mainTable = engine.unit()->block()->symbolTable();

    auto n = mainTable->find(U"n");
    assert(n != nullptr);
    auto record = engine.getObjectRecord(n);
    assert(record != nullptr);
    assert(record->type == SymbolType::Integer);
    assert(Compare::equals_int64(10-3, record->data));

    auto b = mainTable->find(U"b");
    assert(b != nullptr);
    record = engine.getObjectRecord(b);
    assert(record != nullptr);
    assert(record->type == SymbolType::Array);
    Array *arr = (Array*)record->data;
    assert(arr->length() == 3);
    assert(arr->get(0).intValue == 77);
    assert(arr->get(1).intValue == 6);
    assert(arr->get(2).intValue == 9);
}

void Array_Test::test_arraySpliceRemoveNFromPos()
{
    const u32string code1 = UR"(
a = [77,6,9,67,34,89,23,-6,90,11];
b = a.splice(2, 4);
n = a.length;
)";
    EScript engine;
    engine.eval(code1);
    auto mainTable = engine.unit()->block()->symbolTable();

    auto n = mainTable->find(U"n");
    assert(n != nullptr);
    auto record = engine.getObjectRecord(n);
    assert(record != nullptr);
    assert(record->type == SymbolType::Integer);
    assert(Compare::equals_int64(10-4, record->data));

    auto b = mainTable->find(U"b");
    assert(b != nullptr);
    record = engine.getObjectRecord(b);
    assert(record != nullptr);
    assert(record->type == SymbolType::Array);
    Array *arr = (Array*)record->data;
    assert(arr->length() == 4);
    assert(arr->get(0).intValue == 9);
    assert(arr->get(1).intValue == 67);
    assert(arr->get(2).intValue == 34);
    assert(arr->get(3).intValue == 89);
}

void Array_Test::test_arraySpliceRemoveNFromEnd()
{
    const u32string code1 = UR"(
a = [77,6,9,67,34,89,23,-6,90,11];
b = a.splice(a.length, 4);
n = a.length;
)";
    EScript engine;
    engine.eval(code1);
    auto mainTable = engine.unit()->block()->symbolTable();

    auto n = mainTable->find(U"n");
    assert(n != nullptr);
    auto record = engine.getObjectRecord(n);
    assert(record != nullptr);
    assert(record->type == SymbolType::Integer);
    assert(Compare::equals_int64(10, record->data));

    auto b = mainTable->find(U"b");
    assert(b != nullptr);
    record = engine.getObjectRecord(b);
    assert(record != nullptr);
    assert(record->type == SymbolType::Array);
    Array *arr = (Array*)record->data;
    assert(arr->length() == 0);
}

void Array_Test::test_arraySpliceInsertAt0()
{
    const u32string code1 = UR"(
a = [77,6,9,67,34,89,23,-6,90,11];
b = a.splice(0, 0, 1, 2, 3);
n = a.length;
)";
    EScript engine;
    engine.eval(code1);
    auto mainTable = engine.unit()->block()->symbolTable();

    auto n = mainTable->find(U"n");
    assert(n != nullptr);
    auto record = engine.getObjectRecord(n);
    assert(record != nullptr);
    assert(record->type == SymbolType::Integer);
    assert(Compare::equals_int64(13, record->data));

    auto b = mainTable->find(U"b");
    assert(b != nullptr);
    record = engine.getObjectRecord(b);
    assert(record != nullptr);
    assert(record->type == SymbolType::Array);
    Array *arr = (Array*)record->data;
    assert(arr->length() == 0);

    auto a = mainTable->find(U"a");
    assert(a != nullptr);
    record = engine.getObjectRecord(a);
    assert(record != nullptr);
    assert(record->type == SymbolType::Array);
    arr = (Array*)record->data;
    assert(arr->length() == 13);
    assert(arr->get(0).intValue == 1);
    assert(arr->get(1).intValue == 2);
    assert(arr->get(2).intValue == 3);
}

void Array_Test::test_arraySpliceInsertAtN()
{
    const u32string code1 = UR"(
a = [77,6,9,67,34,89,23,-6,90,11];
b = a.splice(3, 0, 1, 2, 3);
n = a.length;
)";
    EScript engine;
    engine.eval(code1);
    auto mainTable = engine.unit()->block()->symbolTable();

    auto n = mainTable->find(U"n");
    assert(n != nullptr);
    auto record = engine.getObjectRecord(n);
    assert(record != nullptr);
    assert(record->type == SymbolType::Integer);
    assert(Compare::equals_int64(13, record->data));

    auto b = mainTable->find(U"b");
    assert(b != nullptr);
    record = engine.getObjectRecord(b);
    assert(record != nullptr);
    assert(record->type == SymbolType::Array);
    Array *arr = (Array*)record->data;
    assert(arr->length() == 0);

    auto a = mainTable->find(U"a");
    assert(a != nullptr);
    record = engine.getObjectRecord(a);
    assert(record != nullptr);
    assert(record->type == SymbolType::Array);
    arr = (Array*)record->data;
    assert(arr->length() == 13);
    assert(arr->get(0).intValue == 77);
    assert(arr->get(2).intValue == 9);
    assert(arr->get(3).intValue == 1);
    assert(arr->get(4).intValue == 2);
    assert(arr->get(5).intValue == 3);
    assert(arr->get(6).intValue == 67);
}

void Array_Test::test_arraySpliceInsertAtEnd()
{
    const u32string code1 = UR"(
a = [77,6,9,67,34,89,23,-6,90,11];
b = a.splice(a.length, 0, 1, 2, 3);
n = a.length;
)";
    EScript engine;
    engine.eval(code1);
    auto mainTable = engine.unit()->block()->symbolTable();

    auto n = mainTable->find(U"n");
    assert(n != nullptr);
    auto record = engine.getObjectRecord(n);
    assert(record != nullptr);
    assert(record->type == SymbolType::Integer);
    assert(Compare::equals_int64(13, record->data));

    auto b = mainTable->find(U"b");
    assert(b != nullptr);
    record = engine.getObjectRecord(b);
    assert(record != nullptr);
    assert(record->type == SymbolType::Array);
    Array *arr = (Array*)record->data;
    assert(arr->length() == 0);

    auto a = mainTable->find(U"a");
    assert(a != nullptr);
    record = engine.getObjectRecord(a);
    assert(record != nullptr);
    assert(record->type == SymbolType::Array);
    arr = (Array*)record->data;
    assert(arr->length() == 13);
    assert(arr->get(0).intValue == 77);
    assert(arr->get(2).intValue == 9);
    assert(arr->get(10).intValue == 1);
    assert(arr->get(11).intValue == 2);
    assert(arr->get(12).intValue == 3);
}

void Array_Test::test_arraySpliceRemoveNInsertFrom0()
{
    const u32string code1 = UR"(
a = [77,6,9,67,34,89,23,-6,90,11];
b = a.splice(0, 4, 1, 2, 3);
n = a.length;
)";
    EScript engine;
    engine.eval(code1);
    auto mainTable = engine.unit()->block()->symbolTable();

    auto n = mainTable->find(U"n");
    assert(n != nullptr);
    auto record = engine.getObjectRecord(n);
    assert(record != nullptr);
    assert(record->type == SymbolType::Integer);
    assert(Compare::equals_int64(10-4+3, record->data));

    auto b = mainTable->find(U"b");
    assert(b != nullptr);
    record = engine.getObjectRecord(b);
    assert(record != nullptr);
    assert(record->type == SymbolType::Array);
    Array *arr = (Array*)record->data;
    assert(arr->length() == 4);
    assert(arr->get(0).intValue == 77);
    assert(arr->get(1).intValue == 6);
    assert(arr->get(2).intValue == 9);
    assert(arr->get(3).intValue == 67);

    auto a = mainTable->find(U"a");
    assert(a != nullptr);
    record = engine.getObjectRecord(a);
    assert(record != nullptr);
    assert(record->type == SymbolType::Array);
    arr = (Array*)record->data;
    assert(arr->length() == 10-4+3);
    assert(arr->get(0).intValue == 1);
    assert(arr->get(1).intValue == 2);
    assert(arr->get(2).intValue == 3);
    assert(arr->get(3).intValue == 34);
}

void Array_Test::test_arraySpliceRemoveNInsertFromPos()
{
    const u32string code1 = UR"(
a = [77,6,9,67,34,89,23,-6,90,11];
b = a.splice(2, 4, 1, 2, 3);
n = a.length;
)";
    EScript engine;
    engine.eval(code1);
    auto mainTable = engine.unit()->block()->symbolTable();

    auto n = mainTable->find(U"n");
    assert(n != nullptr);
    auto record = engine.getObjectRecord(n);
    assert(record != nullptr);
    assert(record->type == SymbolType::Integer);
    assert(Compare::equals_int64(10-4+3, record->data));

    auto b = mainTable->find(U"b");
    assert(b != nullptr);
    record = engine.getObjectRecord(b);
    assert(record != nullptr);
    assert(record->type == SymbolType::Array);
    Array *arr = (Array*)record->data;
    assert(arr->length() == 4);
    assert(arr->get(0).intValue == 9);
    assert(arr->get(1).intValue == 67);
    assert(arr->get(2).intValue == 34);
    assert(arr->get(3).intValue == 89);

    auto a = mainTable->find(U"a");
    assert(a != nullptr);
    record = engine.getObjectRecord(a);
    assert(record != nullptr);
    assert(record->type == SymbolType::Array);
    arr = (Array*)record->data;
    assert(arr->length() == 10-4+3);
    assert(arr->get(2).intValue == 1);
    assert(arr->get(3).intValue == 2);
    assert(arr->get(4).intValue == 3);
    assert(arr->get(5).intValue == 23);
}

void Array_Test::test_arraySpliceRemoveNInsertFromEnd()
{ // чтобы неполный диапазон - с конца-1, но не 4 штуки
    const u32string code1 = UR"(
a = [77,6,9,67,34,89,23,-6,90,11];
b = a.splice(a.length-1, 4, 1, 2, 3);
n = a.length;
)";
    EScript engine;
    engine.eval(code1);
    auto mainTable = engine.unit()->block()->symbolTable();

    auto n = mainTable->find(U"n");
    assert(n != nullptr);
    auto record = engine.getObjectRecord(n);
    assert(record != nullptr);
    assert(record->type == SymbolType::Integer);
    assert(Compare::equals_int64(10-1+3, record->data));

    auto b = mainTable->find(U"b");
    assert(b != nullptr);
    record = engine.getObjectRecord(b);
    assert(record != nullptr);
    assert(record->type == SymbolType::Array);
    Array *arr = (Array*)record->data;
    assert(arr->length() == 1);
    assert(arr->get(0).intValue == 11);

    auto a = mainTable->find(U"a");
    assert(a != nullptr);
    record = engine.getObjectRecord(a);
    assert(record != nullptr);
    assert(record->type == SymbolType::Array);
    arr = (Array*)record->data;
    assert(arr->length() == 10-1+3);
    assert(arr->get(9).intValue == 1);
    assert(arr->get(10).intValue == 2);
    assert(arr->get(11).intValue == 3);
}

void Array_Test::test_arrayReverse()
{
    const u32string code1 = UR"(
a = [77,6,9,67,34];
b = a.reverse();
n = b.length;
)";
    EScript engine;
    engine.eval(code1);
    auto mainTable = engine.unit()->block()->symbolTable();

    auto n = mainTable->find(U"n");
    assert(n != nullptr);
    auto record = engine.getObjectRecord(n);
    assert(record != nullptr);
    assert(record->type == SymbolType::Integer);
    assert(Compare::equals_int64(5, record->data));

    auto b = mainTable->find(U"b");
    assert(b != nullptr);
    record = engine.getObjectRecord(b);
    assert(record != nullptr);
    assert(record->type == SymbolType::Array);
    Array *arrB = (Array*)record->data;
    assert(arrB->length() == 5);
    assert(arrB->get(0).intValue == 34);
    assert(arrB->get(2).intValue == 9);
    assert(arrB->get(4).intValue == 77);

    auto a = mainTable->find(U"a");
    assert(a != nullptr);
    record = engine.getObjectRecord(a);
    assert(record != nullptr);
    assert(record->type == SymbolType::Array);
    Array *arrA = (Array*)record->data;
    assert(arrA->length() == arrB->length());
    assert(arrB->get(0).intValue == 34);
    assert(arrB->get(2).intValue == 9);
    assert(arrB->get(4).intValue == 77);

    assert(arrA == arrB); // один и тот же объект

    for (int i = 0; i < arrA->length(); i++) {
        assert(arrA->get(i) == arrB->get(i));
    }
}

void Array_Test::test_arrayFill()
{
    const u32string code1 = UR"(
a = [77,6,9,67,34];
b = a.fill(45, 2, 10); // --> [77,6,45,45,45]
n = b.length;
)";
    EScript engine;
    engine.eval(code1);
    auto mainTable = engine.unit()->block()->symbolTable();

    auto n = mainTable->find(U"n");
    assert(n != nullptr);
    auto record = engine.getObjectRecord(n);
    assert(record != nullptr);
    assert(record->type == SymbolType::Integer);
    assert(Compare::equals_int64(5, record->data));

    auto b = mainTable->find(U"b");
    assert(b != nullptr);
    record = engine.getObjectRecord(b);
    assert(record != nullptr);
    assert(record->type == SymbolType::Array);
    Array *arrB = (Array*)record->data;
    assert(arrB->length() == 5);
    assert(arrB->get(0).intValue == 77);
    assert(arrB->get(2).intValue == 45);
    assert(arrB->get(4).intValue == 45);

    auto a = mainTable->find(U"a");
    assert(a != nullptr);
    record = engine.getObjectRecord(a);
    assert(record != nullptr);
    assert(record->type == SymbolType::Array);
    Array *arrA = (Array*)record->data;
    assert(arrA->length() == arrB->length());

    assert(arrA == arrB); // один и тот же объект

    for (int i = 0; i < arrA->length(); i++) {
        assert(arrA->get(i) == arrB->get(i));
    }
}

void Array_Test::test_arraySort()
{
    const u32string code1 = UR"(
a = [77,6,9,67,34];
b = a.sort(); // --> [6,9,34,67,77]
n = b.length;
)";
    EScript engine;
    engine.eval(code1);
    auto mainTable = engine.unit()->block()->symbolTable();

    auto n = mainTable->find(U"n");
    assert(n != nullptr);
    auto record = engine.getObjectRecord(n);
    assert(record != nullptr);
    assert(record->type == SymbolType::Integer);
    assert(Compare::equals_int64(5, record->data));

    auto b = mainTable->find(U"b");
    assert(b != nullptr);
    record = engine.getObjectRecord(b);
    assert(record != nullptr);
    assert(record->type == SymbolType::Array);
    Array *arrB = (Array*)record->data;
    assert(arrB->length() == 5);
    assert(arrB->get(0).intValue == 6);
    assert(arrB->get(1).intValue == 9);
    assert(arrB->get(2).intValue == 34);
    assert(arrB->get(3).intValue == 67);
    assert(arrB->get(4).intValue == 77);

    auto a = mainTable->find(U"a");
    assert(a != nullptr);
    record = engine.getObjectRecord(a);
    assert(record != nullptr);
    assert(record->type == SymbolType::Array);
    Array *arrA = (Array*)record->data;
    assert(arrA->length() == arrB->length());

    assert(arrA == arrB); // один и тот же объект

    for (int i = 0; i < arrA->length(); i++) {
        assert(arrA->get(i) == arrB->get(i));
    }
}

void Array_Test::test_arrayUnshift()
{
    const u32string code1 = UR"(
a = [77,6,9,67,34];
b = a.unshift(12,34,56); // --> [12,24,56,77,6,9,67,34]
n = a.length;
)";
    EScript engine;
    engine.eval(code1);
    auto mainTable = engine.unit()->block()->symbolTable();

    auto n = mainTable->find(U"n");
    assert(n != nullptr);
    auto record = engine.getObjectRecord(n);
    assert(record != nullptr);
    assert(record->type == SymbolType::Integer);
    assert(Compare::equals_int64(8, record->data));

    auto b = mainTable->find(U"b");
    assert(b != nullptr);
    record = engine.getObjectRecord(b);
    assert(record != nullptr);
    assert(record->type == SymbolType::Integer);
    assert(Compare::equals_int64(8, record->data));

    auto a = mainTable->find(U"a");
    assert(a != nullptr);
    record = engine.getObjectRecord(a);
    assert(record != nullptr);
    assert(record->type == SymbolType::Array);
    Array *arrA = (Array*)record->data;
    assert(arrA->get(0).intValue == 12);
    assert(arrA->get(1).intValue == 34);
    assert(arrA->get(2).intValue == 56);
    assert(arrA->get(3).intValue == 77);
    assert(arrA->get(arrA->length()-1).intValue == 34);
}

void Array_Test::test_arraySlice()
{
    const u32string code1 = UR"(
a = [77,6,9,67,34];
b = a.slice(1, 4); // --> [6,9,67]
n = a.length;
)";
    EScript engine;
    engine.eval(code1);
    auto mainTable = engine.unit()->block()->symbolTable();

    auto n = mainTable->find(U"n");
    assert(n != nullptr);
    auto record = engine.getObjectRecord(n);
    assert(record != nullptr);
    assert(record->type == SymbolType::Integer);
    assert(Compare::equals_int64(5, record->data));

    auto b = mainTable->find(U"b");
    assert(b != nullptr);
    record = engine.getObjectRecord(b);
    assert(record != nullptr);
    assert(record->type == SymbolType::Array);
    Array *arrB = (Array*)record->data;
    assert(arrB->length() == 3);
    assert(arrB->get(0).intValue == 6);
    assert(arrB->get(1).intValue == 9);
    assert(arrB->get(2).intValue == 67);

    auto a = mainTable->find(U"a");
    assert(a != nullptr);
    record = engine.getObjectRecord(a);
    assert(record != nullptr);
    assert(record->type == SymbolType::Array);
    Array *arrA = (Array*)record->data;
    assert(arrA->length() == 5);
}

void Array_Test::test_arrayJoin()
{
    const u32string code1 = UR"(
a = ["hello", "world", 1, 2, 3];
x = a.join("*_*"); // -> 'hello*_*world*_*1*_*2*_*3'
)";
    EScript engine;
    engine.setShowDisassembleListing(false);
    engine.setShowTCode(false);
    engine.eval(code1);

    auto mainTable = engine.unit()->block()->symbolTable();
    auto x = mainTable->find(U"x");
    auto record = engine.getObjectRecord(x);
    assert(record->type == SymbolType::String);
    assert(U"hello*_*world*_*1*_*2*_*3" == ((StringObject*)record->data)->toString());
}

void Array_Test::test_arrayFilter()
{
    const u32string code1 = UR"(
function fnGreater(x) { return x > 3; }

a = [1,2,3,4,5,6,7];
b = a.filter(fnGreater); // --> [4,5,6,7]
n = b.length;
)";
    EScript engine;
    engine.eval(code1);
    auto mainTable = engine.unit()->block()->symbolTable();

    auto n = mainTable->find(U"n");
    assert(n != nullptr);
    auto record = engine.getObjectRecord(n);
    assert(record != nullptr);
    assert(record->type == SymbolType::Integer);
    assert(Compare::equals_int64(4, record->data));

    auto b = mainTable->find(U"b");
    assert(b != nullptr);
    record = engine.getObjectRecord(b);
    assert(record != nullptr);
    assert(record->type == SymbolType::Array);
    Array *arrB = (Array*)record->data;
    assert(arrB->length() == 4);
    assert(arrB->get(0).intValue == 4);
    assert(arrB->get(1).intValue == 5);
    assert(arrB->get(2).intValue == 6);
    assert(arrB->get(3).intValue == 7);
}

void Array_Test::test_arrayFirstIndex()
{
    const u32string code1 = UR"(
a = [1,2,3,4,5,6,7];
n = a.firstIndex(4);
x = a.firstIndex(11);
)";
    EScript engine;
    engine.eval(code1);
    auto mainTable = engine.unit()->block()->symbolTable();

    auto n = mainTable->find(U"n");
    assert(n != nullptr);
    auto record = engine.getObjectRecord(n);
    assert(record != nullptr);
    assert(record->type == SymbolType::Integer);
    assert(Compare::equals_int64(3, record->data));

    auto x = mainTable->find(U"x");
    assert(x != nullptr);
    record = engine.getObjectRecord(x);
    assert(record != nullptr);
    assert(record->type == SymbolType::Integer);
    assert(Compare::equals_int64(-1, record->data));
}

void Array_Test::test_arrayLastIndex()
{
    const u32string code1 = UR"(
a = [1,2,3,4,5,4,6,7];
n = a.lastIndex(4); // ==5
x = a.lastIndex(4, -4); // ==3
y = a.lastIndex(11, -40); // -1
z = a.lastIndex(4, -40); // ==-1
t = a.lastIndex(8); // ==-1
k = a.lastIndex(4, 4); // ==3
)";
    EScript engine;
    engine.eval(code1);
    auto mainTable = engine.unit()->block()->symbolTable();

    auto n = mainTable->find(U"n");
    assert(n != nullptr);
    auto record = engine.getObjectRecord(n);
    assert(record != nullptr);
    assert(record->type == SymbolType::Integer);
    assert(Compare::equals_int64(5, record->data));

    auto x = mainTable->find(U"x");
    assert(x != nullptr);
    record = engine.getObjectRecord(x);
    assert(record != nullptr);
    assert(record->type == SymbolType::Integer);
    assert(Compare::equals_int64(3, record->data));

    auto y = mainTable->find(U"y");
    assert(y != nullptr);
    record = engine.getObjectRecord(y);
    assert(record != nullptr);
    assert(record->type == SymbolType::Integer);
    assert(Compare::equals_int64(-1, record->data));

    auto z = mainTable->find(U"z");
    assert(z != nullptr);
    record = engine.getObjectRecord(z);
    assert(record != nullptr);
    assert(record->type == SymbolType::Integer);
    assert(Compare::equals_int64(-1, record->data));

    auto t = mainTable->find(U"t");
    assert(t != nullptr);
    record = engine.getObjectRecord(t);
    assert(record != nullptr);
    assert(record->type == SymbolType::Integer);
    assert(Compare::equals_int64(-1, record->data));

    auto k = mainTable->find(U"k");
    assert(k != nullptr);
    record = engine.getObjectRecord(k);
    assert(record != nullptr);
    assert(record->type == SymbolType::Integer);
    assert(Compare::equals_int64(3, record->data));
}

void Array_Test::test_arrayIncludes()
{
    const u32string code1 = UR"(
a = [1,2,3,4,5,6,7];
n = a.includes(4);//true
x = a.includes(11);//false
y = a.includes(5, 2);//true
z = a.includes(5, 5);//false
t = a.includes(5, -5);//true
)";
    EScript engine;
    engine.eval(code1);
    auto mainTable = engine.unit()->block()->symbolTable();

    auto n = mainTable->find(U"n");
    assert(n != nullptr);
    auto record = engine.getObjectRecord(n);
    assert(record != nullptr);
    assert(record->type == SymbolType::Boolean);
    assert(Compare::equals_bool(true, record->data));

    auto x = mainTable->find(U"x");
    assert(x != nullptr);
    record = engine.getObjectRecord(x);
    assert(record != nullptr);
    assert(record->type == SymbolType::Boolean);
    assert(Compare::equals_bool(false, record->data));

    auto y = mainTable->find(U"y");
    record = engine.getObjectRecord(y);
    assert(record != nullptr);
    assert(record->type == SymbolType::Boolean);
    assert(Compare::equals_bool(true, record->data));

    auto z = mainTable->find(U"z");
    record = engine.getObjectRecord(z);
    assert(record != nullptr);
    assert(record->type == SymbolType::Boolean);
    assert(Compare::equals_bool(false, record->data));

    auto t = mainTable->find(U"t");
    record = engine.getObjectRecord(t);
    assert(record != nullptr);
    assert(record->type == SymbolType::Boolean);
    assert(Compare::equals_bool(true, record->data));
}

void Array_Test::test_appendWithSplice()
{
    // для этого start=length, toRemove=0
    const u32string code1 = UR"(
a = [77,6,9,67];
a.splice(4,0,34,89,23,-6,90,11);
n = a.length;
)";
    EScript engine;
    engine.eval(code1);
    auto mainTable = engine.unit()->block()->symbolTable();

    auto n = mainTable->find(U"n");
    assert(n != nullptr);
    auto record = engine.getObjectRecord(n);
    assert(record != nullptr);
    assert(record->type == SymbolType::Integer);
    assert(Compare::equals_int64(10, record->data));

    auto a = mainTable->find(U"a");
    assert(a != nullptr);
    record = engine.getObjectRecord(a);
    assert(record != nullptr);
    assert(record->type == SymbolType::Array);
    Array *arr = (Array*)record->data;
    assert(arr->get(0).intValue == 77);
    assert(arr->get(1).intValue == 6);
    assert(arr->get(2).intValue == 9);
    assert(arr->get(3).intValue == 67);
    assert(arr->get(4).intValue == 34);
    assert(arr->get(9).intValue == 11);
}

void Array_Test::test_arrayForEach()
{
    // массив не должен измениться, выполняется некая функция и что-то делает/меняет, но не сам массив
    const u32string code1 = UR"(
t = 1;
a = [1,2,3,4];
function sumt(x) { return t += x; }
a.forEach(sumt);
// здесь t == 11
n = a.length;
)";
    EScript engine;
    engine.eval(code1);
    auto mainTable = engine.unit()->block()->symbolTable();

    auto n = mainTable->find(U"n");
    assert(n != nullptr);
    auto record = engine.getObjectRecord(n);
    assert(record != nullptr);
    assert(record->type == SymbolType::Integer);
    assert(Compare::equals_int64(4, record->data));

    auto t = mainTable->find(U"t");
    assert(t != nullptr);
    record = engine.getObjectRecord(t);
    assert(record != nullptr);
    assert(record->type == SymbolType::Integer);
    assert(Compare::equals_int64(11, record->data));

    auto a = mainTable->find(U"a");
    assert(a != nullptr);
    record = engine.getObjectRecord(a);
    assert(record != nullptr);
    assert(record->type == SymbolType::Array);
    Array *arr = (Array*)record->data;
    assert(arr->get(0).intValue == 1);
    assert(arr->get(1).intValue == 2);
    assert(arr->get(2).intValue == 3);
    assert(arr->get(3).intValue == 4);
}

void Array_Test::test_arrayMap()
{
    const u32string code1 = UR"(
a = [1,2,3,4];
function mul2(x) { return 2*x; }
b = a.map(mul2);
c = a.map(mul2);//ещё раз для лишней работы
n = a.length;
)";
    EScript engine;
    engine.eval(code1);
    auto mainTable = engine.unit()->block()->symbolTable();

    // не должен измениться
    auto n = mainTable->find(U"n");
    assert(n != nullptr);
    auto record = engine.getObjectRecord(n);
    assert(record != nullptr);
    assert(record->type == SymbolType::Integer);
    assert(Compare::equals_int64(4, record->data));

    auto a = mainTable->find(U"a");
    assert(a != nullptr);
    record = engine.getObjectRecord(a);
    assert(record != nullptr);
    assert(record->type == SymbolType::Array);
    Array *arrA = (Array*)record->data;
    assert(arrA->get(0).intValue == 1);
    assert(arrA->get(1).intValue == 2);
    assert(arrA->get(2).intValue == 3);
    assert(arrA->get(3).intValue == 4);

    auto b = mainTable->find(U"b");
    assert(b != nullptr);
    record = engine.getObjectRecord(b);
    assert(record != nullptr);
    assert(record->type == SymbolType::Array);
    auto arrB = (Array*)record->data;
    assert(arrB->get(0).intValue == 2);
    assert(arrB->get(1).intValue == 4);
    assert(arrB->get(2).intValue == 6);
    assert(arrB->get(3).intValue == 8);

    assert(arrA != arrB);
}

void Array_Test::test_arraySome()
{
    const u32string code1 = UR"(
a = [1,2,3,4];
function even(x) { return x % 2 == 0; }
function notfound(x) { return x == 0; }
x = a.some(even);
y = a.some(notfound);
)";
    EScript engine;
    engine.setShowTCode(false);
    engine.setShowDisassembleListing(false);
    engine.eval(code1);
    auto mainTable = engine.unit()->block()->symbolTable();

    auto x = mainTable->find(U"x");
    assert(x != nullptr);
    auto record = engine.getObjectRecord(x);
    assert(record != nullptr);
    assert(record->type == SymbolType::Boolean);
    assert(Compare::equals_bool(true, record->data));

    auto y = mainTable->find(U"y");
    assert(y != nullptr);
    record = engine.getObjectRecord(y);
    assert(record != nullptr);
    assert(record->type == SymbolType::Boolean);
    assert(Compare::equals_bool(false, record->data));
}

void Array_Test::test_arrayEvery()
{
    const u32string code1 = UR"(
a = [2,2,2,2];
function ever(x) { return x == 2; }
function notfound(x) { return x == 0; }
x = a.every(ever);
y = a.every(notfound);
)";
    EScript engine;
    engine.setShowTCode(false);
    engine.setShowDisassembleListing(false);
    engine.eval(code1);
    auto mainTable = engine.unit()->block()->symbolTable();

    auto x = mainTable->find(U"x");
    assert(x != nullptr);
    auto record = engine.getObjectRecord(x);
    assert(record != nullptr);
    assert(record->type == SymbolType::Boolean);
    assert(Compare::equals_bool(true, record->data));

    auto y = mainTable->find(U"y");
    assert(y != nullptr);
    record = engine.getObjectRecord(y);
    assert(record != nullptr);
    assert(record->type == SymbolType::Boolean);
    assert(Compare::equals_bool(false, record->data));
}
