#include "stdafx.h"
#include "mycellcolor.h"
#include "mycell.h"

using namespace std;

std::map<std::u32string, MyCellColor::pFn> MyCellColor::_fn;

MyCellColor::MyCellColor(std::shared_ptr<MyCell> cell)
    : _cell(cell)
{
    buildFunctionsMap();
}

bool MyCellColor::call(const std::u32string &method, Processor *p)
{
    if (method != U"toString" && BaseClass::call(method, p))
        return true;
    auto ptrToMethod = _fn.find(method);
    if (ptrToMethod == _fn.end())
        throw std::domain_error("Call of unknown method: MyColor." + to_utf8(method));
    (this->*ptrToMethod->second)(p);
    return true;
}

void MyCellColor::buildFunctionsMap()
{
    if (!_fn.empty())
        return;
    _fn[U"toString"] = &MyCellColor::call_toString;
}

void MyCellColor::call_toString(Processor *p)
{
    auto args = loadArguments(p);
    assert(args.empty());
    stringstream ss;
    auto item = _cell->table()->item(_cell->row(), _cell->column());
    QColor c = item->backgroundColor();
    ss << "#" << setfill('0') << setw(2) << hex << uppercase << (0xFFFFFF & c.rgba());
    p->pushToStack(to_u32string(ss.str()));
}

void MyCellColor::set(const std::u32string &c)
{
    if (c.find(U"#") == 0) {
        size_t i = 0;
        int rgb = std::stoi(to_utf8(c.substr(1)), &i, 16);
        auto item = _cell->table()->item(_cell->row(), _cell->column());
        if (!item) {
            item = new QTableWidgetItem (QString());
            _cell->table()->setItem(_cell->row(), _cell->column(), item);
        }
        item->setBackgroundColor(QColor::fromRgb(QRgb(rgb)));
    }
}
