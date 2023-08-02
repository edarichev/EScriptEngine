/**
 * AUTOGENERATED
 * @file assembler_test.cpp
 * @date $(Date)
 * @brief Тестирование записи инструкций в объектный файл
 */
#include "stdafx.h"
#include "assembler_test.h"
#include "assembler.h"

using namespace escript;
using namespace std;

void Assembler_Test::run()
{
    initTestCase();
    test_byteSequence();
    cleanupTestCase();
}

void Assembler_Test::initTestCase()
{

}

void Assembler_Test::cleanupTestCase()
{

}

void Assembler_Test::test_byteSequence()
{
    // проверяется корректность записи в память, никаких вычислений

    // буфер - объектный файл
    std::vector<uint8_t> obj;
    Assembler a(obj);
    // переменные для теста записи
    uint64_t value64 = 0;
    //uint64_t value32 = 0;
    //uint64_t value16 = 0;
    //uint64_t value8 = 0;

    // указатели для чтения
    uint8_t *p = nullptr;
    // сдвиг от начала буфера - счётчик, т.к. указательнельзя двигать просто так
    // т.к. буфер может перевыделяться, поэтому для доступа к буферу нужно
    // присвоить указатель "p" после выполнения команды
    size_t n = 0;

$(Test)
}
