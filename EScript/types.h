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
enum class SymbolType
{
    Undefined,
    Null,
    Integer,
    Real,
    Boolean,
    String,
    Function,
    Object,
    Variable, // вспомогательный тип
};

using IntType = long;
using RealType = double;
using StringType = std::u32string; // потом сделать по стандарту отдельный класс

} // namespace escript

#endif // TYPES_H
