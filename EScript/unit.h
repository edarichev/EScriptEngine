/**
 * @file unit.h
 * @brief Программный модуль
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
#ifndef UNIT_H
#define UNIT_H

#include "EScript_global.h"
#include "block.h"
#include <forward_list>
#include <memory>

namespace escript {

/**
 * @brief Программный модуль.
 */
class ESCRIPT_EXPORT Unit : public std::enable_shared_from_this<Unit>
{
private:
    std::shared_ptr<Block> _block;
public:
    /**
     * @brief Создаёт новый экземпляр класса Unit
     */
    Unit();
    /**
     * @brief Освобождает связанные с этим экземпляром ресурсы
     */
    virtual ~Unit();
    /**
     * @brief Очищает все данные в этом модуле.
     */
    void clear();
    /**
     * @brief Возвращает корневой блок этого модуля (глобальная область видимости).
     * @return
     */
    std::shared_ptr<Block> block();
};

} // namespace escript

#endif // UNIT_H
