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
 * @details Схема такова:
 * команда_процессора параметр=Symbol* -- запись в секции DATA
 * -- ObjectRecord.data -- Object*
 */
class ESCRIPT_EXPORT Symbol
{
private:
    const std::u32string _name;
    // При трансляции: размещение области памяти этой переменной
    // относительно начала объектного файла.
    uint64_t _location = 0;
public:
    /**
     * @brief Создаёт новый экземпляр класса Symbol
     */
    Symbol(const std::u32string &id);
    /**
     * @brief Освобождает связанные с этим экземпляром ресурсы
     */
    virtual ~Symbol();
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
