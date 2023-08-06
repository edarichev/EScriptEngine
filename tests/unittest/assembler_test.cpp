/**
 * AUTOGENERATED
 * @file assembler_test.cpp
 * @date 07.08.2023 00:12:15
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
    uint64_t uint64_t_value = 0;
    int64_t int64_t_value = 0;
    double double_value = 0;
    //uint32_t uint32_t_value32 = 0;
    //uint16_t uint16_t_value = 0;
    uint8_t uint8_t_value = 0;

    // указатели для чтения
    uint8_t *p = nullptr;
    // сдвиг от начала буфера - счётчик, т.к. указательнельзя двигать просто так
    // т.к. буфер может перевыделяться, поэтому для доступа к буферу нужно
    // присвоить указатель "p" после выполнения команды
    size_t n = 0;


    a.nop();
    p = obj.data() + n;
    n += Assembler::instructionSize(OpCode::NOP);
    assert(*((OpCodeType *)p) == (OpCodeType)OpCode::NOP);
    assert(obj.size() == n);
    
    a.halt();
    p = obj.data() + n;
    n += Assembler::instructionSize(OpCode::HALT);
    assert(*((OpCodeType *)p) == (OpCodeType)OpCode::HALT);
    assert(obj.size() == n);
    
    uint64_t_value = rand();
    a.ldc_uint64_data64(uint64_t_value);
    p = obj.data() + n;
    n += Assembler::instructionSize(OpCode::LDC_UINT64_DATA64);
    assert(*p == (OpCodeType)OpCode::LDC_UINT64_DATA64);
    p += Assembler::INSTRUCTION_CODE_SIZE;
    assert(*((uint64_t *)p) == uint64_t_value);
    assert(obj.size() == n);
    
    int64_t_value = rand();
    a.ldc_int64_data64(int64_t_value);
    p = obj.data() + n;
    n += Assembler::instructionSize(OpCode::LDC_INT64_DATA64);
    assert(*p == (OpCodeType)OpCode::LDC_INT64_DATA64);
    p += Assembler::INSTRUCTION_CODE_SIZE;
    assert(*((int64_t *)p) == int64_t_value);
    assert(obj.size() == n);
    
    uint64_t_value = rand();
    a.ldloc_m(uint64_t_value);
    p = obj.data() + n;
    n += Assembler::instructionSize(OpCode::LDLOC_M);
    assert(*p == (OpCodeType)OpCode::LDLOC_M);
    p += Assembler::INSTRUCTION_CODE_SIZE;
    assert(*((uint64_t *)p) == uint64_t_value);
    assert(obj.size() == n);
    
    double_value = rand();
    a.ldc_double_data64(double_value);
    p = obj.data() + n;
    n += Assembler::instructionSize(OpCode::LDC_DOUBLE_DATA64);
    assert(*p == (OpCodeType)OpCode::LDC_DOUBLE_DATA64);
    p += Assembler::INSTRUCTION_CODE_SIZE;
    assert(*((double *)p) == double_value);
    assert(obj.size() == n);
    
    a.addst();
    p = obj.data() + n;
    n += Assembler::instructionSize(OpCode::ADDST);
    assert(*((OpCodeType *)p) == (OpCodeType)OpCode::ADDST);
    assert(obj.size() == n);
    
    a.mulst();
    p = obj.data() + n;
    n += Assembler::instructionSize(OpCode::MULST);
    assert(*((OpCodeType *)p) == (OpCodeType)OpCode::MULST);
    assert(obj.size() == n);
    
    a.divst();
    p = obj.data() + n;
    n += Assembler::instructionSize(OpCode::DIVST);
    assert(*((OpCodeType *)p) == (OpCodeType)OpCode::DIVST);
    assert(obj.size() == n);
    
    a.subst();
    p = obj.data() + n;
    n += Assembler::instructionSize(OpCode::SUBST);
    assert(*((OpCodeType *)p) == (OpCodeType)OpCode::SUBST);
    assert(obj.size() == n);
    
    uint64_t_value = rand();
    a.stloc_m(uint64_t_value);
    p = obj.data() + n;
    n += Assembler::instructionSize(OpCode::STLOC_M);
    assert(*p == (OpCodeType)OpCode::STLOC_M);
    p += Assembler::INSTRUCTION_CODE_SIZE;
    assert(*((uint64_t *)p) == uint64_t_value);
    assert(obj.size() == n);
    
    a.neg();
    p = obj.data() + n;
    n += Assembler::instructionSize(OpCode::NEG);
    assert(*((OpCodeType *)p) == (OpCodeType)OpCode::NEG);
    assert(obj.size() == n);
    
    uint64_t_value = rand();
    a.jmp_m(uint64_t_value);
    p = obj.data() + n;
    n += Assembler::instructionSize(OpCode::JMP_M);
    assert(*p == (OpCodeType)OpCode::JMP_M);
    p += Assembler::INSTRUCTION_CODE_SIZE;
    assert(*((uint64_t *)p) == uint64_t_value);
    assert(obj.size() == n);
    
    uint8_t_value = rand();
    a.ldc_bool_data8(uint8_t_value);
    p = obj.data() + n;
    n += Assembler::instructionSize(OpCode::LDC_BOOL_DATA8);
    assert(*p == (OpCodeType)OpCode::LDC_BOOL_DATA8);
    p += Assembler::INSTRUCTION_CODE_SIZE;
    assert(*((uint8_t *)p) == uint8_t_value);
    assert(obj.size() == n);
    
    uint64_t_value = rand();
    a.brfalse_m(uint64_t_value);
    p = obj.data() + n;
    n += Assembler::instructionSize(OpCode::BRFALSE_M);
    assert(*p == (OpCodeType)OpCode::BRFALSE_M);
    p += Assembler::INSTRUCTION_CODE_SIZE;
    assert(*((uint64_t *)p) == uint64_t_value);
    assert(obj.size() == n);
    
}
