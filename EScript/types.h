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
    Object
};

using IntType = long;

} // namespace escript

#endif // TYPES_H
