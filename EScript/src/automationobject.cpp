/**
 * @file automationobject.cpp
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
#include "stdafx.h"
#include "automationobject.h"
#include "processor.h"

namespace escript {

std::map<std::u32string, AutomationObject::pFn> AutomationObject::_fn;


bool AutomationObject::managed() const
{
    return _managed;
}

const std::u32string AutomationObject::toString() const
{
    return U"Object";
}

AutomationObject::AutomationObject()
{
    buildFunctionsMap();
}

AutomationObject::~AutomationObject()
{
    _mark = OBJECT_REMOVED_MARK;
}

bool AutomationObject::call(const std::u32string &method, Processor *p)
{
    auto ptrToMethod = _fn.find(method);
    if (ptrToMethod == _fn.end())
        return false;
    (this->*ptrToMethod->second)(p);
    return true;
}

void AutomationObject::release()
{
    // уменьшаем счётчик только до нуля,
    // если он равен 0, то объект больше не используется,
    // но он не удаляет сам себя, как в COM, его удаляет сборщик мусора
    assert(_counter >= 0); // явно потребуем это
    if (_counter == 0)
        return;
    --_counter;
}

void AutomationObject::addRef() { ++_counter; }

int64_t AutomationObject::counter() const { return _counter; }

std::stack<StackValue> AutomationObject::loadArguments(Processor *p) const
{
    return p->loadArguments();
}

bool AutomationObject::exists(AutomationObject *pObject)
{
    return pObject->_mark == OBJECT_EXISTS_MARK;
}


void AutomationObject::call_toString(Processor *p)
{
    auto args = loadArguments(p);
    if (args.empty())
        p->pushToStack(U"");
    else
        p->pushToStack(args.top().getStringValue());
}

void AutomationObject::buildFunctionsMap()
{
    if (!_fn.empty())
        return;
    _fn[U"toString"] = &AutomationObject::call_toString;
}

} // namespace escript
