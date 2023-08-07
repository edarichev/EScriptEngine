/**
 * @file processor.cpp
 * @brief Процессор (выполняет команды)
 */
#ifndef ESCRIPT_PROCESSOR_H
#define ESCRIPT_PROCESSOR_H

#include "EScript_global.h"
#include "opcode.h"
#include "assembler.h"
#include <stack>
#include "symbol.h"
#include "storage.h"
#include <cstring>
#include "pvalue.h"

namespace escript {

/**
 * @brief Процессор (выполняет команды)
 */
class ESCRIPT_EXPORT Processor
{
    uint8_t *_memory = nullptr;
    // указатель, синхронный с PC
    uint8_t *_p = nullptr;
    size_t _memoryLimit = 0;
    // PC - Program Counter, счётчик команд, позиция, где находится команда,
    // которая должна выполнятся следующей
    size_t _pc = 0;
    // тип засунутого в стек элемента
    std::stack<SymbolType> _itemType;
    // стек для значений (собственно, сам стек)
    std::stack<uint64_t> _stack;
    Storage *_storage = nullptr;
    // для конвертации операций
    static std::map<OpCode, ArithmeticOperation> optypes;
public:
    Processor();
    /**
     * @brief "Присоединить" к памяти
     * @param memory указатель на память машины
     * @param limit сколько байт
     */
    void setMemory(uint8_t *memory, size_t limit, Storage *storage)
    {
        _memory = memory;
        _memoryLimit = limit;
        _storage = storage;
    }
    /**
     * @brief PC - Program Counter, счётчик команд, позиция, где находится
     *        команда, которая должна выполнятся следующей
     */
    size_t PC() const { return _pc; }
    void setPC(size_t newPc) { _pc = newPc; }

    // <autogenerated>
public:
    /**
     * @brief Запуск выполнения с текущей позиции PC
     */
    void run()
    {
        while (_pc < _memoryLimit) {
            _p = _memory + _pc;
            OpCode opCode = (OpCode) *((OpCodeType*)(_p));
            auto shift = Assembler::instructionSize(opCode);
            switch (opCode) {
            case OpCode::HALT:
                next(shift);
                return;
            case OpCode::NOP: nop(); break;
            case OpCode::LDC_UINT64_DATA64: ldc_uint64_data64(); break;
            case OpCode::LDC_INT64_DATA64: ldc_int64_data64(); break;
            case OpCode::LDLOC_M: ldloc_m(); break;
            case OpCode::LDC_DOUBLE_DATA64: ldc_double_data64(); break;
            case OpCode::ADDST: addst(); break;
            case OpCode::MULST: mulst(); break;
            case OpCode::DIVST: divst(); break;
            case OpCode::SUBST: subst(); break;
            case OpCode::STLOC_M: stloc_m(); break;
            case OpCode::NEG: neg(); break;
            case OpCode::JMP_M: jmp_m(); break;
            case OpCode::LDC_BOOL_DATA8: ldc_bool_data8(); break;
            case OpCode::IFFALSE_M: iffalse_m(); break;
            case OpCode::STLESS: stless(); break;
            case OpCode::STLESS_EQ: stless_eq(); break;
            case OpCode::STGT: stgt(); break;
            case OpCode::STGT_EQ: stgt_eq(); break;
            case OpCode::STEQ: steq(); break;

            default:
                throw std::domain_error("Unsupported instruction: " + std::to_string((int)opCode));
            }
        }
    }
private:

    void next()
    {
        next(sizeof (OpCodeType));
    }

    void next(size_t shift)
    {
        _pc += shift;
        _p += shift;
    }

    /**
     * @brief Нет операции
     */
    void nop();
    /**
     * @brief Остановка машины, возврат, программа полностью выполнена
     */
    void halt();
    /**
     * @brief Загружает в стек 64-битное целое без знака
     */
    void ldc_uint64_data64();
    /**
     * @brief Загружает в стек 64-битное целое со знаком
     */
    void ldc_int64_data64();
    /**
     * @brief Загружает в стек адрес переменной
     */
    void ldloc_m();
    /**
     * @brief Загружает в стек 64-битное вещественное число
     */
    void ldc_double_data64();
    /**
     * @brief Вынимает из стека два числа и складывает их. Результат 
     * помещается обратно в стек.
     */
    void addst();
    /**
     * @brief Вынимает из стека два числа и перемножает их. Результат 
     * помещается обратно в стек.
     */
    void mulst();
    /**
     * @brief Вынимает из стека два числа и делит нижний элемент на 
     * верхний. Результат помещается обратно в стек.
     */
    void divst();
    /**
     * @brief Вынимает из стека два числа и вычитает нижний элемент 
     * из верхнего. Результат помещается обратно в стек.
     */
    void subst();
    /**
     * @brief Вынимает из стека значение и размещает его по адресу 
     * M
     */
    void stloc_m();
    /**
     * @brief Вынимает из стека число и меняет его знак. Результат 
     * помещается обратно в стек.
     */
    void neg();
    /**
     * @brief Выполняет безусловный переход по указанному адресу.
     */
    void jmp_m();
    /**
     * @brief Загружает в стек логическое значение. 1 — true, 0 — false.
     */
    void ldc_bool_data8();
    /**
     * @brief Вынимает значение из стека и если значение равно 0, false, 
     * делает переход по адресу M.
     */
    void iffalse_m();
    /**
     * @brief Вынимает из стека два операнда, сравнивает их, если нижний 
     * элемент меньше верхнего, то результат true, иначе — false. Результат 
     * заносится в стек в виде константы true/false.
     */
    void stless();
    /**
     * @brief Вынимает из стека два операнда, сравнивает их, если нижний 
     * элемент меньше или равен верхнему, то результат true, иначе 
     * — false. Результат заносится в стек в виде константы true/false.
     */
    void stless_eq();
    /**
     * @brief Вынимает из стека два операнда, сравнивает их, если нижний 
     * элемент больше верхнего, то результат true, иначе — false. Результат 
     * заносится в стек в виде константы true/false.
     */
    void stgt();
    /**
     * @brief Вынимает из стека два операнда, сравнивает их, если нижний 
     * элемент больше или равен верхнему, то результат true, иначе 
     * — false. Результат заносится в стек в виде константы true/false.
     */
    void stgt_eq();
    /**
     * @brief Вынимает из стека два операнда, сравнивает их, если нижний 
     * элемент равен верхнему, то результат true, иначе — false. Результат 
     * заносится в стек в виде константы true/false.
     */
    void steq();
// </autogenerated>
private:

    // несколько типизированных вариантов
    void setValue(ObjectRecord *ptr, int64_t value);
    void setValue(ObjectRecord *ptr, double value);
    void setValue(ObjectRecord *ptr, bool value);

    int64_t getIntValue(ObjectRecord *ptr);
    double getRealValue(ObjectRecord *ptr);

    ObjectRecord *installRecord(Symbol *symbol);
    ObjectRecord *readRecord(Symbol /*out*/ *&symbol);

    std::pair<SymbolType, uint64_t> popFromStack();

    void pushToStack(SymbolType type, uint64_t value);
    void binaryStackOp(OpCode opCode);
};


} // namespace escript

#endif // ESCRIPT_PROCESSOR
