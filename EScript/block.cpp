/**
 * @file block.cpp
 * @brief Блок кода (реализация)
 */
#include "stdafx.h"
#include "block.h"

namespace escript {

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

std::shared_ptr<SymbolTable> Block::symbolTable()
{
    return _symbolTable;
}

std::shared_ptr<Block> Block::addBlock()
{
    std::shared_ptr<Block> newBlock = std::make_shared<Block>(_unit, shared_from_this());
    _blocks.push_back(newBlock);
    return newBlock;
}


} // namespace escript
