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
    assert(v1->data() == nullptr);
    // Установим тип - целое
    v1->setType(SymbolType::Integer);
    // должен измениться тип
    assert(v1->type() == SymbolType::Integer);
    // должна быть выделена память
    assert(v1->data() != nullptr);
    // установим некое число
    IntType value1 = 12345;
    *(IntType*)v1->data() = value1;
    // должно смениться число
    assert(*(IntType*)v1->data() == value1);
    // проверим в таблице целых чисел:
    assert(unit->staticIntegerValues().front() == value1);
    // найдём теперь переменную по имени:
    auto v2 = table->find(valueName);
    // проверим, нашли ли переменную
    assert(v2 != nullptr);
    // есть ли у неё данные
    assert(v2->data() != nullptr);
    // верны ли данные
    assert(*(IntType*)v2->data() == value1);
}
