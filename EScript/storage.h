/**
 * @file storage.h
 * @brief Хранилище объектов
 */
#ifndef STORAGE_H
#define STORAGE_H

#include "EScript_global.h"
#include <forward_list>
#include "types.h"

namespace escript {

class Symbol;
/**
 * @brief Запись для таблицы объектов
 */
struct ObjectRecord
{
    /**
     * @brief Cчётчик ссылок
     */
    int counter = 0;
    /**
     * @brief Тип хранимых данных
     */
    SymbolType type = SymbolType::Null;
    /**
     * @brief Если тип простой, это это сами данные. Если строка или объект -
     *        указатель на объект в куче.
     */
    PtrIntType data = 0;
    /**
     * @brief Обратная ссылка на запись в таблице символов.
     */
    Symbol *symbol = nullptr;
    ObjectRecord() = default;
    ObjectRecord(Symbol *s) : symbol(s) {}
};


/**
 * @brief Хранилище объектов
 */
class ESCRIPT_EXPORT Storage
{
    // записи для ссылок на объекты в куче
    std::forward_list<ObjectRecord> _records;
public:
    /**
     * @brief Создаёт новый экземпляр класса Storage
     */
    Storage();
    /**
     * @brief Освобождает связанные с этим экземпляром ресурсы
     */
    virtual ~Storage();
    /**
     * @brief installIntValue
     * @param value
     */
    void installIntValue(int64_t value);
    /**
     * @brief Добавить пустую запись
     */
    ObjectRecord *installRecord(Symbol *s)
    {
        _records.push_front({s});
        return &_records.front();
    }
    ObjectRecord *findRecord(Symbol *symbol);
};

} // namespace escript

#endif // STORAGE_H
