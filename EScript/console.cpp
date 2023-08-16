#include "stdafx.h"
#include "console.h"
#include "processor.h"

namespace escript {

Console::Console()
{
    setOutputStream(std::cout);
}

Console::~Console()
{

}

void Console::log(std::vector<std::u32string> &args)
{
    std::ostream &s = *_outStream;
    for (size_t i = 0; i < args.size(); i++) {
        s << to_utf8(args[i]);
        if (i < args.size() - 1)
            s << " ";
    }
    s << std::endl;
}

void Console::setOutputStream(std::ostream &s)
{
    _outStream = &s;
}


} // namespace escript


bool escript::Console::call(const std::u32string &method, Processor *p)
{
    if (BaseClass::call(method, p))
        return true;
    if (method == U"log") {
        auto nArgs = p->popFromStack().value;
        std::vector<std::u32string> args;
        while (nArgs > 0) {
            auto arg = p->popFromStack();
            args.push_back(arg.uString());
            nArgs--;
        }
        log(args);
        p->pushToStack(0); // OK
        return true;
    }
    throw std::domain_error("Call of unknown method: console." + to_utf8(method));
}
