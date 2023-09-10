/**
 * @file block.cpp
 * @brief Блок кода (реализация)
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
#include "block.h"

namespace escript {

BlockType Block::type() const
{
    return _type;
}

void Block::setType(BlockType newType)
{
    _type = newType;
}

Block::Block(std::shared_ptr<Unit> unit, std::shared_ptr<Block> parent)
    : _unit(unit), _parentBlock(parent)
{
    std::shared_ptr<SymbolTable> parentTable =
            parent == nullptr ? nullptr : parent->_symbolTable;
    _symbolTable = std::make_shared<SymbolTable>(_unit, parentTable);
}

Block::~Block()
{

}

void Block::clear()
{
    _symbolTable->clear();
    for (auto &c : _blocks) {
        c->clear();
    }
}

const std::shared_ptr<Block> &Block::parentBlock() const
{
    return _parentBlock;
}

std::shared_ptr<SymbolTable> Block::symbolTable()
{
    return _symbolTable;
}

std::shared_ptr<Block> Block::globalBlock()
{
    std::shared_ptr<Block> parent = _parentBlock;
    while (parent) {
        if (!parent->_parentBlock)
            return parent;
        parent = parent->_parentBlock;
    }
    return shared_from_this();
}

void Block::addOffset(uint64_t offset)
{
    _symbolTable->addOffset(offset);
    for (auto &b : _blocks)
        b->addOffset(offset);
}

std::shared_ptr<Block> Block::addBlock()
{
    std::shared_ptr<Block> newBlock = std::make_shared<Block>(_unit, shared_from_this());
    _blocks.push_back(newBlock);
    return newBlock;
}

std::vector<std::shared_ptr<Block> > &Block::blocks()
{
    return _blocks;
}

} // namespace escript
