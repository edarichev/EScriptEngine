#include "stdafx.h"
#include "arrayobject.h"
#include "pvalue.h"
#include "processor.h"

namespace escript {

std::map<std::u32string, Array::pFn> Array::_fn;

Array::Array()
{
    _managed = true;
    buildFunctionsMap();
}

Array::~Array()
{

}

void Array::buildFunctionsMap()
{
    if (!_fn.empty())
        return;
    _fn[U"get_length"] = &Array::call_get_length;
    _fn[U"get"] = &Array::call_get;
    _fn[U"set"] = &Array::call_set;
    _fn[U"push"] = &Array::call_push;
    _fn[U"add"] = &Array::call_push;
    _fn[U"pop"] = &Array::call_pop;
    _fn[U"shift"] = &Array::call_shift;
    _fn[U"splice"] = &Array::call_splice;
    _fn[U"slice"] = &Array::call_slice;
    _fn[U"reverse"] = &Array::call_reverse;
    _fn[U"fill"] = &Array::call_fill;
    _fn[U"sort"] = &Array::call_sort;
    _fn[U"unshift"] = &Array::call_unshift;
}

PValue Array::get(int64_t index)
{
    if (index >= (int64_t)_indexedItems.size()) {
        return PValue();
    } else if (index < 0) {
        return get(to_u32string(index));
    }
    return _indexedItems[index];
}

PValue Array::get(const std::u32string &index)
{
    auto it = _namedItems.find(index);
    if (it == _namedItems.end())
        return PValue();
    return it->second;
}

void Array::set(int64_t index, PValue value)
{
    if (index >= 0) {
        if (index >= (int64_t) _indexedItems.size())
            _indexedItems.resize(index + 1);
        _indexedItems[index] = value;
        return;
    }
    auto key = to_u32string(index);
    setKeyValue(key, value);
}

void Array::set(int64_t index, SymbolType t, uint64_t value)
{
    set(index, PValue(t, value));
}

void Array::set(const std::u32string &index, PValue value)
{
    setKeyValue(index, value);
}

int64_t Array::length() const { return _indexedItems.size(); }

void Array::add(PValue value)
{
    set(_indexedItems.size(), value);
}

std::u32string Array::uString() const
{
    // вывести все элементы
    std::u32string s(U"[");
    int64_t n = _indexedItems.size();
    int64_t i = 0;
    for (; i < n; i++) {
        s += _indexedItems[i].uString();
        if (i < n - 1)
            s.append(U", ");
    }
    if (_indexedItems.size() && !_namedItems.empty())
        s.append(U", ");
    n = _namedItems.size();
    i = 0;
    for (auto &p : _namedItems) {
        s.append(enquote(p.first));
        if (i < n - 1)
            s.append(U", ");
        i++;
    }
    s += U"]";
    return s;
}

void Array::setKeyValue(const std::u32string &key, const PValue &value)
{
    _namedItems[key] = value;
}

void Array::call_get_length(Processor *p)
{
    // вытащить из стека аргументы
    // число аргументов, здесь д.б. 0, просто пропустить
    auto args = loadArguments(p);
    int64_t p1 = length();
    p->pushToStack(p1); // результат
}

void Array::call_push(Processor *p)
{
    auto args = loadArguments(p);
    while (!args.empty()) {
        auto arg = args.top();
        // пусть только переменные типа Integer
        if (arg.type == SymbolType::Variable) {
            ObjectRecord *rec = (ObjectRecord*)(arg.value);
            add(PValue(rec->type, rec->data));
        } else {
            add(PValue(arg));
        }
        args.pop();
    }
    p->pushToStack((int64_t)0); // OK
}

void Array::call_get(Processor *p)
{
    auto args = loadArguments(p);
    if (args.empty()) {
        p->pushToStack((int64_t)0); // OK
        return;
    }
    auto index = args.top();
    PValue v;
    if (index.ofType(SymbolType::Integer))
        v = get((int64_t)index.getIntValue());
    else
        v = get(index.getStringValue());
    p->pushToStack(v.type, v.value64()); // OK
}

void Array::call_set(Processor *p)
{
    auto args = loadArguments(p);
    auto argIndex = args.top(); // индекс/ключ
    args.pop();
    auto argValue = args.top(); // значение
    // индекс может не существовать, как ключ он будет добавлен
    // если пришла переменная, вынуть из неё значение и поместить его
    // так чтобы тип Variable В массиве не хранился
    // но это могут быть массивы, объекты, функции и т.п.
    PValue v = PValue::getValue(argValue);
    if (argIndex.ofType(SymbolType::Integer))
        set((int64_t)argIndex.getIntValue(), v);
    else
        set(argIndex.getStringValue(), v);
    p->pushToStack((int64_t)0); // OK
}

void Array::call_pop(Processor *p)
{
    auto args = loadArguments(p);
    if (_indexedItems.size()) {
        PValue v = _indexedItems.back();
        _indexedItems.pop_back();
        p->pushToStack(v.type, v.value64());
    } else
        p->pushToStack((int64_t)0); // OK
}

void Array::call_shift(Processor *p)
{
    auto args = loadArguments(p);
    if (_indexedItems.size()) {
        PValue v = _indexedItems.front();
        _indexedItems.erase(_indexedItems.begin());
        p->pushToStack(v.type, v.value64());
    } else
        p->pushToStack((int64_t)0); // OK
}

void Array::call_splice(Processor *p)
{
    // splice() - вернёт пустой массив, это не изменится
    // splice(pos) - удаляет и возвращает все индексные элементы, начиная с заданной позиции
    // pos < 0 - отсчёт с конца
    // splice(pos, n) - удаляет и возвращает n индексных элементов, начиная с заданной позиции
    // splice(pos, n, x1, x2,....) - удаляет и возвращает n индексных элементов,
    // начиная с заданной позиции, и вставляет перечисленные элементы
    auto args = loadArguments(p);
    Array *arr = new Array();
    arr->addRef();
    if (args.empty()) {
        p->pushArrayToStack(arr);
        return;
    }
    int64_t startPos = args.top().getIntValue();
    if (startPos > (int64_t)_indexedItems.size())
        startPos = _indexedItems.size();
    if (startPos < 0)
        startPos = (int64_t)_indexedItems.size() + startPos;
    if (startPos < 0) // всё равно слишком далеко?
        startPos = 0;
    args.pop();
    int64_t nRemove = args.empty() ? _indexedItems.size() // удалить все
                                   : args.top().getIntValue();
    if (!args.empty())
        args.pop();
    if (nRemove < 0 || nRemove > (int64_t) _indexedItems.size())
        nRemove = _indexedItems.size();
    if (nRemove + startPos > (int64_t)_indexedItems.size())
        nRemove = (int64_t)_indexedItems.size() - startPos;
    auto itStart = _indexedItems.begin() + startPos;
    // сначала сохранить удаляемое
    if (nRemove > 0)
        arr->_indexedItems.insert(arr->_indexedItems.end(),
                                  std::make_move_iterator(itStart),
                                  std::make_move_iterator(itStart + nRemove));
    // теперь удалить
    _indexedItems.erase(itStart, _indexedItems.begin() + startPos + nRemove);
    if (args.empty()) {
        // добавлять нечего
        p->pushArrayToStack(arr);
        return;
    }
    // дальше - добавляемые
    size_t i = 0;
    while (!args.empty()) {
        PValue v = PValue(args.top());
        args.pop();
        _indexedItems.insert(_indexedItems.begin() + startPos + i, v);
        i++;
    }
    p->pushArrayToStack(arr);
}

void Array::call_reverse(Processor *p)
{
    auto args = loadArguments(p);
    std::reverse(_indexedItems.begin(), _indexedItems.end());
    p->pushArrayToStack(this);
}

void Array::call_fill(Processor *p)
{
    // если нет - как есть
    // если 1 аргумент - это значение, которым заполняем
    // арг 2 - стартовый индекс, если нет - с 0
    // арг 3 - конечный индекс, если его нет - до конца
    auto args = loadArguments(p);
    p->pushArrayToStack(this); // сразу занести, т.к. вернём этот же массив
    if (args.empty()) {
        return;
    }
    PValue v(args.top());
    args.pop();
    int64_t start = 0;
    if (!args.empty()) {
        start = args.top().getIntValue();
        args.pop();
    }
    if (start < 0)
        start += _indexedItems.size();
    if (start < 0)
        start = 0;
    int end = _indexedItems.size();
    if (!args.empty()) {
        end = args.top().getIntValue();
        args.pop();
        if (end < 0)
            end += _indexedItems.size();
        if (end < start)
            return;
    }
    if (start < end)
        std::fill(_indexedItems.begin() + start, _indexedItems.begin() + end, v);
}

void Array::call_sort(Processor *p)
{
    auto args = loadArguments(p);
    std::sort(_indexedItems.begin(), _indexedItems.end());
    p->pushArrayToStack(this);
}

void Array::call_unshift(Processor *p)
{
    auto args = loadArguments(p);
    int i = 0;
    while (!args.empty()) {
        PValue v(args.top());
        _indexedItems.insert(_indexedItems.begin() + i, v);
        args.pop();
        i++;
    }
    p->pushToStack(_indexedItems.size());
}

void Array::call_slice(Processor *p)
{
    auto args = loadArguments(p);
    Array *arr = new Array();
    arr->addRef();

    int64_t start = 0;
    int64_t end = _indexedItems.size();
    if (!args.empty())
    {
        start = args.top().getIntValue();
        args.pop();
        if (start < 0)
            start += _indexedItems.size();
        if (start < 0)
            start = 0;
        if (!args.empty()) {
            end = args.top().getIntValue();
            args.pop();
            if (end < 0)
                end += _indexedItems.size();
        }
    }
    if (start < end) {
        arr->_indexedItems.insert(arr->_indexedItems.begin(),
                                  _indexedItems.begin() + start,
                                  _indexedItems.begin() + end);
    }
    p->pushArrayToStack(arr);
}

std::u32string Array::enquote(const std::u32string &key)
{
    bool b = false;
    bool squote = false;
    bool dquote = false;
    bool bquote = false;
    for (auto &c : key) {
        if (!isalnum(c)) {
            b = true;
        }
        switch (c) {
        case '\'': squote = true; break;
        case '\"': dquote = true; break;
        case '`': bquote = true; break;
        }
    }
    // TODO: потом выбрать кавычки
    b = b || squote || dquote || bquote;
    if (b)
        return U"\'" + key + U"\'";
    return key;
}

bool Array::call(const std::u32string &method, Processor *p)
{
    if (BaseClass::call(method, p))
        return true;
    auto ptrToMethod = _fn.find(method);
    if (ptrToMethod == _fn.end())
        throw std::domain_error("Call of unknown method: MyClass." + to_utf8(method));
    (this->*ptrToMethod->second)(p);
    return true;
}

} // namespace escript
