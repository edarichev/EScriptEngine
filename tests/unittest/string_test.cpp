#include "stdafx.h"
#include "string_test.h"

void String_Test::run()
{
    initTestCase();
    test_stringLiteral();
    cleanupTestCase();
}

void String_Test::initTestCase()
{

}

void String_Test::cleanupTestCase()
{

}

void String_Test::test_stringLiteral()
{
    const u32string code1 = U"s = \"hello\";";
    EScript engine;
    engine.eval(code1);
    auto mainTable = engine.unit()->block()->symbolTable();
    auto s = mainTable->find(U"s");
    auto record = engine.getObjectRecord(s);
    assert(record->type == SymbolType::String);
    assert(Compare::equals_int64(0, record->data));
    auto str = engine.getUString(0);
    assert(str == U"hello");
}
