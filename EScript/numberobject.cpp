#include "stdafx.h"
#include "numberobject.h"
#include "processor.h"

namespace escript {

std::map<std::u32string, Number::pFn> Number::_fn;

Number::Number()
{
    _managed = true;
    buildFunctionsMap();
}

Number::~Number()
{

}

bool Number::call(const std::u32string &method, Processor *p)
{
    if (method != U"toString") { // другая реализация
        if (BaseClass::call(method, p))
            return true;
    }
    auto ptrToMethod = _fn.find(method);
    if (ptrToMethod == _fn.end())
        throw std::domain_error("Call of unknown method: MyClass." + to_utf8(method));
    (this->*ptrToMethod->second)(p);
    return true;
}

void Number::buildFunctionsMap()
{
    if (!_fn.empty())
        return;
    _fn[U"parseInt"] = &Number::call_parseInt;
    _fn[U"parseFloat"] = &Number::call_parseFloat;
    _fn[U"toString"] = &Number::call_toString;
}

void Number::call_parseInt(Processor *p)
{
    auto args = loadArguments(p);
    if (args.empty())
        p->pushToStack(std::numeric_limits<int64_t>::signaling_NaN());
    else
        p->pushToStack(args.top().getIntValue());
}

void Number::call_parseFloat(Processor *p)
{
    auto args = loadArguments(p);
    if (args.empty())
        p->pushRealToStack(std::numeric_limits<double>::signaling_NaN());
    else
        p->pushRealToStack(args.top().getRealValue());
}

void Number::call_toString(Processor *p)
{
    auto args = loadArguments(p);
    switch (_type) {
    case SymbolType::Integer: {
        int radix = args.empty() ? 10 : args.top().getIntValue();
        p->pushToStack(toString(intValue, radix));
        break;
    }
    case SymbolType::Real: {
        int radix = args.empty() ? 10 : args.top().getIntValue();
        p->pushToStack(toString(realValue, radix));
        break;
    }
    default:
        throw std::domain_error("The type of Number must be Integer or Real");
    }
}

std::u32string Number::toString(int64_t value, int radix)
{
    if (radix < 2 || radix > 36)
        throw std::domain_error("Radix must be between 2 and 36");
    char digit[] = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ";
    char stack[36];
    std::u32string out;
    int digits = 0;
    do
    {
        int quot = value / radix;
        int rem = value % radix;
        stack[digits] = digit[rem];
        value = quot;
        digits++;
    } while( value );

    while(digits--) {
        out.push_back(stack[digits]);
    }
    return out;
}

std::u32string Number::toString(double value, [[maybe_unused]]int radix)
{
    std::stringstream ss;
    ss << std::noshowpoint << value;
    auto s = ss.str();
    return std::u32string(s.begin(), s.end());
}

} // namespace escript
