#ifndef ARRAY_H
#define ARRAY_H

#include "EScript_global.h"
#include <map>
#include <vector>
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
class ESCRIPT_EXPORT Array final : public AutomationObject
{
    std::map<std::string, PValue> _items;
    std::vector<std::string> _keys;
    using BaseClass = AutomationObject;
    using pFn = void (Array::*)(Processor *p);
    static std::map<std::u32string, pFn> _fn;
public:
    /**
     * @brief Создаёт новый экземпляр класса Array
     */
    Array();
    /**
     * @brief Освобождает связанные с этим экземпляром ресурсы
     */
    virtual ~Array();
    std::map<std::string, PValue>::iterator begin() { return _items.begin(); }
    std::map<std::string, PValue>::iterator end() { return _items.end(); }
    PValue get(int64_t index);
    PValue get(const std::u32string &index);
    void set(int64_t index, PValue value);
    void set(int64_t index, SymbolType t, uint64_t value);
    void set(const std::u32string &index, PValue value);
    int64_t length() const;
    void add(PValue value);
    std::u32string uString() const;
    // AutomationObject interface
public:
    virtual bool call(const std::u32string &method, Processor *p) override;
private:
    void setKeyValue(const std::string &key, const PValue &value);
    void buildFunctionsMap();
    void call_get_length(Processor *p);
    void call_push(Processor *p);
    void call_get(Processor *p);
    void call_set(Processor *p);
};

} // namespace escript

#endif // ARRAY_H
