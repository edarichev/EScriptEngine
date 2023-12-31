﻿/**
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
#ifndef ENGINEERRORS_H
#define ENGINEERRORS_H

#include "EScript_global.h"

namespace escript {

class ESCRIPT_EXPORT Exception : public std::exception
{
    int _line;
    std::string _file;
public:
    Exception();
    /**
     * @brief Общая ошибка
     * @param line подставьте __LINE__
     * @param file подставьте __FILE__
     */
    Exception(int line, const char *file);
    virtual ~Exception();
};

class ESCRIPT_EXPORT ArgumentNullException : public Exception
{
    using BaseClass = Exception;
public:
    ArgumentNullException(int line, const char *file);
};

class ESCRIPT_EXPORT SyntaxError : public Exception
{
    int _srcLine = 0;
    int _pos = 0;
    std::string _msg;
    std::string _fullMessage;
public:
    /**
     * @brief Синтаксическая ошибка
     * @param line номер строки в тексте скрипта
     * @param pos позиция от начала строки в тексте скрипта
     * @param msg дополнительное сообщение
     */
    SyntaxError(int line, int pos, const std::string_view &msg);
    virtual const char* what() const noexcept override;
};

} // namespace escript

#endif // ENGINEERRORS_H
