#include "stdafx.h"
#include "mycell.h"

std::map<std::u32string, MyCell::pFn> MyCell::_fn;

QTableWidget *MyCell::table() const
{
    return _table;
}

int MyCell::row() const
{
    return _row;
}

int MyCell::column() const
{
    return _column;
}

MyCell::MyCell(QTableWidget *table, int row, int column)
    : _table(table), _row(row), _column(column)
{
    buildFunctionsMap();
}

bool MyCell::call(const std::u32string &method, Processor *p)
{
    if (BaseClass::call(method, p))
        return true;
    auto ptrToMethod = _fn.find(method);
    if (ptrToMethod == _fn.end())
        throw std::domain_error("Call of unknown method: MyCell." + to_utf8(method));
    (this->*ptrToMethod->second)(p);
    return true;
}

void MyCell::call_get_text(Processor *p)
{
    auto args = loadArguments(p);
    assert(args.empty());
    p->pushToStack(value());
}

void MyCell::call_set_text(Processor *p)
{
    auto args = loadArguments(p);
    assert(!args.empty());
    auto txt = args.top().getStringValue();
    setValue(txt);
    p->pushToStack(0); // OK
}

void MyCell::call_get_style(Processor *p)
{
    auto args = loadArguments(p);
    assert(args.empty());
    if (!_style)
        _style = std::make_shared<MyCellStyle>(shared_from_this());
    p->pushToStack(SymbolType::Object, (uint64_t)_style.get());
}

void MyCell::buildFunctionsMap()
{
    if (!_fn.empty())
        return;
    _fn[U"set_text"] = &MyCell::call_set_text;
    _fn[U"get_text"] = &MyCell::call_get_text;
    _fn[U"get_style"] = &MyCell::call_get_style;
}

const std::u32string MyCell::value()
{
    auto item = _table->item(_row, _column);
    if (!item) {
        setValue(U"");
        item = _table->item(_row, _column);
    }
    QString s = item->data(Qt::DisplayRole).toString();
    return s.toStdU32String();
}

void MyCell::setValue(const std::u32string &newValue)
{
    auto item = _table->item(_row, _column);
    if (!item) {
        item = new QTableWidgetItem (QString::fromStdU32String(newValue));
        _table->setItem(_row, _column, item);
    } else {
        item->setText(QString::fromStdU32String(newValue));
    }
}

