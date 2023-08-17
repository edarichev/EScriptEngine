#include "stdafx.h"
#include "algo_test.h"



void Algo_Test::run()
{
    initTestCase();
    test_sortInt();
    test_sortReal();
    test_bsearchRecursive();
    cleanupTestCase();
}

void Algo_Test::initTestCase()
{

}

void Algo_Test::cleanupTestCase()
{

}

void Algo_Test::test_sortInt()
{
    const std::string prog1 = R"(a=[15,22,9,0,34,7,87,1,33,2,14];
for (i = 0; i < a.length - 1; i++) {
    for (j = i+1; j < a.length; j++) {
        x = a[i];
        y = a[j];
        if (a[j] < a[i]) {
            tmp = a[j];
            a[j] = a[i];
            a[i] = tmp;
        }
    }
}

console.log(a);

)";
    const std::u32string code1 = to_u32string(prog1);
    EScript engine;
    stringstream ss;
    engine.setOutStream(ss);
    engine.setShowDisassembleListing(false);
    engine.setShowTCode(false);
    engine.eval(code1);
    auto mainTable = engine.unit()->block()->symbolTable();
    auto a = mainTable->find(U"a");
    auto record = engine.getObjectRecord(a);
    assert(record->type == SymbolType::Array);
    string s1 = "[0, 1, 2, 7, 9, 14, 15, 22, 33, 34, 87]" + eol;
    assert(s1 == ss.str());
    ss.clear();
    ss.str("");
}

void Algo_Test::test_sortReal()
{
    const std::string prog1 = R"(a=[15.1,22,9,0,34.4,7,87,1.2,33,2,14];
for (i = 0; i < a.length - 1; i++) {
    for (j = i+1; j < a.length; j++) {
        x = a[i];
        y = a[j];
        if (a[j] < a[i]) {
            tmp = a[j];
            a[j] = a[i];
            a[i] = tmp;
        }
    }
}

console.log(a);

)";
    const std::u32string code1 = to_u32string(prog1);
    EScript engine;
    stringstream ss;
    engine.setOutStream(ss);
    engine.setShowDisassembleListing(false);
    engine.setShowTCode(false);
    engine.eval(code1);
    auto mainTable = engine.unit()->block()->symbolTable();
    auto a = mainTable->find(U"a");
    auto record = engine.getObjectRecord(a);
    assert(record->type == SymbolType::Array);
    string s1 = "[0, 1.2, 2, 7, 9, 14, 15.1, 22, 33, 34.4, 87]" + eol;
    assert(s1 == ss.str());
    ss.clear();
    ss.str("");
}

void Algo_Test::test_bsearchRecursive()
{
    const std::string prog1 = R"(
arr = [1, 3, 5, 7, 8, 9];
v = 5;
function bsearch_index(arr, x, start, end) {
    if (start > end) return -1;
    var mid=((start + end)/2);
    if (arr[mid]==x) return mid;
    if(arr[mid] > x)
        return bsearch_index(arr, x, start, mid-1);
    else
        return bsearch_index(arr, x, mid+1, end);
}
i = bsearch_index(arr, v, 0, arr.length-1);

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
    assert(Compare::equals_int64(2, record->data));
}
