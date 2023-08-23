#include "stdafx.h"
#include "function.h"

namespace escript {


Function::Function(Symbol *sym) : _symbol(sym)
{
    _managed = true;
}

Function::~Function()
{

}

uint64_t Function::callAddress() const
{
    return _callAddress;
}

void Function::setCallAddress(uint64_t newCallAddress)
{
    _callAddress = newCallAddress;
}

const std::u32string &Function::name() const
{
    return _name;
}

void Function::setName(const std::u32string &newName)
{
    _name = newName;
}

void Function::addOffset(int64_t offset)
{
    _callAddress += offset;
}


} // namespace escript
