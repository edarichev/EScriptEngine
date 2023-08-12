#include "stdafx.h"
#include "array.h"
#include "pvalue.h"

namespace escript {

Array::Array()
{

}

Array::~Array()
{

}

PValue Array::get(int64_t index)
{
    return _items[to_utf8(index)];
}

PValue Array::get(const std::u32string &index)
{
    return _items[to_utf8(index)];
}

void Array::set(int64_t index, PValue value)
{
    _items[to_utf8(index)] = value;
}

void Array::set(const std::u32string &index, PValue value)
{
    _items[to_utf8(index)] = value;
}

} // namespace escript
