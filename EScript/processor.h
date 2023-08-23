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
#include "stringcontainer.h"
#include "stackvalue.h"

namespace escript {

class Number;

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
    // стек для значений (собственно, сам стек)
    std::stack<StackValue> _stack;
    Storage *_storage = nullptr;
    StringContainer *_strings = nullptr;
    // для конвертации операций
    static std::map<OpCode, ArithmeticOperation> optypes;
    // для сохранения счётчика команд: CALL-вставить PC, RET - извлечь отсюда PC
    std::stack<uint64_t> _pcStack;
    std::stack<uint64_t> _activationRecords;
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
    void setStrings(StringContainer *s) { _strings = s; }
    /**
     * @brief PC - Program Counter, счётчик команд, позиция, где находится
     *        команда, которая должна выполнятся следующей
     */
    size_t PC() const { return _pc; }
    void setPC(size_t newPc) { _pc = newPc; }

    std::stack<StackValue> loadArguments();

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
            case OpCode::LDARGS: ldargs(); break;
            case OpCode::RET: ret(); break;
            case OpCode::CALL: call(); break;
            case OpCode::LDSTRING: ldstring(); break;
            case OpCode::CALLM: callm(); break;
            case OpCode::ALLOCARRAY: allocarray(); break;
            case OpCode::INC: inc(); break;
            case OpCode::DEC: dec(); break;
            case OpCode::POP: pop(); break;
            case OpCode::RSH: rsh(); break;
            case OpCode::RSHZ: rshz(); break;
            case OpCode::LSH: lsh(); break;
            case OpCode::BIT_AND: bit_and(); break;
            case OpCode::BIT_OR: bit_or(); break;
            case OpCode::BIT_XOR: bit_xor(); break;
            case OpCode::LOG_AND: log_and(); break;
            case OpCode::LOG_OR: log_or(); break;
            case OpCode::BIT_NOT: bit_not(); break;
            case OpCode::LOG_NOT: log_not(); break;
            case OpCode::MODST: modst(); break;
            case OpCode::ST_AR: st_ar(); break;
            case OpCode::LD_AR: ld_ar(); break;

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
     * M. M — указатель на Symbol.
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
    /**
     * @brief Загружает аргументы из стека в переменную arguments
     */
    void ldargs();
    /**
     * @brief Возврат из функции. Из стека вынимается предыдущее значение 
     * PC, и происходит переход к нему.
     */
    void ret();
    /**
     * @brief Вызывает функцию, расположенную по указанному адресу.
     */
    void call();
    /**
     * @brief Загружает в стек индекс строки. Строка устанавливается 
     * в таблицу строк.
     */
    void ldstring();
    /**
     * @brief Вызывает метод объекта автоматизации. Последним в стеке 
     * находится строка, загруженная с помощью LDSTRING, означающая 
     * имя метода.
     */
    void callm();
    /**
     * @brief Создаёт массив. Аргумент — Symbol*. Если массива нет, 
     * он устанавливается в таблицу объектов.
     */
    void allocarray();
    /**
     * @brief Извлекает из стека аргумент, увеличивает на 1 и снова 
     * помещает его в стек. Аргумент: число или переменная в стеке.
     */
    void inc();
    /**
     * @brief Извлекает из стека аргумент, уменьшает на 1 и снова помещает 
     * его в стек. Аргумент: число или переменная в стеке.
     */
    void dec();
    /**
     * @brief Удаляет верхний элемент из стека, никуда его не присваивая.
     */
    void pop();
    /**
     * @brief Сдвиг вправо со знаком (если спереди 1, то она остаётся). 
     * В стеке два операнда, №2 сверху. Операнд 1 сдвигается на величину 
     * операнда 2. Результат помещается в стек.
     */
    void rsh();
    /**
     * @brief Сдвиг вправо с заполнением нулями. В стеке два операнда, 
     * №2 сверху. Операнд 1 сдвигается на величину операнда 2. Результат 
     * помещается в стек.
     */
    void rshz();
    /**
     * @brief Сдвиг влево. В стеке два операнда, №2 сверху. Операнд 
     * 1 сдвигается на величину операнда 2. Результат помещается в 
     * стек.
     */
    void lsh();
    /**
     * @brief Побитовое И между двумя операндами в стеке. Результат 
     * помещается в стек.
     */
    void bit_and();
    /**
     * @brief Побитовое ИЛИ между двумя операндами в стеке. Результат 
     * помещается в стек.
     */
    void bit_or();
    /**
     * @brief Побитовое ИСКЛЮЧАЮЩЕЕ ИЛИ между двумя операндами в стеке. 
     * Результат помещается в стек.
     */
    void bit_xor();
    /**
     * @brief Логическое И между двумя операндами в стеке. Результат 
     * помещается в стек.
     */
    void log_and();
    /**
     * @brief Логическое ИЛИ между двумя операндами в стеке. Результат 
     * помещается в стек.
     */
    void log_or();
    /**
     * @brief Побитовое НЕ над элементом в стеке. Результат помещается 
     * в стек.
     */
    void bit_not();
    /**
     * @brief Побитовое НЕ над элементом в стеке. Результат помещается 
     * в стек.
     */
    void log_not();
    /**
     * @brief Остаток от деления между двумя операндами в стеке. Результат 
     * помещается в стек.
     */
    void modst();
    /**
     * @brief Сохраняет запись активации. Аргумент: 32бит старшие — 
     * смещение до начала таблицы переменных в функции, 32 бит младшие 
     * — следующая позиция после последней переменной в таблице переменных 
     * функции. Эта команда не работает вне функций (игнорируется при 
     * генерации кода).
     */
    void st_ar();
    /**
     * @brief Загружает запись активации. Аргумент: 32бит старшие — 
     * смещение до начала таблицы переменных в функции, 32 бит младшие 
     * — следующая позиция после последней переменной в таблице переменных 
     * функции. Эта команда не работает вне функций (игнорируется при 
     * генерации кода).
     */
    void ld_ar();
// </autogenerated>

    // внешний интерфейс (для вызовов методов)
public:
    /**
     * @brief Вставляет в стек целое число (во время выполнения).
     * Этот метод нужен для размещения возвращаемого значения в стек.
     * @param value
     */
    void pushToStack(int64_t value);
    void pushBooleanToStack(bool value);
    void pushToStack(const std::u32string &value);
    void pushToStack(std::u32string &&value);
    void pushStringToStack(StringObject *strValue);
    void pushArrayToStack(Array *arrValue);
    void pushToStack(SymbolType type, uint64_t value);
    void pushToStack(StackValue value);
    void pushRealToStack(double value);
    void pushObjectToStack(AutomationObject *obj);
    StackValue popFromStack();

private:

    // несколько типизированных вариантов
    void setValue(ObjectRecord *ptr, int64_t value);
    void setValue(ObjectRecord *ptr, double value);
    void setValue(ObjectRecord *ptr, bool value);
    void setValue(ObjectRecord *ptr, StringObject *value);

    int64_t getIntValue(ObjectRecord *ptr);
    double getRealValue(ObjectRecord *ptr);

    ObjectRecord *installRecord(Symbol *symbol);
    ObjectRecord *readRecord(Symbol /*out*/ *&symbol);


    void binaryStackOp(OpCode opCode);

    // упаковка простых типов
private:
    std::unique_ptr<Number> packNumber(const StackValue &v);
};


} // namespace escript

#endif // ESCRIPT_PROCESSOR
