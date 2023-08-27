/**
 * @file functionobject.h
 * @brief Определение класса функции.
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
#ifndef ESCRIPT_FUNCTION_H
#define ESCRIPT_FUNCTION_H

#include "EScript_global.h"
#include "automationobject.h"

namespace escript {

class Symbol;

/**
 * @brief Представляет объект Function
 */
class ESCRIPT_EXPORT Function : public AutomationObject
{
    uint64_t _callAddress {};
    std::u32string _name;
    Symbol *_symbol = nullptr;
public:
    /**
     * @brief Создаёт новый экземпляр класса Function
     */
    Function(Symbol *sym);

    /**
     * @brief Освобождает связанные с этим экземпляром ресурсы
     */
    virtual ~Function();
    uint64_t callAddress() const;
    void setCallAddress(uint64_t newCallAddress);
    const std::u32string &name() const;
    void setName(const std::u32string &newName);
    void addOffset(int64_t offset);
};

} // namespace escript

#endif // ESCRIPT_FUNCTION_H
