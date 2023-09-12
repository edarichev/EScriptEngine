/**
 * @file engineerrors.cpp
 * @brief Классы исключений
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
#include "engineerrors.h"

#define STR_ARGUMENT_NULL "Argument is null"

namespace escript {

Exception::Exception()
{

}

Exception::Exception(int line, const char *file)
    : _line(line), _file(file)
{

}

Exception::~Exception(){}

ArgumentNullException::ArgumentNullException(int line, const char *file)
    : BaseClass(line, file)
{

}

SyntaxError::SyntaxError(int line, int pos, const std::string_view &msg)
    : _srcLine(line), _pos(pos), _msg(msg)
{
    std::stringstream ss;
    ss << "Line: " << _srcLine
       << ", position: " << _pos
       << ": " << _msg;
    _fullMessage = ss.str();
}

const char *SyntaxError::what() const noexcept
{
    return _fullMessage.c_str();
}


} // namespace escript
