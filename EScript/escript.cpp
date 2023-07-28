/**
 * @file escript.cpp
 * @brief Реализация класса скриптового движка.
 */
#include "stdafx.h"
#include "escript.h"
#include "parser.h"

namespace escript {

EScript::EScript()
{
}

EScript::~EScript()
{

}

void EScript::eval(const std::u32string &strCode)
{
    Parser parser;
    parser.parse(strCode);
}

} // namespace escript
