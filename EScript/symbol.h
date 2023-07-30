/**
 * @file symbol.h
 * @brief Запись в таблице символов
 */
#ifndef ESCRIPT_SYMBOL_H
#define ESCRIPT_SYMBOL_H

#include "EScript_global.h"

namespace escript {

/**
 * @brief Тип символа
 */
enum class SymbolType
{
    Undefined,
    Integer,
    Real,
    String,
    Function,
    Object
};

/**
 * @brief Запись в таблице символов
 */
class ESCRIPT_EXPORT Symbol
{
private:
    SymbolType _type { SymbolType::Undefined };
public:
    /**
     * @brief Создаёт новый экземпляр класса Symbol
     */
    Symbol();
    /**
     * @brief Освобождает связанные с этим экземпляром ресурсы
     */
    virtual ~Symbol();
    SymbolType type() const;
    void setType(SymbolType newType);
};

} // namespace escript

#endif // ESCRIPT_SYMBOL_H
