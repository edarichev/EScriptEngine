#ifndef MYCELL_H
#define MYCELL_H

#include "mycellstyle.h"
using namespace escript;

class MyCell : public AutomationObject,
               public std::enable_shared_from_this<MyCell>
{
    using BaseClass = AutomationObject;
    using pFn = void (MyCell::*)(Processor *p);
    static std::map<std::u32string, pFn> _fn;
    std::shared_ptr<MyCellStyle> _style;
    QTableWidget *_table = nullptr;
    int _row = 0;
    int _column = 0;
public:
    MyCell(QTableWidget *table, int row, int column);
    const std::u32string value();
    void setValue(const std::u32string &newValue);

    // AutomationObject interface
public:
    virtual bool call(const std::u32string &method, Processor *p) override;
    QTableWidget *table() const;

    int row() const;

    int column() const;

private:
    void call_get_text(Processor *p);

    void call_set_text(Processor *p);

    void call_get_style(Processor *p);

    void buildFunctionsMap();
};

#endif // MYCELL_H
