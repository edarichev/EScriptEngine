#include "stdafx.h"
#include "automationobject.h"
#include "processor.h"

namespace escript {

std::map<std::u32string, AutomationObject::pFn> AutomationObject::_fn;


AutomationObject::AutomationObject()
{
    buildFunctionsMap();
}

AutomationObject::~AutomationObject()
{
    _mark = 0xDEADBEEF;
}

bool AutomationObject::call(const std::u32string &method, Processor *p)
{
    auto ptrToMethod = _fn.find(method);
    if (ptrToMethod == _fn.end())
        return false;
    (this->*ptrToMethod->second)(p);
    return true;
}

void AutomationObject::release()
{
    // уменьшаем счётчик только до нуля,
    // если он равен 0, то объект больше не используется,
    // но он не удаляет сам себя, как в COM, его удаляет сборщик мусора
    assert(_counter != 0); // явно потребуем это
    if (_counter == 0)
        return;
    --_counter;
}

void AutomationObject::addRef() { ++_counter; }

int64_t AutomationObject::counter() const { return _counter; }

std::stack<StackValue> AutomationObject::loadArguments(Processor *p) const
{
    return p->loadArguments();
}


void AutomationObject::call_toString(Processor *p)
{
    auto args = loadArguments(p);
    if (args.empty())
        p->pushToStack(U"");
    else
        p->pushToStack(args.top().getStringValue());
}

void AutomationObject::buildFunctionsMap()
{
    if (!_fn.empty())
        return;
    _fn[U"toString"] = &AutomationObject::call_toString;
}

} // namespace escript
