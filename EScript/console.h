/**
 * @file console.h
 * @brief Класс для объекта "console"
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
#ifndef ESCRIPT_CONSOLE_H
#define ESCRIPT_CONSOLE_H

#include <vector>
#include "automationobject.h"

namespace escript {

class ESCRIPT_EXPORT Console : public escript::AutomationObject
{
    using BaseClass = AutomationObject;
    std::ostream *_outStream = nullptr;
    bool _separateWithSpace = true;
public:
    /**
     * @brief Создаёт новый экземпляр класса Console
     */
    Console();
    /**
     * @brief Освобождает связанные с этим экземпляром ресурсы
     */
    virtual ~Console();
    void log(std::vector<std::u32string> &args);
    void setOutputStream(std::ostream *s);
    // AutomationObject interface
public:
    virtual bool call(const std::u32string &method, Processor *p) override;
    bool separateWithSpace() const;
    void setSeparateWithSpace(bool newSeparateWithSpace);
};

} // namespace escript

#endif // ESCRIPT_CONSOLE_H
