#include "stdafx.h"
#include "mycellstyle.h"

std::map<std::u32string, MyCellStyle::pFn> MyCellStyle::_fn;


MyCellStyle::MyCellStyle(std::shared_ptr<MyCell> cell)
    : _cell(cell)
{
    buildFunctionsMap();
}

bool MyCellStyle::call(const std::u32string &method, Processor *p)
{
    if (BaseClass::call(method, p))
        return true;
    auto ptrToMethod = _fn.find(method);
    if (ptrToMethod == _fn.end())
        throw std::domain_error("Call of unknown method: MyStyle." + to_utf8(method));
    (this->*ptrToMethod->second)(p);
    return true;
}

void MyCellStyle::buildFunctionsMap()
{
    if (!_fn.empty())
        return;
    _fn[U"set_color"] = &MyCellStyle::call_set_color;
    _fn[U"get_color"] = &MyCellStyle::call_get_color;
}

void MyCellStyle::call_get_color(Processor *p)
{
    auto args = loadArguments(p);
    assert(args.empty());
    if (!_color)
        _color = std::make_shared<MyCellColor>(_cell);
    p->pushToStack(SymbolType::Object, (uint64_t)_color.get());
}

void MyCellStyle::call_set_color(Processor *p)
{
    auto args = loadArguments(p);
    assert(!args.empty());
    if (!_color)
        _color = std::make_shared<MyCellColor>(_cell);
    _color->set(args.top().getStringValue());
    p->pushToStack(0);
}
