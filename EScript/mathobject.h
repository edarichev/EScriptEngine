#ifndef ESCRIPT_MATH_H
#define ESCRIPT_MATH_H

#include "automationobject.h"
#include <cmath>
#include <map>

namespace escript {

class Math : public AutomationObject
{
private:
    using BaseClass = AutomationObject;
    static std::map<std::u32string, double> _constants;
    typedef void (Math::*pFn)(Processor *p);
    static std::map<std::u32string, pFn> _fn;
public:
    static const constexpr double E = M_E;
    static const constexpr double LN10 = M_LN10;
    static const constexpr double LN2 = M_LN2;
    static const constexpr double LOG10E = M_LOG10E;
    static const constexpr double LOG2E = M_LOG2E;
    static const constexpr double PI = M_PI;
    static const constexpr double SQRT1_2 = M_SQRT1_2;
    static const constexpr double SQRT2 = M_SQRT2;
public:
    /**
     * @brief Создаёт новый экземпляр класса Math
     */
    Math();
    /**
     * @brief Освобождает связанные с этим экземпляром ресурсы
     */
    virtual ~Math();
    static bool getConstant(const std::u32string &name, /*out*/ double &value);
private:
    static void buildConstantsMap();
    static void buildFunctionsMap();
    void call_abs(Processor *p);
    void call_acos(Processor *p);
    void call_acosh(Processor *p);
    void call_asin(Processor *p);
    void call_asinh(Processor *p);
    void call_atan(Processor *p);
    void call_atan2(Processor *p);
    void call_atanh(Processor *p);
    void call_cbrt(Processor *p);
    void call_ceil(Processor *p);
    void call_clz32(Processor *p);
    void call_cos(Processor *p);
    void call_cosh(Processor *p);
    void call_exp(Processor *p);
    void call_expm1(Processor *p);
    void call_floor(Processor *p);
    void call_fround(Processor *p);
    void call_hypot(Processor *p);
    void call_imul(Processor *p);
    void call_log(Processor *p);
    void call_log10(Processor *p);
    void call_log1p(Processor *p);
    void call_log2(Processor *p);
    void call_max(Processor *p);
    void call_min(Processor *p);
    void call_pow(Processor *p);
    void call_random(Processor *p);
    void call_round(Processor *p);
    void call_sign(Processor *p);
    void call_sin(Processor *p);
    void call_sinh(Processor *p);
    void call_sqrt(Processor *p);
    void call_tan(Processor *p);
    void call_tanh(Processor *p);
    void call_trunc(Processor *p);

    // AutomationObject interface
public:
    virtual bool call(const std::u32string &method, Processor *p) override;
};

} // namespace escript

#endif // ESCRIPT_MATH_H
