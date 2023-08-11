#include "stdafx.h"
#include "function.h"

namespace escript {


Function::Function()
{

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


} // namespace escript
