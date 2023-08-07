#include "stdafx.h"
#include "forstatement_test.h"

#include "compare.h"

using namespace escript;
using namespace std;

void ForStatement_Test::run()
{
    initTestCase();
    test_for();
    cleanupTestCase();
}

void ForStatement_Test::initTestCase()
{

}

void ForStatement_Test::cleanupTestCase()
{

}

void ForStatement_Test::test_for()
{
    const u32string code1 = U"for (i = 0; i < 10; i = i + 1) {i = i + 1;}";
    EScript engine;
    engine.eval(code1);
    auto mainTable = engine.unit()->block()->symbolTable();
    auto i = mainTable->find(U"i");
    auto record = engine.getObjectRecord(i);
    assert(record->type == SymbolType::Integer);
    assert(Compare::equals_int64(10, record->data));

    const u32string code2 = U"x = 1; for (i = 1; i < 10; i = i + 1) {x = x * i;}";
    engine.eval(code2);
    mainTable = engine.unit()->block()->symbolTable();
    auto x = mainTable->find(U"x");
    record = engine.getObjectRecord(x);
    assert(record->type == SymbolType::Integer);
    assert(Compare::equals_int64(1*2*3*4*5*6*7*8*9, record->data));
}
