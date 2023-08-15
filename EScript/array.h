#ifndef ARRAY_H
#define ARRAY_H

#include "EScript_global.h"
#include <map>
#include "automationobject.h"
#include "types.h"

namespace escript {

struct PValue;

/**
 * @brief Массив.
 * @details Индексами служат строки. Так можно использовать его как
 * ассоциативный. Фактически это хэш-таблица (ключ в utf8).
 * При обращении к несуществующему индексу вставляются новые элементы (пустые).
 * Каждый элемент - это PValue.
 */
class ESCRIPT_EXPORT Array : public AutomationObject
{
    std::map<std::string, PValue> _items;
    using BaseClass = AutomationObject;
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
    void set(int64_t index, SymbolType t, uint64_t value);
    void set(const std::u32string &index, PValue value);
    int64_t length() const;
    void add(PValue value);

    // AutomationObject interface
public:
    virtual bool call(const std::u32string &method, Processor *p) override;
};

} // namespace escript

#endif // ARRAY_H
