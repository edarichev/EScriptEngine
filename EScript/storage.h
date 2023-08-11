/**
 * @file storage.h
 * @brief Хранилище объектов
 */
#ifndef STORAGE_H
#define STORAGE_H

#include "EScript_global.h"
#include <forward_list>
#include <list>
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
    SymbolType type = SymbolType::Undefined;
    /**
     * @brief Если тип простой, то это сами данные. Если строка или объект -
     *        указатель на объект в куче.
     */
    PtrIntType data = 0;
    /**
     * @brief Необязательно: Обратная ссылка на запись в таблице символов.
     */
    Symbol *symbol = nullptr;
    ObjectRecord() = default;
    ObjectRecord(const ObjectRecord &rhs) = default;
    ObjectRecord(ObjectRecord &&rhs);
    ObjectRecord(Symbol *s) : symbol(s) {}
    virtual ~ObjectRecord();
};


/**
 * @brief Хранилище объектов
 */
class ESCRIPT_EXPORT Storage
{
    // записи для ссылок на объекты в куче
    std::list<ObjectRecord> _records;
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
     * @brief Добавить пустую запись
     */
    ObjectRecord *installRecord(Symbol *s)
    {
        return &_records.emplace_front(s);
    }
    ObjectRecord *findRecord(Symbol *symbol);
};

} // namespace escript

#endif // STORAGE_H
