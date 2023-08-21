#include "stdafx.h"
#include "mathobject.h"
#include "processor.h"
#include <random>

namespace escript {

std::map<std::u32string, double> Math::_constants;
std::map<std::u32string, Math::pFn> Math::_fn;

Math::Math()
{
    buildFunctionsMap();
}

Math::~Math()
{

}

bool Math::getConstant(const std::u32string &name, double &value)
{
    buildConstantsMap();
    auto it = _constants.find(name);
    if (it == _constants.end())
        return false;
    value = it->second;
    return true;
}

void Math::buildConstantsMap()
{
    if (!_constants.empty())
        return;
    _constants[U"E"] = E;
    _constants[U"LN10"] = LN10;
    _constants[U"LN2"] = LN2;
    _constants[U"LOG10E"] = LOG10E;
    _constants[U"LOG2E"] = LOG2E;
    _constants[U"PI"] = PI;
    _constants[U"SQRT1_2"] = SQRT1_2;
    _constants[U"SQRT2"] = SQRT2;
}

void Math::buildFunctionsMap()
{
    if (!_fn.empty())
        return;
    _fn[U"abs"] = &Math::call_abs;
    _fn[U"acos"] = &Math::call_acos;
    _fn[U"acosh"] = &Math::call_acosh;
    _fn[U"asin"] = &Math::call_asin;
    _fn[U"asinh"] = &Math::call_asinh;
    _fn[U"atan"] = &Math::call_atan;
    _fn[U"atan2"] = &Math::call_atan2;
    _fn[U"atanh"] = &Math::call_atanh;
    _fn[U"cbrt"] = &Math::call_cbrt;
    _fn[U"ceil"] = &Math::call_ceil;
    _fn[U"clz32"] = &Math::call_clz32;
    _fn[U"cos"] = &Math::call_cos;
    _fn[U"cosh"] = &Math::call_cosh;
    _fn[U"exp"] = &Math::call_exp;
    _fn[U"expm1"] = &Math::call_expm1;
    _fn[U"floor"] = &Math::call_floor;
    _fn[U"fround"] = &Math::call_fround;
    _fn[U"hypot"] = &Math::call_hypot;
    _fn[U"imul"] = &Math::call_imul;
    _fn[U"log"] = &Math::call_log;
    _fn[U"log10"] = &Math::call_log10;
    _fn[U"log1p"] = &Math::call_log1p;
    _fn[U"log2"] = &Math::call_log2;
    _fn[U"max"] = &Math::call_max;
    _fn[U"min"] = &Math::call_min;
    _fn[U"pow"] = &Math::call_pow;
    _fn[U"random"] = &Math::call_random;
    _fn[U"round"] = &Math::call_round;
    _fn[U"sign"] = &Math::call_sign;
    _fn[U"sin"] = &Math::call_sin;
    _fn[U"sinh"] = &Math::call_sinh;
    _fn[U"sqrt"] = &Math::call_sqrt;
    _fn[U"tan"] = &Math::call_tan;
    _fn[U"tanh"] = &Math::call_tanh;
    _fn[U"trunc"] = &Math::call_trunc;
}

bool Math::call(const std::u32string &method, Processor *p)
{
    if (BaseClass::call(method, p))
        return true;
    auto ptrToMethod = _fn.find(method);
    if (ptrToMethod == _fn.end())
        throw std::domain_error("Call of unknown method: Math." + to_utf8(method));
    (this->*ptrToMethod->second)(p);
    return true;
}

void Math::call_abs(Processor *p)
{
    auto args = loadArguments(p);
    if (args.empty())
        p->pushToStack(0);
    else {
        if (args.top().type == SymbolType::Integer)
            p->pushToStack(llabs(args.top().getIntValue()));
        else {
            double v = args.top().getRealValue();
            p->pushRealToStack(fabs(v));
        }
    }
}

void Math::call_acos(Processor *p)
{
    auto args = loadArguments(p);
    if (args.empty())
        p->pushRealToStack(std::numeric_limits<double>::signaling_NaN());
    else
        p->pushRealToStack(acos(args.top().getRealValue()));
}

void Math::call_acosh(Processor *p)
{
    auto args = loadArguments(p);
    if (args.empty())
        p->pushRealToStack(std::numeric_limits<double>::signaling_NaN());
    else
        p->pushRealToStack(acosh(args.top().getRealValue()));
}

void Math::call_asin(Processor *p)
{
    auto args = loadArguments(p);
    if (args.empty())
        p->pushRealToStack(std::numeric_limits<double>::signaling_NaN());
    else
        p->pushRealToStack(asin(args.top().getRealValue()));
}

void Math::call_asinh(Processor *p)
{
    auto args = loadArguments(p);
    if (args.empty())
        p->pushRealToStack(std::numeric_limits<double>::signaling_NaN());
    else
        p->pushRealToStack(asinh(args.top().getRealValue()));
}

void Math::call_atan(Processor *p)
{
    auto args = loadArguments(p);
    if (args.empty())
        p->pushRealToStack(std::numeric_limits<double>::signaling_NaN());
    else
        p->pushRealToStack(atan(args.top().getRealValue()));
}

void Math::call_atan2(Processor *p)
{
    // Computes the arc tangent of y / x
    auto args = loadArguments(p);
    if (args.size() < 2)
        p->pushRealToStack(std::numeric_limits<double>::signaling_NaN());
    else {
        double y = args.top().getRealValue();
        args.pop();
        double x = args.top().getRealValue();
        p->pushRealToStack(atan2(y, x));
    }
}

void Math::call_atanh(Processor *p)
{
    auto args = loadArguments(p);
    if (args.empty())
        p->pushRealToStack(std::numeric_limits<double>::signaling_NaN());
    else
        p->pushRealToStack(atanh(args.top().getRealValue()));
}

void Math::call_cbrt(Processor *p)
{
    auto args = loadArguments(p);
    if (args.empty())
        p->pushRealToStack(std::numeric_limits<double>::signaling_NaN());
    else
        p->pushRealToStack(cbrt(args.top().getRealValue()));
}

void Math::call_ceil(Processor *p)
{
    auto args = loadArguments(p);
    if (args.empty())
        p->pushRealToStack(std::numeric_limits<double>::signaling_NaN());
    else
        p->pushRealToStack(ceil(args.top().getRealValue()));
}

void Math::call_clz32(Processor *p)
{ // число ведущих нулей d 32-разрядном числе
    auto args = loadArguments(p);
    if (args.empty())
        p->pushRealToStack(std::numeric_limits<double>::signaling_NaN());
    else {
        uint64_t v = args.top().getIntValue();
        uint32_t i32 = (uint32_t)(0xFFFFFFFF & v);
        int64_t i = 32;
        while (i32 != 0) {
            i32 >>= 1;
            i--;
        }
        p->pushToStack(i);
    }
}

void Math::call_cos(Processor *p)
{
    auto args = loadArguments(p);
    if (args.empty())
        p->pushRealToStack(std::numeric_limits<double>::signaling_NaN());
    else
        p->pushRealToStack(cos(args.top().getRealValue()));
}

void Math::call_cosh(Processor *p)
{
    auto args = loadArguments(p);
    if (args.empty())
        p->pushRealToStack(std::numeric_limits<double>::signaling_NaN());
    else
        p->pushRealToStack(cosh(args.top().getRealValue()));
}

void Math::call_exp(Processor *p)
{
    auto args = loadArguments(p);
    if (args.empty())
        p->pushRealToStack(std::numeric_limits<double>::signaling_NaN());
    else
        p->pushRealToStack(exp(args.top().getRealValue()));
}

void Math::call_expm1(Processor *p)
{
    // e^x-1 ~ 2.71^x-1
    auto args = loadArguments(p);
    if (args.empty())
        p->pushRealToStack(std::numeric_limits<double>::signaling_NaN());
    else
        p->pushRealToStack(expm1(args.top().getRealValue()));
}

void Math::call_floor(Processor *p)
{
    auto args = loadArguments(p);
    if (args.empty())
        p->pushRealToStack(std::numeric_limits<double>::signaling_NaN());
    else
        p->pushRealToStack(floor(args.top().getRealValue()));
}

void Math::call_fround(Processor *p)
{
    auto args = loadArguments(p);
    if (args.empty())
        p->pushRealToStack(std::numeric_limits<double>::signaling_NaN());
    else
        p->pushRealToStack((double)(round((float)args.top().getRealValue())));
}

void Math::call_hypot(Processor *p)
{
    auto args = loadArguments(p);
    if (args.empty())
        p->pushRealToStack(std::numeric_limits<double>::signaling_NaN());
    else {
        double a = args.top().getRealValue();
        args.pop();
        double b = args.empty() ? 0 : args.top().getRealValue();
        p->pushRealToStack(hypot(a, b));
    }
}

void Math::call_imul(Processor *p)
{
    auto args = loadArguments(p);
    if (args.empty())
        p->pushRealToStack(std::numeric_limits<double>::signaling_NaN());
    else {
        double a = args.top().getRealValue();
        args.pop();
        double b = args.empty() ? 0 : args.top().getRealValue();
        p->pushToStack((int64_t)a * (int64_t)b);
    }
}

void Math::call_log(Processor *p)
{ // ln (натуральный)
    auto args = loadArguments(p);
    if (args.empty())
        p->pushRealToStack(std::numeric_limits<double>::signaling_NaN());
    else
        p->pushRealToStack(log(args.top().getRealValue()));
}

void Math::call_log10(Processor *p)
{
    auto args = loadArguments(p);
    if (args.empty())
        p->pushRealToStack(std::numeric_limits<double>::signaling_NaN());
    else
        p->pushRealToStack(log10(args.top().getRealValue()));
}

void Math::call_log1p(Processor *p)
{ // ln (1 + x)
    auto args = loadArguments(p);
    if (args.empty())
        p->pushRealToStack(std::numeric_limits<double>::signaling_NaN());
    else
        p->pushRealToStack(log1p(args.top().getRealValue()));
}

void Math::call_log2(Processor *p)
{
    auto args = loadArguments(p);
    if (args.empty())
        p->pushRealToStack(std::numeric_limits<double>::signaling_NaN());
    else
        p->pushRealToStack(log2(args.top().getRealValue()));
}

void Math::call_max(Processor *p)
{
    auto args = loadArguments(p);
    if (args.empty())
        p->pushRealToStack(std::numeric_limits<double>::signaling_NaN());
    else {
        double m = args.top().getRealValue();
        args.pop();
        while (!args.empty()) {
            double n = args.top().getRealValue();
            args.pop();
            if (m < n)
                m = n;
        }
        p->pushRealToStack(m);
    }
}

void Math::call_min(Processor *p)
{
    auto args = loadArguments(p);
    if (args.empty())
        p->pushRealToStack(std::numeric_limits<double>::signaling_NaN());
    else {
        double m = args.top().getRealValue();
        args.pop();
        while (!args.empty()) {
            double n = args.top().getRealValue();
            args.pop();
            if (m > n)
                m = n;
        }
        p->pushRealToStack(m);
    }
}

void Math::call_pow(Processor *p)
{
    auto args = loadArguments(p);
    if (args.empty())
        p->pushRealToStack(std::numeric_limits<double>::signaling_NaN());
    else {
        double a = args.top().getRealValue();
        args.pop();
        double b = args.empty() ? 0 : args.top().getRealValue();
        p->pushRealToStack(pow(a, b));
    }
}

void Math::call_random(Processor *p)
{
    loadArguments(p); // ничего нет, просто всё очистить
    std::random_device r;
    std::default_random_engine dre(r());
    std::uniform_real_distribution<double> rd(0, 1);
    double value = rd(dre);
    p->pushRealToStack(value);
}

void Math::call_round(Processor *p)
{
    auto args = loadArguments(p);
    if (args.empty())
        p->pushRealToStack(std::numeric_limits<double>::signaling_NaN());
    else
        p->pushRealToStack(round(args.top().getRealValue()));
}

void Math::call_sign(Processor *p)
{
    auto args = loadArguments(p);
    if (args.empty())
        p->pushRealToStack(std::numeric_limits<double>::signaling_NaN());
    else {
        double v = args.top().getRealValue();
        int64_t sign = v == 0 ? 0 : (v < 0 ? -1 : 1);
        p->pushToStack(sign);
    }
}

void Math::call_sin(Processor *p)
{
    auto args = loadArguments(p);
    if (args.empty())
        p->pushRealToStack(std::numeric_limits<double>::signaling_NaN());
    else
        p->pushRealToStack(sin(args.top().getRealValue()));
}

void Math::call_sinh(Processor *p)
{
    auto args = loadArguments(p);
    if (args.empty())
        p->pushRealToStack(std::numeric_limits<double>::signaling_NaN());
    else
        p->pushRealToStack(sinh(args.top().getRealValue()));
}

void Math::call_sqrt(Processor *p)
{
    auto args = loadArguments(p);
    if (args.empty())
        p->pushRealToStack(std::numeric_limits<double>::signaling_NaN());
    else
        p->pushRealToStack(sqrt(args.top().getRealValue()));
}

void Math::call_tan(Processor *p)
{
    auto args = loadArguments(p);
    if (args.empty())
        p->pushRealToStack(std::numeric_limits<double>::signaling_NaN());
    else
        p->pushRealToStack(tan(args.top().getRealValue()));
}

void Math::call_tanh(Processor *p)
{
    auto args = loadArguments(p);
    if (args.empty())
        p->pushRealToStack(std::numeric_limits<double>::signaling_NaN());
    else
        p->pushRealToStack(tanh(args.top().getRealValue()));
}

void Math::call_trunc(Processor *p)
{
    auto args = loadArguments(p);
    if (args.empty())
        p->pushRealToStack(std::numeric_limits<double>::signaling_NaN());
    else
        p->pushRealToStack(trunc(args.top().getRealValue()));
}


} // namespace escript
