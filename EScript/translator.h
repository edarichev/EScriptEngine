/**
 * @file translator.h
 * @brief Транслятор трёхадресного кода в целевой
 */
#ifndef TRANSLATOR_H
#define TRANSLATOR_H

#include "EScript_global.h"
#include "tcode.h"
#include "unit.h"
#include "assembler.h"
#include "pvalue.h"

namespace escript {

/**
 * @brief Транслятор трёхадресного кода в целевой
 */
class ESCRIPT_EXPORT Translator
{
private:
    std::unique_ptr<Assembler> _asm;
    std::shared_ptr<Block> _block;
    // для конвертации операций
    static std::map<OperationType, ArithmeticOperation> optypes;
public:
    /**
     * @brief Создаёт новый экземпляр класса Translator
     */
    Translator();
    /**
     * @brief Освобождает связанные с этим экземпляром ресурсы
     */
    virtual ~Translator() = default;
    /**
     * @brief Транслирует в машинный код указанный блок как единое целое,
     *        получая объектный файл outBuffer
     * @param block транслируемый блок верхнего уровня
     * @param inputBuffer буфер с трёхадресным кодом
     * @param outBuffer выходной объектный файл (должен быть пустым)
     */
    void translate(std::shared_ptr<Block> block,
                   const std::vector<TCode> &inputBuffer,
                   std::vector<uint8_t> &outBuffer);
private:
    /**
     * @brief Возвращает короткую ссылку на ассемблер для удобства
     */
    Assembler &as() { return *_asm.get(); }
    /**
     * @brief Записывает секцию DATA (переменные)
     * @param block транслируемый блок
     * @param outBuffer выходной буфер
     */
    void writeVariableSection(std::shared_ptr<Block> block,
                              std::vector<uint8_t> &outBuffer);
    /**
     * @brief Записывает все символы из всех таблиц символов этого блока
     *        и всех его дочерних боков.
     * @param block транслируемый блок
     * @param outBuffer выходной буфер
     * @param totalRecords полное число записей во всех вложенных таблицах
     *        символов
     */
    void writeAllSymbols(std::shared_ptr<Block> block,
                         std::vector<uint8_t> &outBuffer,
                         uint32_t &totalRecords);
    /**
     * @brief Транслирует в машинный код указанную операцию
     * @param c трёхадресная операция
     */
    void translateOperation(const TCode &c);
    /**
     * @brief Абстракция для извлечения адреса
     * @param symbol
     * @return
     */
    PtrIntType location(Symbol *symbol);
private:
    void binaryOp(const TCode &c);
    void opAdd(const TCode &c);
    void opMul(const TCode &c);
    void opUMinus(const TCode &c);
    void opAssign(const TCode &c);
    /**
     * @brief Генерирует вывод команды ldc_*** в зависимотси от типа
     * @param type тип аргумента
     * @param operand значение операнда
     */
    void emit_ldc(SymbolType type, const OperandRecord &operand);
    /**
     * @brief Пробует вычислить бинарную операцию, если это возможно.
     * @param c инструкция
     * @return true, если удалось сократить код
     */
    bool tryCalcBinaryOp(const TCode &c);
};

} // namespace escript

#endif // TRANSLATOR_H
