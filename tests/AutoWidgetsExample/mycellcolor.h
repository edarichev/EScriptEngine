#ifndef MYCELLCOLOR_H
#define MYCELLCOLOR_H

using namespace escript;

class MyCell;

class MyCellColor : public AutomationObject
{
    using BaseClass = AutomationObject;
    using pFn = void (MyCellColor::*)(Processor *p);
    static std::map<std::u32string, pFn> _fn;
    std::shared_ptr<MyCell> _cell;
public:
    MyCellColor(std::shared_ptr<MyCell> cell);
    void set(const std::u32string &c);
    virtual bool call(const std::u32string &method, Processor *p) override;
    void buildFunctionsMap();
    void call_toString(Processor *p);
};

#endif // MYCELLCOLOR_H
