/**
 * AUTOGENERATED
 * @file opcode.h
 * @date 07.08.2023 15:37:45
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
     * Вынимает из стека значение и размещает его по адресу M
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

};

} // namespace escript

#endif // OPCODE_H
