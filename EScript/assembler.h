/**
 * AUTOGENERATED
 * @file assembler.h
 * @date 15.08.2023 21:38:34
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
private:
    bool _showListing = false;
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
    /**
     * @brief Возвращает текущую позицию, куда записывается следующая команда.
     */
    uint64_t currentPos() const { return _buffer.size(); }
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
     * @param out поток вывода, например, std::cout
     */
    void disassemble(std::ostream &out);

    // Команды. Код и операнды записываются в буфер памяти
public:
    // <autogenerated>

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
     * @brief Вынимает из стека два числа и делит нижний элемент на 
     * верхний. Результат помещается обратно в стек.
     */
    void divst()
    {
        op1(OpCode::DIVST);
    }
    /**
     * @brief Вынимает из стека два числа и вычитает нижний элемент 
     * из верхнего. Результат помещается обратно в стек.
     */
    void subst()
    {
        op1(OpCode::SUBST);
    }
    /**
     * @brief Вынимает из стека значение и размещает его по адресу 
     * M. M — указатель на Symbol.
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
    /**
     * @brief Выполняет безусловный переход по указанному адресу.
     */
    void jmp_m(uint64_t data)
    {
        op2(OpCode::JMP_M, data);
    }
    /**
     * @brief Загружает в стек логическое значение. 1 — true, 0 — false.
     */
    void ldc_bool_data8(uint8_t data)
    {
        op2(OpCode::LDC_BOOL_DATA8, data);
    }
    /**
     * @brief Вынимает значение из стека и если значение равно 0, false, 
     * делает переход по адресу M.
     */
    void iffalse_m(uint64_t data)
    {
        op2(OpCode::IFFALSE_M, data);
    }
    /**
     * @brief Вынимает из стека два операнда, сравнивает их, если нижний 
     * элемент меньше верхнего, то результат true, иначе — false. Результат 
     * заносится в стек в виде константы true/false.
     */
    void stless()
    {
        op1(OpCode::STLESS);
    }
    /**
     * @brief Вынимает из стека два операнда, сравнивает их, если нижний 
     * элемент меньше или равен верхнему, то результат true, иначе 
     * — false. Результат заносится в стек в виде константы true/false.
     */
    void stless_eq()
    {
        op1(OpCode::STLESS_EQ);
    }
    /**
     * @brief Вынимает из стека два операнда, сравнивает их, если нижний 
     * элемент больше верхнего, то результат true, иначе — false. Результат 
     * заносится в стек в виде константы true/false.
     */
    void stgt()
    {
        op1(OpCode::STGT);
    }
    /**
     * @brief Вынимает из стека два операнда, сравнивает их, если нижний 
     * элемент больше или равен верхнему, то результат true, иначе 
     * — false. Результат заносится в стек в виде константы true/false.
     */
    void stgt_eq()
    {
        op1(OpCode::STGT_EQ);
    }
    /**
     * @brief Вынимает из стека два операнда, сравнивает их, если нижний 
     * элемент равен верхнему, то результат true, иначе — false. Результат 
     * заносится в стек в виде константы true/false.
     */
    void steq()
    {
        op1(OpCode::STEQ);
    }
    /**
     * @brief Загружает аргументы из стека в переменную arguments
     */
    void ldargs()
    {
        op1(OpCode::LDARGS);
    }
    /**
     * @brief Возврат из функции. Из стека вынимается предыдущее значение 
     * PC, и происходит переход к нему.
     */
    void ret()
    {
        op1(OpCode::RET);
    }
    /**
     * @brief Вызывает функцию, расположенную по указанному адресу.
     */
    void call(uint64_t data)
    {
        op2(OpCode::CALL, data);
    }
    /**
     * @brief Загружает в стек индекс строки. Строка устанавливается 
     * в таблицу строк.
     */
    void ldstring(uint64_t data)
    {
        op2(OpCode::LDSTRING, data);
    }
    /**
     * @brief Вызывает метод объекта автоматизации. Последним в стеке 
     * находится строка, загруженная с помощью LDSTRING, означающая 
     * имя метода.
     */
    void callm()
    {
        op1(OpCode::CALLM);
    }
    /**
     * @brief Создаёт массив. Аргумент — Symbol*. Если массива нет, 
     * он устанавливается в таблицу объектов.
     */
    void allocarray(uint64_t data)
    {
        op2(OpCode::ALLOCARRAY, data);
    }
    /**
     * @brief Извлекает из стека аргумент, увеличивает на 1 и снова 
     * помещает его в стек. Аргумент: число или переменная в стеке.
     */
    void inc()
    {
        op1(OpCode::INC);
    }
    /**
     * @brief Извлекает из стека аргумент, уменьшает на 1 и снова помещает 
     * его в стек. Аргумент: число или переменная в стеке.
     */
    void dec()
    {
        op1(OpCode::DEC);
    }
    /**
     * @brief Удаляет верхний элемент из стека, никуда его не присваивая.
     */
    void pop()
    {
        op1(OpCode::POP);
    }
    /**
     * @brief Сдвиг вправо со знаком (если спереди 1, то она остаётся). 
     * В стеке два операнда, №2 сверху. Операнд 1 сдвигается на величину 
     * операнда 2. Результат помещается в стек.
     */
    void rsh()
    {
        op1(OpCode::RSH);
    }
    /**
     * @brief Сдвиг вправо с заполнением нулями. В стеке два операнда, 
     * №2 сверху. Операнд 1 сдвигается на величину операнда 2. Результат 
     * помещается в стек.
     */
    void rshz()
    {
        op1(OpCode::RSHZ);
    }
    /**
     * @brief Сдвиг влево. В стеке два операнда, №2 сверху. Операнд 
     * 1 сдвигается на величину операнда 2. Результат помещается в 
     * стек.
     */
    void lsh()
    {
        op1(OpCode::LSH);
    }
    /**
     * @brief Побитовое И между двумя операндами в стеке. Результат 
     * помещается в стек.
     */
    void bit_and()
    {
        op1(OpCode::BIT_AND);
    }
    /**
     * @brief Побитовое ИЛИ между двумя операндами в стеке. Результат 
     * помещается в стек.
     */
    void bit_or()
    {
        op1(OpCode::BIT_OR);
    }
    /**
     * @brief Побитовое ИСКЛЮЧАЮЩЕЕ ИЛИ между двумя операндами в стеке. 
     * Результат помещается в стек.
     */
    void bit_xor()
    {
        op1(OpCode::BIT_XOR);
    }
    /**
     * @brief Логическое И между двумя операндами в стеке. Результат 
     * помещается в стек.
     */
    void log_and()
    {
        op1(OpCode::LOG_AND);
    }
    /**
     * @brief Логическое ИЛИ между двумя операндами в стеке. Результат 
     * помещается в стек.
     */
    void log_or()
    {
        op1(OpCode::LOG_OR);
    }


// </autogenerated>
    void setShowListing(bool newShowListing);

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
