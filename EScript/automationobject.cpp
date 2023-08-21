#include "stdafx.h"
#include "automationobject.h"
#include "processor.h"

namespace escript {

std::stack<StackValue> AutomationObject::loadArguments(Processor *p) const
{
    return p->loadArguments();
}



} // namespace escript
