#include "stdafx.h"
#include "functions_test.h"


void Functions_Test::run()
{
    initTestCase();
    test_byReference();
    test_recursionFactorial();
    test_funcAlias();
    test_funcAliasAssign();
    test_functionAsParameter();
    test_unnamedFuncAssign();
    test_nestedFunction();
    test_nestedFunctionVariable();
    test_functionReturnsFunction();
    test_function2ReturnsFunction();
    test_functionFactory();
    test_arrayOfFunctions();
    test_funcCallFormsArrayItem();
    test_funcCallFormsOfFuncResult();
    cleanupTestCase();
}

void Functions_Test::initTestCase()
{

}

void Functions_Test::cleanupTestCase()
{

}

void Functions_Test::test_byReference()
{
    const u32string code1 = U"a = [6,5,4]; function fnref(x) { x[0] = 8; } fnref(a); y = a[0];";
    EScript engine;
    engine.setShowDisassembleListing(false);
    engine.setShowTCode(false);
    engine.eval(code1);
    auto mainTable = engine.unit()->block()->symbolTable();
    auto y = mainTable->find(U"y");
    auto record = engine.getObjectRecord(y);
    assert(record->type == SymbolType::Integer);
    assert(Compare::equals_int64(8, record->data));
}

void Functions_Test::test_recursionFactorial()
{
    const u32string code1 = U"function factorial(i) { if (i == 0) return 1; return i * factorial(i - 1); } y = factorial(10);";
    EScript engine;
    engine.setShowDisassembleListing(false);
    engine.setShowTCode(false);
    //stringstream ss;
    //engine.setOutStream(ss);
    engine.eval(code1);
    auto mainTable = engine.unit()->block()->symbolTable();
    auto y = mainTable->find(U"y");
    auto record = engine.getObjectRecord(y);
    assert(record->type == SymbolType::Integer);
    assert(Compare::equals_int64(3628800, record->data));
}

void Functions_Test::test_funcAlias()
{ // пока не нужно

    const u32string code1 =
U"fnRef = function func1(i) { return 123+i; };"
"x = func1(7);"
 "y = fnRef(45);";
    EScript engine;
    engine.setShowDisassembleListing(false);
    engine.setShowTCode(false);
    engine.eval(code1);
    auto mainTable = engine.unit()->block()->symbolTable();
    auto x = mainTable->find(U"x");
    auto record = engine.getObjectRecord(x);
    assert(record->type == SymbolType::Integer);
    assert(Compare::equals_int64(123+7, record->data));

    auto y = mainTable->find(U"y");
    record = engine.getObjectRecord(y);
    assert(record->type == SymbolType::Integer);
    assert(Compare::equals_int64(123+45, record->data));
}

void Functions_Test::test_funcAliasAssign()
{
    const u32string code1 =
U"function func1(i) { return 123+i; }"
"fnRef = func1;"
 "y = fnRef(45);";
    EScript engine;
    engine.setShowDisassembleListing(false);
    engine.setShowTCode(false);
    engine.eval(code1);
    auto mainTable = engine.unit()->block()->symbolTable();
    auto y = mainTable->find(U"y");
    auto record = engine.getObjectRecord(y);
    assert(record->type == SymbolType::Integer);
    assert(Compare::equals_int64(123+45, record->data));
}

void Functions_Test::test_functionAsParameter()
{
    const std::string macro1 = R"(
function testFunc(x) {
    return 2 * x;
}
func = testFunc;
function fnTest(pFn, x) {
    return pFn(x);
}

y = fnTest(func, 12);

)";
    const u32string code1 = to_u32string(macro1);
    EScript engine;
    engine.setShowDisassembleListing(false);
    engine.setShowTCode(false);
    engine.eval(code1);
    auto mainTable = engine.unit()->block()->symbolTable();
    auto y = mainTable->find(U"y");
    auto record = engine.getObjectRecord(y);
    assert(record->type == SymbolType::Integer);
    assert(Compare::equals_int64(12*2, record->data));

    const u32string code2 = U"z = fnTest(testFunc, 14);";
    engine.eval(code2);
    auto z = mainTable->find(U"z");
    record = engine.getObjectRecord(z);
    assert(record->type == SymbolType::Integer);
    assert(Compare::equals_int64(14*2, record->data));
}

void Functions_Test::test_unnamedFuncAssign()
{
    const std::string macro1 = R"(
testFunc = function(x) {
    return 2 * x;
};

y = testFunc(7);

)";
    const u32string code1 = to_u32string(macro1);
    EScript engine;
    engine.setShowDisassembleListing(false);
    engine.setShowTCode(false);
    engine.eval(code1);
    auto mainTable = engine.unit()->block()->symbolTable();
    auto y = mainTable->find(U"y");
    auto record = engine.getObjectRecord(y);
    assert(record->type == SymbolType::Integer);
    assert(Compare::equals_int64(14, record->data));
}

void Functions_Test::test_functionReturnsFunction()
{
    const std::u32string code1 = UR"(
function makeFunc(a) {
    function fn3(x) { return 3 * x; };
    return fn3;
};

fn = makeFunc('none');
y = fn(5);

)";
    EScript engine;
    engine.setShowDisassembleListing(false);
    engine.setShowTCode(false);
    engine.eval(code1);
    auto mainTable = engine.unit()->block()->symbolTable();
    auto y = mainTable->find(U"y");
    auto record = engine.getObjectRecord(y);
    assert(record->type == SymbolType::Integer);
    assert(Compare::equals_int64(15, record->data));
}

void Functions_Test::test_nestedFunction()
{
    const std::u32string code1 = UR"(
function makeFunc(a) {
    function fn3(x) { return 3 * x; };
    return fn3(a);
};

y = makeFunc(5);

)";
    EScript engine;
    engine.setShowDisassembleListing(false);
    engine.setShowTCode(false);
    engine.eval(code1);
    auto mainTable = engine.unit()->block()->symbolTable();
    auto y = mainTable->find(U"y");
    auto record = engine.getObjectRecord(y);
    assert(record->type == SymbolType::Integer);
    assert(Compare::equals_int64(15, record->data));
}

void Functions_Test::test_nestedFunctionVariable()
{
    const std::u32string code1 = UR"(
function makeFunc(a) {
    fn3 = function (x) { return 3 * x; };
    return fn3(a);
};

y = makeFunc(5);

)";
    EScript engine;
    engine.setShowDisassembleListing(false);
    engine.setShowTCode(false);
    engine.eval(code1);
    auto mainTable = engine.unit()->block()->symbolTable();
    auto y = mainTable->find(U"y");
    auto record = engine.getObjectRecord(y);
    assert(record->type == SymbolType::Integer);
    assert(Compare::equals_int64(15, record->data));
}

void Functions_Test::test_function2ReturnsFunction()
{
    const std::u32string code1 = UR"(
// create a function
makeFunc = function(name) {
    t1 = function(x) { return 3 * x; };
    t2 = function(x) { return 5 * x; };
    switch (name) {
        case "three":
            return t1;
        default:
            return t2;
    }
};

x = makeFunc("three");
y = x(5);

)";
    EScript engine;
    engine.setShowDisassembleListing(false);
    engine.setShowTCode(false);
    engine.eval(code1);
    auto mainTable = engine.unit()->block()->symbolTable();
    auto y = mainTable->find(U"y");
    auto record = engine.getObjectRecord(y);
    assert(record->type == SymbolType::Integer);
    assert(Compare::equals_int64(15, record->data));
}

void Functions_Test::test_functionFactory()
{
    const std::u32string code1 = UR"(
// create a function
factory = function(name) {
    switch (name) {
        case 3:
            return function(x) { return 3 * x; };
        case 5:
            return function(x) { return 5 * x; };
        default:
            break;
    }
    return function(x) { return x; };
};

func = factory(5);
y = func(12);

)";
    EScript engine;
    engine.setShowDisassembleListing(false);
    engine.setShowTCode(false);
    engine.eval(code1);
    auto mainTable = engine.unit()->block()->symbolTable();
    auto y = mainTable->find(U"y");
    auto record = engine.getObjectRecord(y);
    assert(record->type == SymbolType::Integer);
    assert(Compare::equals_int64(60, record->data));
}

void Functions_Test::test_arrayOfFunctions()
{
    const std::u32string code1 = UR"(
function fn1(x) { return x; }
function fn2(x) { return 2*x; }
function fn3(x) { return 3*x; }

arr = [fn1, fn2, fn3];
x = 7;
a0 = arr[0];
a = a0(x);

)";
    EScript engine;
    engine.eval(code1);
    auto mainTable = engine.unit()->block()->symbolTable();
    auto a = mainTable->find(U"a");
    auto record = engine.getObjectRecord(a);
    assert(record->type == SymbolType::Integer);
    assert(Compare::equals_int64(7, record->data));
}

void Functions_Test::test_funcCallFormsArrayItem()
{
    const std::u32string code1 = UR"(
function fn2(x) { return x*x; }

arr = [fn2];
a = arr[0](7);


)";
    EScript engine;
    engine.eval(code1);
    auto mainTable = engine.unit()->block()->symbolTable();
    auto a = mainTable->find(U"a");
    auto record = engine.getObjectRecord(a);
    assert(record->type == SymbolType::Integer);
    assert(Compare::equals_int64(7*7, record->data));
}

void Functions_Test::test_funcCallFormsOfFuncResult()
{
    const std::u32string code1 = UR"(
function fn2(x) { return x*x; }

function fn() {
    return fn2;
}

a = fn()(7); // == 7*7
)";
    EScript engine;
    engine.eval(code1);
    auto mainTable = engine.unit()->block()->symbolTable();
    auto a = mainTable->find(U"a");
    auto record = engine.getObjectRecord(a);
    assert(record->type == SymbolType::Integer);
    assert(Compare::equals_int64(7*7, record->data));
}

