#ifndef ARRAYOBJECT_H
#define ARRAYOBJECT_H

#include "EScript_global.h"
#include <map>
#include <vector>
#include "automationobject.h"
#include "types.h"
#include <functional>

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
    std::vector<PValue> _indexedItems;
    std::map<std::u32string, PValue> _namedItems;

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
    std::vector<PValue>::iterator begin() { return _indexedItems.begin(); }
    std::vector<PValue>::iterator end() { return _indexedItems.end(); }
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
    void setKeyValue(const std::u32string &key, const PValue &value);
    void buildFunctionsMap();
    void call_get_length(Processor *p);
    void call_push(Processor *p);
    void call_get(Processor *p);
    void call_set(Processor *p);
    void call_pop(Processor *p);
    void call_shift(Processor *p);
    void call_splice(Processor *p);
    void call_reverse(Processor *p);
    void call_fill(Processor *p);
    void call_sort(Processor *p);
    void call_unshift(Processor *p);
    void call_slice(Processor *p);
    void call_join(Processor *p);
    void call_filter(Processor *p);
    void call_firstIndex(Processor *p);
    void call_lastIndex(Processor *p);
    void call_includes(Processor *p);
    void call_forEach(Processor *p);
    void call_map(Processor *p);
    void call_some(Processor *p);
    void call_every(Processor *p);
private:
    /**
     * @brief При необходимости заключает в кавычки ключ (имя свойства)
     * @param key
     * @return
     */
    static std::u32string enquote(const std::u32string &key);
    int64_t firstIndex(const PValue &v, int64_t start);
    int64_t lastIndex(const PValue &v, int64_t start);
    // это функция для методов map, forEach и т.п. - они отличаются только
    // способом обработки возвращаемого значения
    void mappedWorkerFunction(std::stack<StackValue> &args, Processor *p, std::function<void(const StackValue &)>);
};

} // namespace escript

#endif // ARRAYOBJECT_H
