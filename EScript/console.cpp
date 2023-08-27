/**
 * @file console.cpp
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
#include "stdafx.h"
#include "console.h"
#include "processor.h"

namespace escript {

bool Console::separateWithSpace() const
{
    return _separateWithSpace;
}

void Console::setSeparateWithSpace(bool newSeparateWithSpace)
{
    _separateWithSpace = newSeparateWithSpace;
}

Console::Console()
{
    _managed = true;
    setOutputStream(&std::cout);
}

Console::~Console()
{

}

void Console::log(std::vector<std::u32string> &args)
{
    std::ostream &s = _outStream ? *_outStream : std::cout;
    for (size_t i = 0; i < args.size(); i++) {
        s << to_utf8(args[i]);
        if (separateWithSpace()) {
            if (i < args.size() - 1)
                s << " ";
        }
    }
    s << std::endl;
}

void Console::setOutputStream(std::ostream *s)
{
    _outStream = s;
}


} // namespace escript


bool escript::Console::call(const std::u32string &method, Processor *p)
{
    if (BaseClass::call(method, p))
        return true;
    if (method == U"log") {
        auto nArgs = p->popFromStack().value;
        std::vector<std::u32string> args;
        while (nArgs > 0) {
            auto arg = p->popFromStack();
            args.insert(args.begin(), arg.getStringValue());
            nArgs--;
        }
        log(args);
        p->pushToStack(0); // OK
        return true;
    }
    throw std::domain_error("Call of unknown method: console." + to_utf8(method));
}
