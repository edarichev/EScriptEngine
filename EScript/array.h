#ifndef ARRAY_H
#define ARRAY_H

#include "EScript_global.h"
#include <map>

namespace escript {

struct PValue;

/**
 * @brief Массив.
 * @details Индексами служат строки. Так можно использовать его как
 * ассоциативный. Фактически это хэш-таблица (ключ в utf8).
 * При обращении к несуществующему индексу вставляются новые элементы (пустые).
 * Каждый элемент - это PValue.
 */
class ESCRIPT_EXPORT Array
{
    std::map<std::string, PValue> _items;
public:
    /**
     * @brief Создаёт новый экземпляр класса Array
     */
    Array();
    /**
     * @brief Освобождает связанные с этим экземпляром ресурсы
     */
    virtual ~Array();
    PValue get(int64_t index);
    PValue get(const std::u32string &index);
    void set(int64_t index, PValue value);
    void set(const std::u32string &index, PValue value);
    uint64_t length() { return _items.size(); }
};

} // namespace escript

#endif // ARRAY_H
