#include "stdafx.h"
#include "codeunit_test.h"

using namespace escript;
using namespace std;

void CodeUnit_Test::run()
{
    initTestCase();
    test_objectsWork();
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
