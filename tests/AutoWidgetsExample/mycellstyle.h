#ifndef MYCELLSTYLE_H
#define MYCELLSTYLE_H

#include "mycellcolor.h"

class MyCell;

class MyCellStyle : public AutomationObject,
        public std::enable_shared_from_this<MyCellStyle>
{
    using BaseClass = AutomationObject;
    using pFn = void (MyCellStyle::*)(Processor *p);
    static std::map<std::u32string, pFn> _fn;
    std::shared_ptr<MyCellColor> _color;
    std::shared_ptr<MyCell> _cell;
public:
    MyCellStyle(std::shared_ptr<MyCell> cell);

    virtual bool call(const std::u32string &method, Processor *p) override;

    void buildFunctionsMap();

    void call_get_color(Processor *p);

    void call_set_color(Processor *p);
};

#endif // MYCELLSTYLE_H
