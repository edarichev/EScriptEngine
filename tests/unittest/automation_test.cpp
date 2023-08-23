#include "stdafx.h"
#include "automation_test.h"

class MyCell : public AutomationObject
{
    using BaseClass = AutomationObject;
    using pFn = void (MyCell::*)(Processor *p);
    std::u32string _value;
    static std::map<std::u32string, pFn> _fn;
public:
    const std::u32string &value() const { return _value; }
    void setValue(const u32string &newValue) { _value = newValue; }
    MyCell() { buildFunctionsMap(); }
    MyCell(const MyCell &) = default;
    MyCell(const std::u32string &s) : _value(s) { }
    MyCell &operator=(const std::u32string &s)
    {
        _value = s;
        return *this;
    }

    // AutomationObject interface
public:
    virtual bool call(const u32string &method, Processor *p) override
    {
        if (BaseClass::call(method, p))
            return true;
        auto ptrToMethod = _fn.find(method);
        if (ptrToMethod == _fn.end())
            throw std::domain_error("Call of unknown method: MyCell." + to_utf8(method));
        (this->*ptrToMethod->second)(p);
        return true;
    }
private:
    void call_get_text(Processor *p)
    {
        auto args = loadArguments(p);
        assert(args.empty());
        p->pushToStack(value());
    }

    void call_set_text(Processor *p)
    {
        auto args = loadArguments(p);
        assert(!args.empty());
        auto txt = args.top().getStringValue();
        setValue(txt);
        p->pushToStack(0); // OK
    }

    void buildFunctionsMap()
    {
        if (!_fn.empty())
            return;
        _fn[U"set_text"] = &MyCell::call_set_text;
        _fn[U"get_text"] = &MyCell::call_get_text;
    }
};

std::map<std::u32string, MyCell::pFn> MyCell::_fn;

/////////////////////// MySpreadSheet ///////////////////////////////////////

class MySpreadSheet : public AutomationObject
{
    using BaseClass = AutomationObject;
    using pFn = void (MySpreadSheet::*)(Processor *p);

    std::vector<std::vector<MyCell> > _cells;
    static constexpr const int ROWS = 10;
    static constexpr const int COLUMNS = 5;

    int _id = 0;
    static std::map<std::u32string, pFn> _fn;
public:
    MySpreadSheet() : _cells(ROWS)
    {
        buildFunctionsMap();
        for (size_t row = 0; row < _cells.size(); row++) {
            _cells[row].resize(COLUMNS);
        }
    }
    // AutomationObject interface
public:
    virtual bool call(const u32string &method, Processor *p) override
    {
        if (BaseClass::call(method, p))
            return true;
        auto ptrToMethod = _fn.find(method);
        if (ptrToMethod == _fn.end())
            throw std::domain_error("Call of unknown method: MySpreadSheet." + to_utf8(method));
        (this->*ptrToMethod->second)(p);
        return true;
    }
    void setCellValue(int row, int col, std::u32string str)
    {
        checkIndexes(row, col);
        _cells[row][col] = std::move(str);
    }
    void setCellValue(int row, int col, const StackValue &value)
    {
        setCellValue(row, col, value.getStringValue());
    }
    const std::u32string getCellValue(int row, int col) const
    {
        checkIndexes(row, col);
        return _cells[row][col].value();
    }
    int id() const { return _id;}
    void setId(int newId) { _id = newId; }

private:
    void buildFunctionsMap()
    {
        if (!_fn.empty())
            return;
        _fn[U"set_id"] = &MySpreadSheet::call_set_id;
        _fn[U"get_id"] = &MySpreadSheet::call_get_id;
        _fn[U"acceptArray"] = &MySpreadSheet::call_acceptArray;
        _fn[U"getCellValue"] = &MySpreadSheet::call_getCellValue;
        _fn[U"setCellValue"] = &MySpreadSheet::call_setCellValue;
        _fn[U"cell"] = &MySpreadSheet::call_cell;
    }
    void checkIndexes(int rowIndex, int columnIndex) const
    {
        if (rowIndex < 0 || rowIndex >= ROWS)
            throw std::out_of_range("row index is out of range");
        if (columnIndex < 0 || columnIndex >= COLUMNS)
            throw std::out_of_range("column index is out of range");
    }
    void call_set_id(Processor *p)
    {
        auto args = loadArguments(p);
        assert(!args.empty());
        int newId = args.top().getIntValue();
        setId(newId);
        p->pushToStack((int64_t)0);
    }
    void call_get_id(Processor *p)
    {
        auto args = loadArguments(p);
        assert(args.empty());
        p->pushToStack(id());
    }
    void call_acceptArray(Processor *p)
    {
        auto args = loadArguments(p);
        assert(args.size());
        Array *a = args.top().getArrayValue();
        p->pushToStack(a->length()); // вернуть длину
    }
    void call_getCellValue(Processor *p)
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
    void call_setCellValue(Processor *p)
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
    void call_cell(Processor *p)
    { // вернуть указатель на ячейку
        auto args = loadArguments(p);
        assert(args.size() == 2);
        int rowIndex = args.top().getIntValue(); // индекс строки
        args.pop();
        int columnIndex = args.top().getIntValue(); // индекс столбца
        MyCell *c = &_cells[rowIndex][columnIndex];
        p->pushObjectToStack(c);
    }
};

std::map<std::u32string, MySpreadSheet::pFn> MySpreadSheet::_fn;

////////////////////////// Test ////////////////////////////////////////////////

void Automation_Test::run()
{
    initTestCase();
    test_auto1();
    test_autoPropSet();
    test_autoAddAfterFirstRun();
    test_acceptArray();
    test_acceptArrayVariable();
    test_nestedProperty();
    cleanupTestCase();
}

void Automation_Test::initTestCase()
{

}

void Automation_Test::cleanupTestCase()
{

}

void Automation_Test::test_auto1()
{
    const std::string macro1 = R"(
    function updateCell(row, col, v) {
        spreadsheet.setCellValue(row, col, v);
    }

    updateCell(1, 3, "Hello, world");
    updateCell(1, 2, 12345);
    s = spreadsheet.getCellValue(1, 3);
    s += "!!!!";
    updateCell(0, 0, s);

)";
    const std::u32string code1 = to_u32string(macro1);
    MySpreadSheet spreadsheet;
    EScript engine;
    engine.attachObject(&spreadsheet, U"spreadsheet");
    engine.eval(code1);
    engine.detachObject(&spreadsheet);

    assert(spreadsheet.getCellValue(1, 3) == U"Hello, world");
    assert(spreadsheet.getCellValue(1, 2) == U"12345");
    assert(spreadsheet.getCellValue(0, 0) == U"Hello, world!!!!");
}

void Automation_Test::test_autoPropSet()
{
    const std::string macro1 = R"(
    spreadsheet.id = 123;
    x = spreadsheet.id;
)";
    const std::u32string code1 = to_u32string(macro1);
    MySpreadSheet spreadsheet;
    EScript engine;
    engine.attachObject(&spreadsheet, U"spreadsheet");
    engine.eval(code1);
    engine.detachObject(&spreadsheet);
    auto mainTable = engine.unit()->block()->symbolTable();
    auto x = mainTable->find(U"x");
    auto record = engine.getObjectRecord(x);
    assert(record->type == SymbolType::Integer);
    assert(Compare::equals_int64(123, record->data));
}

void Automation_Test::test_autoAddAfterFirstRun()
{
    // тест добавления объекта не в первом заходе
    EScript engine;
    engine.eval(U"i = 123;");

    const std::string macro1 = R"(
    spreadsheet.id = 123;
    x = spreadsheet.id;
)";
    const std::u32string code1 = to_u32string(macro1);
    MySpreadSheet spreadsheet;
    engine.attachObject(&spreadsheet, U"spreadsheet");
    engine.eval(code1);
    engine.detachObject(&spreadsheet);
    auto mainTable = engine.unit()->block()->symbolTable();
    auto x = mainTable->find(U"x");
    auto record = engine.getObjectRecord(x);
    assert(record->type == SymbolType::Integer);
    assert(Compare::equals_int64(123, record->data));
}

void Automation_Test::test_acceptArray()
{
    const std::u32string code1 = UR"(
x = spreadsheet.acceptArray([1,2,3,7]);
)";
    EScript engine;
    MySpreadSheet spreadsheet;
    engine.attachObject(&spreadsheet, U"spreadsheet");
    engine.eval(code1);
    engine.detachObject(&spreadsheet);
    auto mainTable = engine.unit()->block()->symbolTable();
    auto x = mainTable->find(U"x");
    auto record = engine.getObjectRecord(x);
    assert(record->type == SymbolType::Integer);
    assert(Compare::equals_int64(4, record->data));//length==4 штуки
}

void Automation_Test::test_acceptArrayVariable()
{
    const std::u32string code1 = UR"(
a = [1,2,3,7,11,11122];
x = spreadsheet.acceptArray(a);
)";
    EScript engine;
    MySpreadSheet spreadsheet;
    engine.attachObject(&spreadsheet, U"spreadsheet");
    engine.eval(code1);
    engine.detachObject(&spreadsheet);
    auto mainTable = engine.unit()->block()->symbolTable();
    auto x = mainTable->find(U"x");
    auto record = engine.getObjectRecord(x);
    assert(record->type == SymbolType::Integer);
    assert(Compare::equals_int64(6, record->data));//length==6
}

void Automation_Test::test_nestedProperty()
{
    const std::u32string code1 = UR"(
c1 = spreadsheet.cell(1, 1);
c1.text = "Hello";
s = c1.text;
s = spreadsheet.cell(1, 1).text;
spreadsheet.cell(1, 2).text = "World";
spreadsheet.cell(1, 2).text = spreadsheet.cell(1, 2).text + "!!!";
spreadsheet.cell(1, 3).text = "Hello";
console.log(s);
p = spreadsheet.cell(1, 3).text[1] = "a";
console.log(p);
console.log(spreadsheet.cell(1, 3).text);
)";
    EScript engine;
    MySpreadSheet spreadsheet;
    engine.attachObject(&spreadsheet, U"spreadsheet");
    engine.eval(code1);
    engine.detachObject(&spreadsheet);
    auto mainTable = engine.unit()->block()->symbolTable();
    auto c1 = mainTable->find(U"c1");
    auto record = engine.getObjectRecord(c1);
    assert(record->type == SymbolType::Object);

    auto s = mainTable->find(U"s");
    record = engine.getObjectRecord(s);
    assert(record->type == SymbolType::String);
    assert(Compare::equals_string(U"Hello", record->data));
    assert(spreadsheet.getCellValue(1, 1) == U"Hello");
    assert(spreadsheet.getCellValue(1, 2) == U"World!!!");
    assert(spreadsheet.getCellValue(1, 3) == U"Hello");

    auto p = mainTable->find(U"p");
    record = engine.getObjectRecord(p);
    assert(record->type == SymbolType::String);
    assert(Compare::equals_string(U"Hallo", record->data));
}



