#include "stdafx.h"
#include "codeunit_test.h"

using namespace escript;
using namespace std;

void CodeUnit_Test::run()
{
    initTestCase();
    test_objectsWork();
    test_clear();
    cleanupTestCase();
}

void CodeUnit_Test::initTestCase()
{

}

void CodeUnit_Test::cleanupTestCase()
{

}

void CodeUnit_Test::test_objectsWork()
{
    std::shared_ptr<Unit> unit = std::make_shared<Unit>();
    auto table = unit->block()->symbolTable();
    // добавим в таблицу символов запись без указания типа:
    const u32string valueName = U"x";
    auto v1 = table->add(valueName);
    assert(v1->type() == SymbolType::Undefined);
    // Установим тип - целое
    v1->setType(SymbolType::Integer);
    // должен измениться тип
    assert(v1->type() == SymbolType::Integer);
}

void CodeUnit_Test::test_clear()
{
    // должно быть выполнено просто без ошибок
    const std::u32string code1 = U"x = 10; y = 'hello'; z = [1,2,3];";
    EScript engine;
    engine.eval(code1);
    auto unit = engine.unit();
    auto mainBlock = unit->block();
    auto mainTable = mainBlock->symbolTable();

    engine.clear();
    const std::u32string code2 = U"t = 10; f = 'hello'; j = [1,2,3];";
    engine.eval(code2);
    unit = engine.unit();
    mainBlock = unit->block();
    mainTable = mainBlock->symbolTable();
}
