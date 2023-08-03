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
    void writeVariableSection(std::shared_ptr<Block> block,
                              std::vector<uint8_t> &outBuffer);
};

} // namespace escript

#endif // TRANSLATOR_H
