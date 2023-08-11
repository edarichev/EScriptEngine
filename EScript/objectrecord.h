#ifndef ESCRIPT_OBJECTRECORD_H
#define ESCRIPT_OBJECTRECORD_H

#include "EScript_global.h"
#include "types.h"

namespace escript {

class Symbol;
/**
 * @brief Запись для таблицы объектов
 */
struct ESCRIPT_EXPORT ObjectRecord
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

} // namespace escript

#endif // ESCRIPT_OBJECTRECORD_H
