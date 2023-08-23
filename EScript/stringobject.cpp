#include "stdafx.h"
#include "stringobject.h"
#include "processor.h"

namespace escript {

std::map<std::u32string, StringObject::pFn> StringObject::_fn;

void StringObject::buildFunctionsMap()
{
    if (_fn.empty()) {
        _fn[U"get_length"] = &StringObject::call_get_length;
        _fn[U"at"] = &StringObject::call_at;
        _fn[U"charAt"] = &StringObject::call_charAt;
        _fn[U"charCodeAt"] = &StringObject::call_charCodeAt;
        _fn[U"endsWith"] = &StringObject::call_endsWith;
        _fn[U"get"] = &StringObject::call_get;
        _fn[U"includes"] = &StringObject::call_includes;
        _fn[U"indexOf"] = &StringObject::call_indexOf;
        _fn[U"lastIndexOf"] = &StringObject::call_lastIndexOf;
        _fn[U"padEnd"] = &StringObject::call_padEnd;
        _fn[U"padStart"] = &StringObject::call_padStart;
        _fn[U"repeat"] = &StringObject::call_repeat;
        _fn[U"replace"] = &StringObject::call_replace;
        _fn[U"replaceAll"] = &StringObject::call_replaceAll;
        _fn[U"set"] = &StringObject::call_set;
        _fn[U"slice"] = &StringObject::call_slice;
        _fn[U"split"] = &StringObject::call_split;
        _fn[U"startsWith"] = &StringObject::call_startsWith;
        _fn[U"substring"] = &StringObject::call_substring;
        _fn[U"toLowerCase"] = &StringObject::call_toLowerCase;
        _fn[U"toUpperCase"] = &StringObject::call_toUpperCase;
        _fn[U"trim"] = &StringObject::call_trim;
        _fn[U"trimStart"] = &StringObject::call_trimStart;
        _fn[U"trimEnd"] = &StringObject::call_trimEnd;
    }
}

StringObject::StringObject()
{
    _managed = true;
    buildFunctionsMap();
}

StringObject::StringObject(const std::u32string &s) :
    _s(s)
{
    _managed = true;
    buildFunctionsMap();
}

StringObject::StringObject(std::u32string &&s)
    : _s(std::move(s))
{
    _managed = true;
    buildFunctionsMap();
}

bool StringObject::call(const std::u32string &method, Processor *p)
{
    if (BaseClass::call(method, p))
        return true;
    auto ptrToMethod = _fn.find(method);
    if (ptrToMethod == _fn.end())
        throw std::domain_error("Call of unknown method: string." + to_utf8(method));
    (this->*ptrToMethod->second)(p);
    return true;
}

int64_t StringObject::at(int64_t i) const
{
    return _s.at(i);
}

int64_t StringObject::charCodeAt(int64_t i) const
{
    return _s.at(i);
}

std::u32string StringObject::charAt(int64_t i) const
{
    if (i < 0 || i >= (int64_t)_s.length())
        return U"";
    return std::u32string(1, _s[i]);
}

bool StringObject::endsWith(const std::u32string &s) const
{
    if (s.length() > _s.length())
        return false;
    return _s.rfind(s) == _s.length() - s.length();
}

int StringObject::compare(StringObject *s) const
{
    return _s.compare(s->_s);
}

int StringObject::compare(const std::u32string &s) const
{
    return _s.compare(s);
}

bool StringObject::includes(const std::u32string &s) const
{
    return _s.find(s) != std::u32string::npos;
}

int64_t StringObject::indexOf(const std::u32string &s) const
{
    return _s.find(s);
}

int64_t StringObject::lastIndexOf(const std::u32string &s) const
{
    return _s.rfind(s);
}

std::u32string StringObject::padEnd(int64_t width, const std::u32string &s) const
{
    if (width <= (int64_t)_s.length())
        return _s;
    std::u32string t(_s);
    std::u32string filler(s.empty() ? U" " : s);
    size_t i = 0;
    while ((int64_t)t.length() < width) {
        t.push_back(filler[i]);
        i++;
        if (i == filler.length())
            i = 0;
    }
    return t;
}

std::u32string StringObject::padStart(int64_t width, const std::u32string &s) const
{
    if (width <= (int64_t)_s.length())
        return _s;
    int64_t addCount = width - _s.length();
    std::u32string t;
    std::u32string filler(s.empty() ? U" " : s);
    size_t i = 0;
    while (addCount-- > 0) {
        t.push_back(filler[i]);
        i++;
        if (i == filler.length())
            i = 0;
    }
    t.append(_s);
    return t;
}

std::u32string StringObject::repeat(int64_t i) const
{
    if (i == 0)
        return _s;
    std::u32string t;
    while (i-- > 0) {
        t.append(_s);
    }
    return t;
}

std::u32string StringObject::replace(const std::u32string &what, const std::u32string &with) const
{
    std::size_t pos = _s.find(what);
    if (pos == std::u32string::npos)
        return _s;
    std::u32string t(_s);
    t.replace(pos, what.length(), with);
    return t;
}

std::u32string StringObject::replaceAll(const std::u32string &what, const std::u32string &with) const
{
    std::size_t pos = _s.find(what);
    if (pos == std::u32string::npos)
        return _s;
    std::u32string t(_s);
    while ((pos = t.find(what)) != std::u32string::npos)
        t.replace(pos, what.length(), with);
    return t;
}

std::u32string StringObject::slice(int64_t from, int64_t to) const
{
    if (from < 0)
        from = _s.length() + from;
    if (to >= (int64_t)_s.length())
        to = _s.length();
    if (to < 0)
        to = _s.length() + to;
    if (to < from)
        return U"";
    return _s.substr(from, to - from);
}

bool StringObject::startsWith(const std::u32string &s, int64_t from) const
{
    return _s.find(s, from) == 0;
}

std::u32string StringObject::toLowerCase() const
{
    std::u32string t(_s);
    std::transform(t.begin(), t.end(), t.begin(), [](char32_t c){ return std::tolower(c); });
    return t;
}

std::u32string StringObject::toUpperCase() const
{
    std::u32string t(_s);
    std::transform(t.begin(), t.end(), t.begin(), [](char32_t c){ return std::toupper(c); });
    return t;
}

std::u32string StringObject::trim() const
{
    auto itLeft = _s.cbegin();
    for (; itLeft != _s.cend(); ++itLeft) {
        if (!std::isspace(*itLeft))
            break;
    }
    auto itRight = _s.crbegin();
    for (; itRight != std::make_reverse_iterator(itLeft); ++itRight) {
        if (!std::isspace(*itRight))
            break;
    }
    return std::u32string(itLeft, itRight.base());
}

std::u32string StringObject::trimStart() const
{
    auto itLeft = _s.cbegin();
    for (; itLeft != _s.cend(); ++itLeft) {
        if (!std::isspace(*itLeft))
            break;
    }
    return std::u32string(itLeft, _s.end());
}

std::u32string StringObject::trimEnd() const
{
    auto itRight = _s.crbegin();
    for (; itRight != _s.crend(); ++itRight) {
        if (!std::isspace(*itRight))
            break;
    }
    return std::u32string(_s.begin(), itRight.base());
}

Array *StringObject::split(const std::u32string &sep)
{
    Array *arr = new Array();
    arr->addRef();
    std::vector<std::u32string> parts;
    if (sep.length()) {
        // строка ненулевой длины, делим как обычно
        size_t prevPos = 0;
        size_t pos = _s.find(sep);
        while (pos != std::u32string::npos) {
            parts.emplace_back(_s.begin() + prevPos, _s.begin() + pos);
            prevPos = pos + sep.length();
            pos = _s.find(sep, prevPos);
        }
        parts.emplace_back(_s.begin() + prevPos, _s.end());
    } else {
        // строка нулевой длины, нужно поделить на буквы
        for (auto &c : _s) {
            parts.emplace_back(1, c);
        }
    }
    for (int64_t i = 0; i < (int64_t)parts.size(); i++) {
        arr->set(i, PValue(SymbolType::String,
                    (uint64_t)new StringObject(std::move(parts[i]))));
    }
    return arr;
}

Array *StringObject::split()
{
    Array *arr = new Array();
    arr->addRef();
    arr->set(0, PValue(SymbolType::String,
                (uint64_t)new StringObject(_s)));
    return arr;
}

int64_t StringObject::length() const
{
    return _s.length();
}

StringObject::StringObject(const char32_t *str)
{
    _s = str;
}

StringObject &StringObject::operator=(const std::u32string &str)
{
    _s = str;
    return *this;
}

bool StringObject::operator==(const std::u32string &s) const
{
    return s == _s;
}

bool StringObject::operator==(const char32_t *s) const
{
    return s == _s;
}

const std::u32string &StringObject::uString() const
{
    return _s;
}

StringObject *StringObject::concat(StringObject *s1, StringObject *s2)
{
    StringObject *newString = new StringObject(s1->_s + s2->_s);
    return newString;
}

std::u32string StringObject::substring(int64_t fromIndex, int64_t toIndex) const
{
    if (fromIndex < 0)
        fromIndex = 0;
    if (toIndex > (int64_t) _s.length() || toIndex < fromIndex)
        toIndex = _s.length();
    return _s.substr(fromIndex, toIndex - fromIndex);
}

void StringObject::call_get_length(Processor *p)
{
    // вытащить из стека аргументы
    // число аргументов, здесь д.б. 0, просто пропустить
    auto args = loadArguments(p);
    int64_t p1 = length();
    p->pushToStack(p1); // результат или 0
}

void StringObject::call_at(Processor *p)
{
    // метод с аргументами
    // вытащить из стека аргументы
    // число аргументов, пока не нужно, просто пропустить
    auto args = loadArguments(p);
    int64_t param1 = args.empty() ? 0 : args.top().getIntValue();
    int64_t c = at(param1);
    p->pushToStack(c); // результат или 0
}

void StringObject::call_charAt(Processor *p)
{
    // метод с аргументами
    // вытащить из стека аргументы
    // число аргументов, пока не нужно, просто пропустить
    auto args = loadArguments(p);
    int64_t param1 = args.empty() ? 0 : args.top().getIntValue();
    p->pushToStack(charAt(param1));
}

void StringObject::call_charCodeAt(Processor *p)
{
    // метод с аргументами
    // вытащить из стека аргументы
    // число аргументов, пока не нужно, просто пропустить
    auto args = loadArguments(p);
    int64_t param1 = args.empty() ? 0 : args.top().getIntValue();
    int64_t c = at(param1);
    p->pushToStack(c); // результат или 0
}

void StringObject::call_endsWith(Processor *p)
{
    auto args = loadArguments(p);
    if (!args.empty()) {
        auto value = args.top().getStringValue();
        bool result = endsWith(value);
        p->pushBooleanToStack(result);
    } else {
        p->pushBooleanToStack(false);
    }
}

void StringObject::call_get(Processor *p)
{
    auto args = loadArguments(p);
    if (args.empty())
        p->pushToStack(U"");
    else {
        p->pushToStack(charAt(args.top().getIntValue()));
    }
}

void StringObject::call_set(Processor *p)
{
    auto args = loadArguments(p);
    if (args.empty())
        p->pushToStack(U"");
    else {
        int64_t index = args.top().getIntValue();
        args.pop();
        if ((index < 0 || index > (int64_t) _s.length()) ||
                args.empty()) {
            p->pushToStack(0);
            return;
        }
        auto value = args.top();
        int charCode = 0;
        do {
            switch (value.type) {
            case SymbolType::Integer:
                charCode = value.value;
                break;
            case SymbolType::Real:
                charCode = (int)bit_cast<double>(value.value);
                break;
            case SymbolType::String: {
                auto str = (StringObject*)value.value;
                charCode = str->_s.length() == 0 ? ' ' : str->_s[0];
                break;
            }
            case SymbolType::Variable: {
                ObjectRecord *rec = (ObjectRecord*)value.value;
                value.type = rec->type;
                value.value = rec->data;
                continue;
            }
            default:
                throw std::domain_error("Can not convert value to character in string.set");
            }
            break;
        } while (true);
        _s[index] = charCode;
        p->pushToStack(0);
    }
}

void StringObject::call_includes(Processor *p)
{
    auto args = loadArguments(p);
    if (!args.empty()) {
        auto value = args.top().getStringValue();
        bool result = includes(value);
        p->pushBooleanToStack(result);
    } else {
        p->pushBooleanToStack(false);
    }
}

void StringObject::call_indexOf(Processor *p)
{
    auto args = loadArguments(p);
    if (!args.empty()) {
        auto value = args.top().getStringValue();
        int64_t result = indexOf(value);
        p->pushToStack(result);
    } else {
        p->pushToStack(-1);
    }
}

void StringObject::call_lastIndexOf(Processor *p)
{
    auto args = loadArguments(p);
    if (!args.empty()) {
        auto value = args.top().getStringValue();
        int64_t result = lastIndexOf(value);
        p->pushToStack(result);
    } else {
        p->pushToStack(-1);
    }
}

void StringObject::call_padEnd(Processor *p)
{
    // если аргумент один, то
    // если это целое - то пробелы
    // если аргументов два, то:
    // первый - это ширина, второй - заполнитель
    auto args = loadArguments(p);
    if (args.size() == 0) {
        p->pushStringToStack(new StringObject(_s));
        return;
    }
    int64_t width = args.top().getIntValue();
    args.pop();
    std::u32string fill = args.empty() ? U" " : args.top().getStringValue();
    p->pushToStack(padEnd(width, fill));
}

void StringObject::call_padStart(Processor *p)
{
    auto args = loadArguments(p);
    if (args.empty()) {
        p->pushToStack(_s);
        return;
    }
    // если аргумент один, то
    // если это целое - то пробелы
    // если аргументов два, то:
    // первый - это ширина, второй - заполнитель
    int64_t width = args.top().getIntValue();
    args.pop();
    std::u32string fill = args.empty() ? U" " : args.top().getStringValue();
    p->pushToStack(padStart(width, fill));
}

void StringObject::call_repeat(Processor *p)
{
    auto args = loadArguments(p);
    if (args.empty()) {
        p->pushToStack(_s);
        return;
    }
    auto n = args.top().getIntValue();
    p->pushToStack(repeat(n));
}

void StringObject::call_replace(Processor *p)
{
    auto args = loadArguments(p);
    if (args.empty()) {
        p->pushToStack(_s);
        return;
    }
    auto what = args.top().getStringValue();
    args.pop();
    auto with = args.empty() ? U"undefined" : args.top().getStringValue();
    p->pushToStack(replace(what, with));
}

void StringObject::call_replaceAll(Processor *p)
{
    auto args = loadArguments(p);
    if (args.empty()) {
        p->pushToStack(_s);
        return;
    }
    auto what = args.top().getStringValue();
    args.pop();
    auto with = args.empty() ? U"undefined" : args.top().getStringValue();
    p->pushToStack(replaceAll(what, with));
}

void StringObject::call_slice(Processor *p)
{
    auto args = loadArguments(p);
    if (args.empty()) {
        p->pushToStack(_s);
        return;
    }
    auto from = args.top().getIntValue();
    args.pop();
    auto to = args.empty() ? _s.length() : args.top().getIntValue();
    p->pushToStack(slice(from, to));
}

void StringObject::call_split(Processor *p)
{
    auto args = loadArguments(p);
    if (args.empty()) {
        p->pushArrayToStack(split());
        return;
    }
    p->pushArrayToStack(split(args.top().getStringValue()));
}

void StringObject::call_startsWith(Processor *p)
{
    auto args = loadArguments(p);
    if (!args.empty()) {
        auto value = args.top().getStringValue();
        args.pop();
        int64_t from = args.empty() ? 0 : args.top().getIntValue();
        p->pushBooleanToStack(startsWith(value, from));
    } else {
        p->pushBooleanToStack(false);
    }
}

void StringObject::call_substring(Processor *p)
{
    auto args = loadArguments(p);
    int64_t fromIndex = 0, toIndex = _s.length();
    if (!args.empty()) {
        fromIndex = args.top().getIntValue();
        args.pop();
    }
    if (!args.empty()) {
        toIndex = args.top().getIntValue();
    }
    p->pushToStack(substring(fromIndex, toIndex));
}

void StringObject::call_toLowerCase(Processor *p)
{
    loadArguments(p);
    p->pushToStack(toLowerCase());
}

void StringObject::call_toUpperCase(Processor *p)
{
    loadArguments(p);
    p->pushToStack(toUpperCase());
}

void StringObject::call_trim(Processor *p)
{
    loadArguments(p);
    p->pushToStack(trim());
}

void StringObject::call_trimStart(Processor *p)
{
    loadArguments(p);
    p->pushToStack(trimStart());
}

void StringObject::call_trimEnd(Processor *p)
{
    loadArguments(p);
    p->pushToStack(trimEnd());
}





} // namespace escript
