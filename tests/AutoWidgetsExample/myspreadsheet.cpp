#include "stdafx.h"
#include "myspreadsheet.h"

std::map<std::u32string, MySpreadSheet::pFn> MySpreadSheet::_fn;

MySpreadSheet::MySpreadSheet(QTableWidget *table)
    : _table(table),
      _cells(table->rowCount())
{
    buildFunctionsMap();
    for (size_t row = 0; row < _cells.size(); row++) {
        _cells[row].resize(table->columnCount());
    }
}

bool MySpreadSheet::call(const std::u32string &method, Processor *p)
{
    if (BaseClass::call(method, p))
        return true;
    auto ptrToMethod = _fn.find(method);
    if (ptrToMethod == _fn.end())
        throw std::domain_error("Call of unknown method: MySpreadSheet." + to_utf8(method));
    (this->*ptrToMethod->second)(p);
    return true;
}

void MySpreadSheet::setCellValue(int row, int col, const std::u32string &str)
{
    checkIndexes(row, col);
    auto c = getCell(row, col);
    c->setValue(str);
}

void MySpreadSheet::setCellValue(int row, int col, const StackValue &value)
{
    setCellValue(row, col, value.getStringValue());
}

const std::u32string MySpreadSheet::getCellValue(int row, int col)
{
    checkIndexes(row, col);
    auto c = getCell(row, col);
    auto item = _table->item(row, col);
    if (!item) {
        item = new QTableWidgetItem (QString(""));
        _table->setItem(row, col, item);
    }

    return c->value();
}

std::shared_ptr<MyCell> MySpreadSheet::getCell(int row, int column)
{
    if (!_cells[row][column]) {
        _cells[row][column] = std::make_shared<MyCell>(this->_table, row, column);
    }
    return _cells[row][column];
}

void MySpreadSheet::buildFunctionsMap()
{
    if (!_fn.empty())
        return;
    _fn[U"getCellValue"] = &MySpreadSheet::call_getCellValue;
    _fn[U"setCellValue"] = &MySpreadSheet::call_setCellValue;
    _fn[U"cell"] = &MySpreadSheet::call_cell;
}

void MySpreadSheet::checkIndexes(int rowIndex, int columnIndex) const
{
    if (rowIndex < 0 || rowIndex >= _table->rowCount())
        throw std::out_of_range("row index is out of range");
    if (columnIndex < 0 || columnIndex >= _table->columnCount())
        throw std::out_of_range("column index is out of range");
}

void MySpreadSheet::call_getCellValue(Processor *p)
{
    auto args = loadArguments(p);
    assert(args.size() == 2);
    int rowIndex = args.top().getIntValue(); // индекс строки
    args.pop();
    int columnIndex = args.top().getIntValue(); // индекс столбца
    std::u32string value = getCellValue(rowIndex, columnIndex);
    StringObject *newString = new StringObject(value);
    p->pushStringToStack(newString);
}

void MySpreadSheet::call_setCellValue(Processor *p)
{
    auto args = loadArguments(p);
    assert(args.size() == 3); // число аргументов == 3
    int rowIndex = args.top().getIntValue();
    args.pop();
    int columnIndex = args.top().getIntValue();
    args.pop();
    auto argValue = args.top(); // значение
    setCellValue(rowIndex, columnIndex, argValue);
    p->pushToStack(0); // OK, всегда что-то вернуть. Если не результат, то 0.
}

void MySpreadSheet::call_cell(Processor *p)
{ // вернуть указатель на ячейку
    auto args = loadArguments(p);
    assert(args.size() == 2);
    int rowIndex = args.top().getIntValue(); // индекс строки
    args.pop();
    int columnIndex = args.top().getIntValue(); // индекс столбца
    MyCell *c = getCell(rowIndex, columnIndex).get();
    p->pushObjectToStack(c);
}
