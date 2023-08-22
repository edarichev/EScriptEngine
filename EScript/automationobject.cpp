#include "stdafx.h"
#include "automationobject.h"
#include "processor.h"

namespace escript {

std::map<std::u32string, AutomationObject::pFn> AutomationObject::_fn;

AutomationObject::AutomationObject()
{
    buildFunctionsMap();
}

bool AutomationObject::call(const std::u32string &method, Processor *p)
{
    auto ptrToMethod = _fn.find(method);
    if (ptrToMethod == _fn.end())
        return false;
    (this->*ptrToMethod->second)(p);
    return true;
}

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
