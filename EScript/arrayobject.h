/**
 * @file arrayobject.h
 * @brief Определение класса Array.
 *
 * Eugen-Script
 * The ECMAScript-like engine for C++ programs.
 * Copyright (C) 2023, Evgeny Darichev
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 */
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
    // индексные элементы, индекс соответствует позиции в этом векторе
    std::vector<PValue> _indexedItems;
    // именованные элементы
    std::map<std::u32string, PValue> _namedItems;

    using BaseClass = AutomationObject;
    using pFn = void (Array::*)(Processor *p);
    // карта функций этого класса
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
    /**
     * @brief Возвращает индексный элемент, находящийся на указанной позиции
     * @param index индекс от 0 и не более числа элементов, хранящихся в массиве
     * @return если индекс выходит за пределы массива, возвращается пустой
     *         объект PValue; если индекс отрицателен, он переводится в строку
     *         и ищется как именованный.
     */
    PValue get(int64_t index);
    PValue get(const std::u32string &index);
    void set(int64_t index, PValue value);
    void set(int64_t index, SymbolType t, uint64_t value);
    void set(const std::u32string &index, PValue value);
    int64_t length() const;
    void add(PValue value);
    const std::u32string toString() const override;
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
    // возврат true одначает прервать цикл
    void mappedWorkerFunction(std::stack<StackValue> &args, Processor *p, std::function<bool (const StackValue &)>);
};

} // namespace escript

#endif // ARRAYOBJECT_H
