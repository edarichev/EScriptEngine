#include "stdafx.h"
#include "stringcontainer.h"
#include "stringobject.h"

namespace escript {

StringContainer::StringContainer()
{

}

StringContainer::~StringContainer()
{
    clear();
}

void StringContainer::clear()
{
    for (auto &c : _strings) {
        delete c;
    }
    _strings.clear();
}

StringObject *StringContainer::add(const std::u32string &s)
{
    _strings.push_back(new StringObject(s));
    return _strings.back();
}

StringObject *StringContainer::add(StringObject *s)
{
    if (std::find(_strings.begin(), _strings.end(), s) == _strings.end())
        _strings.push_back(s);
    return s;
}


} // namespace escript
