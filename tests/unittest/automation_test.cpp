#include "stdafx.h"
#include "automation_test.h"

class MySpreadSheet : public AutomationObject
{
    using BaseClass = AutomationObject;
    std::vector<std::vector<std::u32string> > _cells;
    static constexpr const int ROWS = 10;
    static constexpr const int COLUMNS = 5;
    int _id = 0;
public:
    MySpreadSheet() : _cells(ROWS)
    {
        for (size_t row = 0; row < _cells.size(); row++) {
            _cells[row].resize(COLUMNS);
        }
    }
    // AutomationObject interface
public:
    virtual bool call(const u32string &method, Processor *p) override;
    void setCellValue(int row, int col, std::u32string str)
    {
        checkIndexes(row, col);
        _cells[row][col] = std::move(str);
    }
    void setCellValue(int row, int col, const StackValue &value)
    {
        setCellValue(row, col, value.uString());
    }
    const std::u32string getCellValue(int row, int col) const
    {
        checkIndexes(row, col);
        return _cells[row][col];
    }
    int id() const { return _id;}
    void setId(int newId) { _id = newId; }

private:
    void checkIndexes(int rowIndex, int columnIndex) const
    {
        if (rowIndex < 0 || rowIndex >= ROWS)
            throw std::out_of_range("row index is out of range");
        if (columnIndex < 0 || columnIndex >= COLUMNS)
            throw std::out_of_range("column index is out of range");
    }
};

bool MySpreadSheet::call(const u32string &method, Processor *p)
{
    if (BaseClass::call(method, p))
        return true;
    if (method == U"set_id") {
        auto argCount = p->popFromStack().value; // число аргументов == 1
        assert(argCount == 1);
        auto argId = p->popFromStack(); // новое значение
        int newId = argId.getIntValue();
        setId(newId);
        p->pushToStack((int64_t)0);
        return true;
    }
    if (method == U"get_id") {
        auto argCount = p->popFromStack().value; // число аргументов == 0
        assert(argCount == 0);
        p->pushToStack(id());
        return true;
    }
    if (method == U"getCellValue") {
        auto argCount = p->popFromStack().value; // число аргументов == 2
        assert(argCount == 2);
        auto argColumnIndex = p->popFromStack(); // индекс столбца
        auto argRowIndex = p->popFromStack(); // индекс строки
        int columnIndex = argColumnIndex.getIntValue();
        int rowIndex = argRowIndex.getIntValue();
        std::u32string value = getCellValue(rowIndex, columnIndex);
        StringObject *newString = new StringObject(value);
        p->pushStringToStack(newString);
        return true;
    }
    if (method == U"setCellValue") {
        auto argCount = p->popFromStack().value; // число аргументов == 3
        assert(argCount == 3);
        auto argValue = p->popFromStack(); // сначала значение
        auto argColumnIndex = p->popFromStack(); // индекс столбца
        auto argRowIndex = p->popFromStack(); // индекс строки
        int columnIndex = argColumnIndex.getIntValue();
        int rowIndex = argRowIndex.getIntValue();
        if (argValue.type == SymbolType::Variable) {
            ObjectRecord *rec = (ObjectRecord*)argValue.value;
            switch (rec->type) {
            case SymbolType::Boolean:
            case SymbolType::Integer:
            case SymbolType::Real:
            case SymbolType::String:
                argValue.type = rec->type;
                argValue.value = rec->data;
                break;
            default:
                throw std::domain_error("Not supported type");
            }
        }
        setCellValue(rowIndex, columnIndex, argValue);
        p->pushToStack(0); // OK, всегда что-то вернуть. Если не результат, то 0.
        return true;
    }
    throw std::domain_error("Call of unknown method: MySpreadSheet." + to_utf8(method));
}

////////////////////////// Test ////////////////////////////////////////////////

void Automation_Test::run()
{
    initTestCase();
    test_auto1();
    test_autoPropSet();
    test_autoAddAfterFirstRun();
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


