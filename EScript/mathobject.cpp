#include "stdafx.h"
#include "mathobject.h"

namespace escript {

std::map<std::u32string, double> Math::_constants;

Math::Math()
{

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


} // namespace escript
