/**
 * @file block.h
 * @brief Блок кода
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
#ifndef ESCRIPT_BLOCK_H
#define ESCRIPT_BLOCK_H

#include "EScript_global.h"
#include "symboltable.h"
#include <vector>
#include <memory>

namespace escript {

enum class BlockType
{
    Regular,
    Function,
};

/**
 * @brief Блок кода.
 */
class ESCRIPT_EXPORT Block : public std::enable_shared_from_this<Block>
{
private:
    std::shared_ptr<Unit> _unit;
    std::vector<std::shared_ptr<Block> > _blocks;
    std::shared_ptr<Block> _parentBlock;
    std::shared_ptr<SymbolTable> _symbolTable;
    BlockType _type { BlockType::Regular };
public:
    /**
     * @brief Создаёт новый экземпляр класса Block
     */
    Block(std::shared_ptr<Unit> unit, std::shared_ptr<Block> parent = nullptr);
    /**
     * @brief Освобождает связанные с этим экземпляром ресурсы
     */
    virtual ~Block();
    void clear();
    /**
     * @brief Добавить новый вложенный блок.
     * @return
     */
    std::shared_ptr<Block> addBlock();
    /**
     * @brief Возвращает таблицу символов этого блока
     * @return
     */
    std::shared_ptr<SymbolTable> symbolTable();
    /**
     * @brief Возвращает глобальный блок (самый верхний в иерархии)
     * @return
     */
    std::shared_ptr<Block> globalBlock();
    /**
     * @brief Добавляет смещение ко всем символам всех дочерних таблиц символов
     * @param offset смещение
     */
    void addOffset(uint64_t offset);
    std::vector<std::shared_ptr<Block> > &blocks();
    const std::shared_ptr<Block> &parentBlock() const;
    BlockType type() const;
    void setType(BlockType newType);
};

} // namespace escript

#endif // ESCRIPT_BLOCK_H
