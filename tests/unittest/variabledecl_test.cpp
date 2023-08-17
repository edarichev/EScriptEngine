#include "stdafx.h"
#include "variabledecl_test.h"


void VariableDecl_Test::run()
{
    initTestCase();
    test_decl();
    test_declComma();
    test_declNested();
    test_declNestedWithSameName();
    cleanupTestCase();
}

void VariableDecl_Test::initTestCase()
{

}

void VariableDecl_Test::cleanupTestCase()
{

}

void VariableDecl_Test::test_decl()
{
    const u32string code1 = U"var a = 157;";
    EScript engine;
    engine.setShowDisassembleListing(false);
    engine.setShowTCode(false);
    engine.eval(code1);
    auto mainTable = engine.unit()->block()->symbolTable();
    auto a = mainTable->find(U"a");
    auto record = engine.getObjectRecord(a);
    assert(record->type == SymbolType::Integer);
    assert(Compare::equals_int64(157, record->data));
}

void VariableDecl_Test::test_declComma()
{
    const u32string code1 = U"var a = 157, b = 34;";
    EScript engine;
    engine.setShowDisassembleListing(false);
    engine.setShowTCode(false);
    engine.eval(code1);
    auto mainTable = engine.unit()->block()->symbolTable();
    auto a = mainTable->find(U"a");
    auto record = engine.getObjectRecord(a);
    assert(record->type == SymbolType::Integer);
    assert(Compare::equals_int64(157, record->data));

    auto b = mainTable->find(U"b");
    record = engine.getObjectRecord(b);
    assert(record->type == SymbolType::Integer);
    assert(Compare::equals_int64(34, record->data));
}

void VariableDecl_Test::test_declNested()
{
    const u32string code1 = U"var a = 157; { var b = 34; }";
    EScript engine;
    engine.setShowDisassembleListing(false);
    engine.setShowTCode(false);
    engine.eval(code1);
    auto mainTable = engine.unit()->block()->symbolTable();
    auto a = mainTable->find(U"a");
    auto record = engine.getObjectRecord(a);
    assert(record->type == SymbolType::Integer);
    assert(Compare::equals_int64(157, record->data));

    auto subTable = engine.unit()->block()->blocks()[0]->blocks()[0]->symbolTable();
    auto b = subTable->find(U"b");
    record = engine.getObjectRecord(b);
    assert(record->type == SymbolType::Integer);
    assert(Compare::equals_int64(34, record->data));
}

void VariableDecl_Test::test_declNestedWithSameName()
{
    const u32string code1 = U"var a = 157; { var a = 34; }";
    EScript engine;
    engine.setShowDisassembleListing(false);
    engine.setShowTCode(false);
    engine.eval(code1);
    auto mainTable = engine.unit()->block()->symbolTable();
    auto a = mainTable->find(U"a");
    auto record = engine.getObjectRecord(a);
    assert(record->type == SymbolType::Integer);
    assert(Compare::equals_int64(157, record->data));

    auto subTable = engine.unit()->block()->blocks()[0]->blocks()[0]->symbolTable();
    auto a1 = subTable->find(U"a");
    record = engine.getObjectRecord(a1);
    assert(record->type == SymbolType::Integer);
    assert(Compare::equals_int64(34, record->data));
}

