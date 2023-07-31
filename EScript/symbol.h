/**
 * @file symbol.h
 * @brief Запись в таблице символов
 */
#ifndef ESCRIPT_SYMBOL_H
#define ESCRIPT_SYMBOL_H

#include "EScript_global.h"
#include "types.h"

namespace escript {

class Unit;

/**
 * @brief Запись в таблице символов
 */
class ESCRIPT_EXPORT Symbol
{
private:
    std::shared_ptr<Unit> _unit;
    const std::u32string _name;
    // тип данных
    SymbolType _type { SymbolType::Undefined };
    // указатель на данные
    void *_dataPtr = nullptr;
public:
    /**
     * @brief Создаёт новый экземпляр класса Symbol
     */
    Symbol(std::shared_ptr<Unit> unit,
           const std::u32string &id,
           SymbolType dataType = SymbolType::Undefined);
    /**
     * @brief Освобождает связанные с этим экземпляром ресурсы
     */
    virtual ~Symbol();
    /**
     * @brief Возвращает тип данных
     */
    SymbolType type() const;
    /**
     * @brief Устанавливает тип данных
     * @param newType новый тип данных
     */
    void setType(SymbolType newType);
    /**
     * @brief Возвращает указатель на данные
     * @return
     */
    void *data() const;
private:
    /**
     * @brief Отсоединяет старое значение
     */
    void detachOldValue();
};

} // namespace escript

#endif // ESCRIPT_SYMBOL_H
