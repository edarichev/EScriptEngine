/**
 * @file automationobject.h
 * @brief Базовый класс для объектов автоматизации.
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
#ifndef ESCRIPT_AUTOMATIONOBJECT_H
#define ESCRIPT_AUTOMATIONOBJECT_H

#include "EScript_global.h"
#include "stackvalue.h"

#define OBJECT_EXISTS_MARK 0xCCCCCCCC
#define OBJECT_REMOVED_MARK 0xDEADBEEF

namespace escript {

class EScript;
class Processor;
class AutomationObject;

using ConstructorFunction = AutomationObject* (*)(Processor *);

/**
 * @brief Базовый класс для всех обёрток, в т.ч. классов в виртуальной машине.
 * @details Чтобы сделать свойство, имя метода должно начинаться с "get_"
 */
class ESCRIPT_EXPORT AutomationObject
{
    unsigned int _mark = OBJECT_EXISTS_MARK;
    typedef void (AutomationObject::*pFn)(Processor *p);
    static std::map<std::u32string, pFn> _fn;
    int64_t _counter = 0;
protected:
    bool _managed = false;
public:
    AutomationObject();
    virtual ~AutomationObject();
    virtual bool call([[maybe_unused]] const std::u32string &method,
                      [[maybe_unused]] Processor *p);
    void release();
    void addRef();
    int64_t counter() const;;

    std::stack<StackValue> loadArguments(Processor *p) const;
    static bool exists(AutomationObject *pObject);
    /**
     * @brief Возвращает true, если этот объект может быть удалён автоматически
     *        при разрушении объекта движка.
     * @details Обычно true установлено у встроенных (стандартных) объектов:
     *        StringObject, Console, Number, Math, Function, Array.
     *        По умолчанию равно false,
     *        так что объекты производных пользовательских классов
     *        могут не беспокоиться, что они будут удалены движком.
     * @return
     */
    bool managed() const;
    virtual const std::u32string toString() const;
private:
    void call_toString(Processor *p);
    static void buildFunctionsMap();
};


} // namespace escript

#endif // ESCRIPT_AUTOMATIONOBJECT_H
