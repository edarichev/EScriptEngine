/**
 * @file block.cpp
 * @brief Блок кода (реализация)
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
