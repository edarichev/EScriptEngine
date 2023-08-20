#include "stdafx.h"
#include "automationobject.h"
#include "processor.h"

namespace escript {

std::stack<StackValue> AutomationObject::loadArguments(Processor *p) const
{
    int64_t nArgs = p->popFromStack().getIntValue();
    std::stack<StackValue> args;
    while (nArgs-- > 0)
        args.push(p->popFromStack());
    return args;
}



} // namespace escript
