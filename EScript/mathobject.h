#ifndef ESCRIPT_MATH_H
#define ESCRIPT_MATH_H

#include "automationobject.h"
#include <cmath>
#include <map>

namespace escript {

class Math : public AutomationObject
{
private:
    static std::map<std::u32string, double> _constants;
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
};

} // namespace escript

#endif // ESCRIPT_MATH_H
