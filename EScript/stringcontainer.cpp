#include "stdafx.h"
#include "stringcontainer.h"

namespace escript {

StringContainer::StringContainer()
{

}

StringContainer::~StringContainer()
{

}

const std::u32string &StringContainer::get(int index) const
{
    return _strings[index];
}

size_t StringContainer::add(std::u32string s)
{
    _strings.push_back(std::move(s));
    return _strings.size() - 1;
}


} // namespace escript
