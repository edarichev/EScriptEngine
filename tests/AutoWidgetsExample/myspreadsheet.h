#ifndef MYSPREADSHEET_H
#define MYSPREADSHEET_H

#include "escriptengine.h"
#include "mycell.h"

using namespace escript;

/**
 * @brief Поскольку будет сложно использовать QTableWidget в скрипте,
 * мы делаем адаптер.
 */
class MySpreadSheet : public AutomationObject
{
    using BaseClass = AutomationObject;
    using pFn = void (MySpreadSheet::*)(Processor *p);
    QTableWidget *_table = nullptr;
    int _id = 0;
    static std::map<std::u32string, pFn> _fn;
    std::vector<std::vector<std::shared_ptr<MyCell>>> _cells;
public:
    MySpreadSheet(QTableWidget *table);
    // AutomationObject interface
public:
    virtual bool call(const std::u32string &method, Processor *p) override;
    void setCellValue(int row, int col, const std::u32string &str);
    void setCellValue(int row, int col, const StackValue &value);
    const std::u32string getCellValue(int row, int col);
    std::shared_ptr<MyCell> getCell(int row, int column);

private:
    void buildFunctionsMap();
    void checkIndexes(int rowIndex, int columnIndex) const;
    void call_getCellValue(Processor *p);
    void call_setCellValue(Processor *p);
    void call_cell(Processor *p);
};
#endif // MYSPREADSHEET_H
