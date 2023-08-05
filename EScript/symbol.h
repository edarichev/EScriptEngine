/**
 * @file symbol.h
 * @brief Запись в таблице символов
 */
#ifndef ESCRIPT_SYMBOL_H
#define ESCRIPT_SYMBOL_H

#include "EScript_global.h"
#include "types.h"
#include <memory>

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
    // При трансляции: размещение области памяти этой переменной
    // относительно начала объектного файла.
    uint64_t _location = 0;
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
    const std::u32string &name() const;
    const std::string utf8Name() const;
    /**
     * @brief При трансляции: размещение области памяти этой переменной
     *        относительно начала объектного файла.
     * @return
     */
    uint64_t location() const;
    void setLocation(uint64_t newLocation);
    /**
     * @brief Добавляет смещение
     * @param offset смещение
     */
    void addOffset(uint64_t offset);
};

} // namespace escript

#endif // ESCRIPT_SYMBOL_H
