#include "stdafx.h"
#include "stringcontainer.h"
#include "stringobject.h"

namespace escript {

StringContainer::StringContainer()
{

}

StringContainer::~StringContainer()
{
    for (auto &c : _strings) {
        delete c;
    }
}

StringObject *StringContainer::add(const std::u32string &s)
{
    _strings.push_back(new StringObject(s));
    return _strings.back();
}

StringObject *StringContainer::add(StringObject *s)
{
    _strings.push_back(s);
    return s;
}


} // namespace escript
