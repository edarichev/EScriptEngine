/**
 * AUTOGENERATED
 * @file opcode.h
 * @date 16.08.2023 23:44:47
 * @brief Перечисление кодов операций
 */
#ifndef OPCODE_H
#define OPCODE_H

namespace escript {

using OpCodeType = uint8_t;

/**
 * @brief Перечисление кодов операций
 */
enum class OpCode : uint8_t
{
    /**
     * Нет операции
     */
    NOP = 0,
    /**
     * Остановка машины, возврат, программа полностью выполнена
     */
    HALT = 1,
    /**
     * Загружает в стек 64-битное целое без знака
     */
    LDC_UINT64_DATA64 = 2,
    /**
     * Загружает в стек 64-битное целое со знаком
     */
    LDC_INT64_DATA64 = 3,
    /**
     * Загружает в стек адрес переменной
     */
    LDLOC_M = 4,
    /**
     * Загружает в стек 64-битное вещественное число
     */
    LDC_DOUBLE_DATA64 = 5,
    /**
     * Вынимает из стека два числа и складывает их. Результат помещается 
     * обратно в стек.
     */
    ADDST = 6,
    /**
     * Вынимает из стека два числа и перемножает их. Результат помещается 
     * обратно в стек.
     */
    MULST = 7,
    /**
     * Вынимает из стека два числа и делит нижний элемент на верхний. 
     * Результат помещается обратно в стек.
     */
    DIVST = 8,
    /**
     * Вынимает из стека два числа и вычитает нижний элемент из верхнего. 
     * Результат помещается обратно в стек.
     */
    SUBST = 9,
    /**
     * Вынимает из стека значение и размещает его по адресу M. M — 
     * указатель на Symbol.
     */
    STLOC_M = 10,
    /**
     * Вынимает из стека число и меняет его знак. Результат помещается 
     * обратно в стек.
     */
    NEG = 11,
    /**
     * Выполняет безусловный переход по указанному адресу.
     */
    JMP_M = 12,
    /**
     * Загружает в стек логическое значение. 1 — true, 0 — false.
     */
    LDC_BOOL_DATA8 = 13,
    /**
     * Вынимает значение из стека и если значение равно 0, false, делает 
     * переход по адресу M.
     */
    IFFALSE_M = 14,
    /**
     * Вынимает из стека два операнда, сравнивает их, если нижний элемент 
     * меньше верхнего, то результат true, иначе — false. Результат 
     * заносится в стек в виде константы true/false.
     */
    STLESS = 15,
    /**
     * Вынимает из стека два операнда, сравнивает их, если нижний элемент 
     * меньше или равен верхнему, то результат true, иначе — false. 
     * Результат заносится в стек в виде константы true/false.
     */
    STLESS_EQ = 16,
    /**
     * Вынимает из стека два операнда, сравнивает их, если нижний элемент 
     * больше верхнего, то результат true, иначе — false. Результат 
     * заносится в стек в виде константы true/false.
     */
    STGT = 17,
    /**
     * Вынимает из стека два операнда, сравнивает их, если нижний элемент 
     * больше или равен верхнему, то результат true, иначе — false. 
     * Результат заносится в стек в виде константы true/false.
     */
    STGT_EQ = 18,
    /**
     * Вынимает из стека два операнда, сравнивает их, если нижний элемент 
     * равен верхнему, то результат true, иначе — false. Результат 
     * заносится в стек в виде константы true/false.
     */
    STEQ = 19,
    /**
     * Загружает аргументы из стека в переменную arguments
     */
    LDARGS = 20,
    /**
     * Возврат из функции. Из стека вынимается предыдущее значение 
     * PC, и происходит переход к нему.
     */
    RET = 21,
    /**
     * Вызывает функцию, расположенную по указанному адресу.
     */
    CALL = 22,
    /**
     * Загружает в стек индекс строки. Строка устанавливается в таблицу 
     * строк.
     */
    LDSTRING = 23,
    /**
     * Вызывает метод объекта автоматизации. Последним в стеке находится 
     * строка, загруженная с помощью LDSTRING, означающая имя метода.
     */
    CALLM = 24,
    /**
     * Создаёт массив. Аргумент — Symbol*. Если массива нет, он устанавливается 
     * в таблицу объектов.
     */
    ALLOCARRAY = 25,
    /**
     * Извлекает из стека аргумент, увеличивает на 1 и снова помещает 
     * его в стек. Аргумент: число или переменная в стеке.
     */
    INC = 26,
    /**
     * Извлекает из стека аргумент, уменьшает на 1 и снова помещает 
     * его в стек. Аргумент: число или переменная в стеке.
     */
    DEC = 27,
    /**
     * Удаляет верхний элемент из стека, никуда его не присваивая.
     */
    POP = 28,
    /**
     * Сдвиг вправо со знаком (если спереди 1, то она остаётся). В 
     * стеке два операнда, №2 сверху. Операнд 1 сдвигается на величину 
     * операнда 2. Результат помещается в стек.
     */
    RSH = 29,
    /**
     * Сдвиг вправо с заполнением нулями. В стеке два операнда, №2 
     * сверху. Операнд 1 сдвигается на величину операнда 2. Результат 
     * помещается в стек.
     */
    RSHZ = 30,
    /**
     * Сдвиг влево. В стеке два операнда, №2 сверху. Операнд 1 сдвигается 
     * на величину операнда 2. Результат помещается в стек.
     */
    LSH = 31,
    /**
     * Побитовое И между двумя операндами в стеке. Результат помещается 
     * в стек.
     */
    BIT_AND = 32,
    /**
     * Побитовое ИЛИ между двумя операндами в стеке. Результат помещается 
     * в стек.
     */
    BIT_OR = 33,
    /**
     * Побитовое ИСКЛЮЧАЮЩЕЕ ИЛИ между двумя операндами в стеке. Результат 
     * помещается в стек.
     */
    BIT_XOR = 34,
    /**
     * Логическое И между двумя операндами в стеке. Результат помещается 
     * в стек.
     */
    LOG_AND = 35,
    /**
     * Логическое ИЛИ между двумя операндами в стеке. Результат помещается 
     * в стек.
     */
    LOG_OR = 36,
    /**
     * Побитовое НЕ над элементом в стеке. Результат помещается в стек.
     */
    BIT_NOT = 37,
    /**
     * Побитовое НЕ над элементом в стеке. Результат помещается в стек.
     */
    LOG_NOT = 38,
    /**
     * Остаток от деления между двумя операндами в стеке. Результат 
     * помещается в стек.
     */
    MODST = 39,

};

} // namespace escript

#endif // OPCODE_H
