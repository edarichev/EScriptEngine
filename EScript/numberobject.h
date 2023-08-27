/**
 * @file numberobject.h
 * @brief Определение класса для объекта Number.
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
#ifndef NUMBEROBJECT_H
#define NUMBEROBJECT_H

#include "automationobject.h"

namespace escript {

class ESCRIPT_EXPORT Number : public AutomationObject
{
    using BaseClass = AutomationObject;
    typedef void (Number::*pFn)(Processor *p);
    static std::map<std::u32string, pFn> _fn;

    SymbolType _type;
    union {
        int64_t intValue = 0;
        double realValue;
    };

public:
    /**
     * @brief Создаёт новый экземпляр класса Number
     */
    Number();
    Number(int64_t i) : _type(SymbolType::Integer), intValue(i) { };
    Number(double d) : _type(SymbolType::Real), realValue(d) { };
    /**
     * @brief Освобождает связанные с этим экземпляром ресурсы
     */
    virtual ~Number();


    // AutomationObject interface
public:
    virtual bool call(const std::u32string &method, Processor *p) override;
private:
    static void buildFunctionsMap();
    void call_parseInt(Processor *p);
    void call_parseFloat(Processor *p);
    void call_toString(Processor *p);
    std::u32string toString(int64_t value, int radix);
    std::u32string toString(double value, int radix);
};


} // namespace escript

#endif // NUMBEROBJECT_H
