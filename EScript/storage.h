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
#include "objectrecord.h"

namespace escript {

class AutomationObject;

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
    void removeRecord(AutomationObject *obj);
};

} // namespace escript

#endif // STORAGE_H
