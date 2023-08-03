/**
 * AUTOGENERATED
 * @file assembler.h
 * @date 03.08.2023 22:34:34
 * @brief Для записи инструкций в объектный файл
 */
#ifndef ASSEMBLER_H
#define ASSEMBLER_H

#include "EScript_global.h"
#include <vector>
#include "opcode.h"

namespace escript {

/**
 * @brief Записывает инструкции в объектный файл
 */
class ESCRIPT_EXPORT Assembler
{
public:
    using ContainerElementType = uint8_t;
    using OpCodeType = uint8_t;
private:
    // буфер для записи (объектный файл)
    std::vector<ContainerElementType> &_buffer;
    // мнемоники
    static const std::string _opCodeNames[];
    // все коды операций
    static OpCode _opCodes[];
    // размер в байтах каждой команды
    static uint8_t _opCodeSize[];
    // ключ - код операции, значение - индекс в массивах
    static std::map<OpCode, int> _opCodesMap;
public:
    /**
     * @brief Размер кода машинной инструкции
     */
    static const constexpr int INSTRUCTION_CODE_SIZE = sizeof (OpCodeType);
public:
    /**
     * @brief Создаёт новый экземпляр класса Assembler
     */
    Assembler(std::vector<ContainerElementType> &buffer): _buffer(buffer)
    {
        initOpCodesMap();
    }
    // Доступ  командам и мнемоникам
public:
    /**
     * @brief Возвращает мнемонику команды
     * @param opCode код операции
     * @return мнемоника, например, "MOV A, B"
     */
    static const std::string &mnemonics(OpCode opCode);

    /**
     * @brief Возвращает размер команды в байтах
     * @param opCode код операции
     * @return зависит от числа и размера операндов
     */
    static uint8_t instructionSize(OpCode opCode);

    /**
     * @brief Дизассемблирует объектный файл
     * @param objectFile объектный файл
     * @param out поток вывода, например, std::cout
     */
    static void disassemble(const std::vector<ContainerElementType> &objectFile, std::ostream &out);

    // Команды. Код и операнды записываются в буфер памяти
public:
    /**
     * @brief Нет операции
     */
    void nop()
    {
        op1(OpCode::NOP);
    }
    /**
     * @brief Остановка машины, возврат, программа полностью выполнена
     */
    void halt()
    {
        op1(OpCode::HALT);
    }
    /**
     * @brief Пересылает в регистр A 64-битное число
     */
    void mov_a_data64(uint64_t data)
    {
        op2(OpCode::MOV_A_DATA64, data);
    }
    /**
     * @brief Пересылает в регистр B целое число
     */
    void mov_b_data64(uint64_t data)
    {
        op2(OpCode::MOV_B_DATA64, data);
    }
    /**
     * @brief Загружает в регистр A слово, расположенное по адресу 
     * M
     */
    void ld64_a_m(uint64_t data)
    {
        op2(OpCode::LD64_A_M, data);
    }
    /**
     * @brief Поместить по адресу M слово из регистра A
     */
    void st64_m_a(uint64_t data)
    {
        op2(OpCode::ST64_M_A, data);
    }
    /**
     * @brief A=A+B, складывает два числа со знаком
     */
    void adi_a_b()
    {
        op1(OpCode::ADI_A_B);
    }
    /**
     * @brief A=A*B, умножает два числа со знаком
     */
    void imul_a_b()
    {
        op1(OpCode::IMUL_A_B);
    }
    /**
     * @brief Загружает в стек 64-битное целое без знака
     */
    void ldc_uint64_data64(uint64_t data)
    {
        op2(OpCode::LDC_UINT64_DATA64, data);
    }
    /**
     * @brief Загружает в стек 64-битное целое со знаком
     */
    void ldc_int64_data64(int64_t data)
    {
        op2(OpCode::LDC_INT64_DATA64, data);
    }
    /**
     * @brief Загружает в стек адрес переменной
     */
    void ldloc_m(uint64_t data)
    {
        op2(OpCode::LDLOC_M, data);
    }
    /**
     * @brief Загружает в стек 64-битное вещественное число
     */
    void ldc_double_data64(double data)
    {
        op2(OpCode::LDC_DOUBLE_DATA64, data);
    }
    /**
     * @brief Вынимает из стека два числа и складывает их. Результат 
     * помещается обратно в стек.
     */
    void addst()
    {
        op1(OpCode::ADDST);
    }
    /**
     * @brief Вынимает из стека два числа и перемножает их. Результат 
     * помещается обратно в стек.
     */
    void mulst()
    {
        op1(OpCode::MULST);
    }
    /**
     * @brief Вынимает из стека значение в формате 64-битного числа
     */
    void pop_a()
    {
        op1(OpCode::POP_A);
    }
    /**
     * @brief Сравнивает содержимое регистра A с 64-битным целым без 
     * знака
     */
    void cmp_a_d64(uint64_t data)
    {
        op2(OpCode::CMP_A_D64, data);
    }
    /**
     * @brief Вынимает из стека значение и размещает его по адресу 
     * M
     */
    void stloc_m(uint64_t data)
    {
        op2(OpCode::STLOC_M, data);
    }
    /**
     * @brief Вынимает из стека число и меняет его знак. Результат 
     * помещается обратно в стек.
     */
    void neg()
    {
        op1(OpCode::NEG);
    }

private:
    /**
     * @brief Операция с одним только кодом
     * @param opCode код операции
     */
    void op1(OpCode opCode)
    {
        _buffer.insert(_buffer.end(),
                       (uint8_t*)&opCode,
                       (uint8_t*)&opCode + sizeof (OpCode));
    }
    /**
     * @brief Операция с двумя частями: код операции и операнд
     * @param opCode код операции
     * @param data операнд
     */
    template<typename T>
    typename std::enable_if_t<std::is_arithmetic_v<T>, void>
    op2(OpCode opCode, T data)
    {
        op1(opCode);                  // код операции
        _buffer.insert(_buffer.end(), // записываем операнд
                       (uint8_t*)&data,
                       (uint8_t*)&data + sizeof (T));
    }
private:
    /**
     * @brief Заполняет хэш-таблицу с индексами кодов операций
     */
    static void initOpCodesMap();
};

} // namespace escript
#endif // ASSEMBLER_H
