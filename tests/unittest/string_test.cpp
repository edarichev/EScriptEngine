#include "stdafx.h"
#include "string_test.h"

void String_Test::run()
{
    initTestCase();
    test_stringLiteral();
    test_stringPlus();
    test_stringLength();
    test_stringAt();
    test_stringSubstring();
    test_callMethodOfStringLiteral();
    test_callMethodOfBQStringLiteral();
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
    auto str = (StringObject*)record->data;
    assert(*str == U"hello");
}

void String_Test::test_stringPlus()
{
    const u32string code1 = U"s = \"hello\" + ', world';";
    EScript engine;
    engine.eval(code1);
    auto mainTable = engine.unit()->block()->symbolTable();
    auto s = mainTable->find(U"s");
    auto record = engine.getObjectRecord(s);
    assert(record->type == SymbolType::String);
    auto str = (StringObject*)record->data;
    assert(*str == U"hello, world");
}

void String_Test::test_stringLength()
{
    const u32string code1 = U"s = \"hello\"; i = s.length;";
    EScript engine;
    engine.eval(code1);
    auto mainTable = engine.unit()->block()->symbolTable();
    auto i = mainTable->find(U"i");
    auto record = engine.getObjectRecord(i);
    assert(record->type == SymbolType::Integer);
    assert(Compare::equals_int64(5, record->data));
}

void String_Test::test_stringAt()
{
    const u32string code1 = U"s = \"hello\"; i = s.at(1);";
    EScript engine;
    engine.eval(code1);
    auto mainTable = engine.unit()->block()->symbolTable();
    auto i = mainTable->find(U"i");
    auto record = engine.getObjectRecord(i);
    assert(record->type == SymbolType::Integer);
    assert(Compare::equals_int64('e', record->data));
}

void String_Test::test_stringSubstring()
{
    const u32string code1 = U"s = \"hello\"; i = s.substring(1, 3);";
    EScript engine;
    engine.eval(code1);
    auto mainTable = engine.unit()->block()->symbolTable();
    auto i = mainTable->find(U"i");
    auto record = engine.getObjectRecord(i);
    assert(record->type == SymbolType::String);
    auto str = (StringObject*)record->data;
    assert(*str == U"el");
}

void String_Test::test_callMethodOfStringLiteral()
{
    const u32string code1 = U"s = 'hello, 🌏'.substring(1, 3);";
    EScript engine;
    engine.eval(code1);
    auto mainTable = engine.unit()->block()->symbolTable();
    auto s = mainTable->find(U"s");
    auto record = engine.getObjectRecord(s);
    assert(record->type == SymbolType::String);
    auto str = (StringObject*)record->data;
    assert(*str == U"el");
}

void String_Test::test_callMethodOfBQStringLiteral()
{
    const u32string code1 = U"s = `hello, 🌏`.substring(1, 3);";
    EScript engine;
    engine.eval(code1);
    auto mainTable = engine.unit()->block()->symbolTable();
    auto s = mainTable->find(U"s");
    auto record = engine.getObjectRecord(s);
    assert(record->type == SymbolType::String);
    auto str = (StringObject*)record->data;
    assert(*str == U"el");
}
