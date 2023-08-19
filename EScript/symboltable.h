/**
 * @file symboltable.h
 * @brief Таблица символов
 */
#ifndef ESCRIPT_SYMBOLTABLE_H
#define ESCRIPT_SYMBOLTABLE_H

#include "EScript_global.h"
#include "symbol.h"
#include <map>
#include <vector>

namespace escript {

class Unit;

/**
 * @brief Таблица символов
 */
class ESCRIPT_EXPORT SymbolTable
{
private:
    std::shared_ptr<Unit> _unit;
    std::shared_ptr<SymbolTable> _parentTable;
    // для быстрого поиска
    std::map<std::u32string, std::shared_ptr<Symbol> > _symbols;
    // вставленные по порядку
    // это нужно для сохранения переменных при генерации кода в порядке их
    // появления, тогда, например, можно вычислить их позиции,
    // не используя дополнительные средства
    std::vector<std::shared_ptr<Symbol>> _orderedSymbols;
    // счётчик для имён временных переменных
    static long _tmpVariableCounter;
public:
    /**
     * @brief Создаёт новый экземпляр класса SymbolTable
     */
    SymbolTable(std::shared_ptr<Unit> unit, std::shared_ptr<SymbolTable> parentSymbolTable = nullptr);
    /**
     * @brief Освобождает связанные с этим экземпляром ресурсы
     */
    virtual ~SymbolTable();
    void clear();
    std::vector<std::shared_ptr<Symbol>>::iterator begin()
    {
        return _orderedSymbols.begin();
    }
    std::vector<std::shared_ptr<Symbol>>::iterator end()
    {
        return _orderedSymbols.end();
    }
    size_t size()
    {
        return _orderedSymbols.size();
    }
    /**
     * @brief Добавляет новый символ в таблицу
     * @param identifier идентификатор
     * @param type тип данных
     * @return объект Symbol
     */
    std::shared_ptr<Symbol> add(const std::u32string &identifier);
    void addRange(std::shared_ptr<SymbolTable> &&otherTable);
    /**
     * @brief Добавляет временную переменную в таблицу
     * @return объект Symbol
     */
    std::shared_ptr<Symbol> addTemp();
    std::shared_ptr<Symbol> operator[](const std::u32string &identifier);
    /**
     * @brief Возвращает либо nullptr, либо указатель на запись в таблице
     *        символов для данного идентификатора.
     * @param identifier
     * @return указатель на nullptr, если не найдено
     */
    std::shared_ptr<Symbol> find(const std::u32string &identifier);
    std::shared_ptr<Symbol> findCurrentScopeOnly(const std::u32string &identifier);
    /**
     * @brief Добавляет ко всем символам смещение
     * @param offset смещение
     */
    void addOffset(uint64_t offset);
private:
    /**
     * @brief Генерирует временное имя для идентификатора
     * @return
     */
    static std::u32string makeTempId();
};

} // namespace escript

#endif // ESCRIPT_SYMBOLTABLE_H
