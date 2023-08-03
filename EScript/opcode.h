/**
 * AUTOGENERATED
 * @file opcode.h
 * @date 03.08.2023 22:34:34
 * @brief Перечисление кодов операций
 */
#ifndef OPCODE_H
#define OPCODE_H

namespace escript {

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
     * Пересылает в регистр A 64-битное число
     */
    MOV_A_DATA64 = 2,
    /**
     * Пересылает в регистр B целое число
     */
    MOV_B_DATA64 = 3,
    /**
     * Загружает в регистр A слово, расположенное по адресу M
     */
    LD64_A_M = 4,
    /**
     * Поместить по адресу M слово из регистра A
     */
    ST64_M_A = 5,
    /**
     * A=A+B, складывает два числа со знаком
     */
    ADI_A_B = 6,
    /**
     * A=A*B, умножает два числа со знаком
     */
    IMUL_A_B = 7,
    /**
     * Загружает в стек 64-битное целое без знака
     */
    LDC_UINT64_DATA64 = 8,
    /**
     * Загружает в стек 64-битное целое со знаком
     */
    LDC_INT64_DATA64 = 9,
    /**
     * Загружает в стек адрес переменной
     */
    LDLOC_M = 10,
    /**
     * Загружает в стек 64-битное вещественное число
     */
    LDC_DOUBLE_DATA64 = 11,
    /**
     * Вынимает из стека два числа и складывает их. Результат помещается 
     * обратно в стек.
     */
    ADDST = 12,
    /**
     * Вынимает из стека два числа и перемножает их. Результат помещается 
     * обратно в стек.
     */
    MULST = 13,
    /**
     * Вынимает из стека значение в формате 64-битного числа
     */
    POP_A = 14,
    /**
     * Сравнивает содержимое регистра A с 64-битным целым без знака
     */
    CMP_A_D64 = 15,
    /**
     * Вынимает из стека значение и размещает его по адресу M
     */
    STLOC_M = 16,
    /**
     * Вынимает из стека число и меняет его знак. Результат помещается 
     * обратно в стек.
     */
    NEG = 17,

};

} // namespace escript

#endif // OPCODE_H
