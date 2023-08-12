/**
 * @file types.h
 * @brief Определения типов
 */
#ifndef TYPES_H
#define TYPES_H

namespace escript {

/**
 * @brief Тип символа
 */
enum class SymbolType : uint8_t
{
    Undefined,
    Null,
    Integer,
    Real,
    Boolean,
    String,
    Function,
    Object,
    /**
     * Вспомогательный тип, он нужен только в таблице символов
     */
    Variable,
    Block,
};

using IntType = long;
using RealType = double;
using StringType = std::u32string; // потом сделать по стандарту отдельный класс

} // namespace escript

#endif // TYPES_H
