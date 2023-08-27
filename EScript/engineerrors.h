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
#ifndef ENGINEERRORS_H
#define ENGINEERRORS_H

#include "EScript_global.h"

namespace escript {

class ESCRIPT_EXPORT ArgumentNullException : public std::domain_error
{
    using BaseClass = std::domain_error;
public:
    ArgumentNullException(const char *file, int line);
};

} // namespace escript

#endif // ENGINEERRORS_H
