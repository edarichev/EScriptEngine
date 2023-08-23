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
    checkDuplicates();
    for (auto &c : _strings) {
        delete c;
    }
    _strings.clear();
}

StringObject *StringContainer::add(const std::u32string &s)
{
    StringObject *str = findString(s);
    if (!str) {
        str = new StringObject(s);
        str->addRef();
        _strings.push_back(str);
        return _strings.back();
    }
    str->addRef();
    return str;
}

StringObject *StringContainer::add(StringObject *s)
{
    auto prev = findString(s->uString());
    if (prev && prev != s) {
        delete s;
        s = prev;
    }
    if (std::find(_strings.begin(), _strings.end(), s) == _strings.end()) {
        s->addRef();
        _strings.push_back(s);
    }
    return s;
}

void StringContainer::checkDuplicates() const
{
    for (auto it = _strings.begin(); it != prev(_strings.end()); ++it) {
        for (auto it2 = next(it); it2 != _strings.end(); ++it2) {
            auto s1 = *it;
            auto s2 = *it2;
            assert(s1 != s2);
        }
    }
}

StringObject *StringContainer::findString(const std::u32string &s) const
{
    // это поиск одинаковых строк, чтобы не делать их слишком много
    // и повторно использовать
    // TODO: этот поиск, а также и вставка, должны быть реализованы
    // более оптимально, пока сделано так, чтобы оно просто работало
    auto it = std::find_if(_strings.begin(), _strings.end(),
                        [&s](const StringObject *s1) { return s1->compare(s) == 0; } );
    if (it != _strings.end())
        return (*it);
    return nullptr;
}


} // namespace escript
