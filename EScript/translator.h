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

namespace escript {

/**
 * @brief Транслятор трёхадресного кода в целевой
 */
class ESCRIPT_EXPORT Translator
{
private:
    std::unique_ptr<Assembler> _asm;
    std::shared_ptr<Block> _block;
public:
    /**
     * @brief Создаёт новый экземпляр класса Translator
     */
    Translator();
    /**
     * @brief Освобождает связанные с этим экземпляром ресурсы
     */
    virtual ~Translator() = default;

    void translate(std::shared_ptr<Block> block,
                   const std::vector<TCode> &inputBuffer,
                   std::vector<uint8_t> &outBuffer);
private:
    Assembler &as() { return *_asm.get(); }
    void writeVariableSection(std::shared_ptr<Block> block,
                              std::vector<uint8_t> &outBuffer);
    void translateOperation(const TCode &c, std::vector<uint8_t> &outBuffer);
    /**
     * @brief Абстракция для извлечения адреса
     * @param symbol
     * @return
     */
    PtrIntType location(Symbol *symbol);
private:
    void opAdd(const TCode &c);
    void opMul(const TCode &c);
    void opUMinus(const TCode &c);
    void opAssign(const TCode &c);
};

} // namespace escript

#endif // TRANSLATOR_H
