#ifndef ESCRIPT_AUTOMATIONOBJECT_H
#define ESCRIPT_AUTOMATIONOBJECT_H

#include "EScript_global.h"
#include "stackvalue.h"

namespace escript {

class EScript;
class Processor;

/**
 * @brief Базовый класс для всех обёрток, в т.ч. классов в виртуальной машине.
 * @details Чтобы сделать свойство, имя метода должно начинаться с "get_"
 */
class ESCRIPT_EXPORT AutomationObject
{
    typedef void (AutomationObject::*pFn)(Processor *p);
    static std::map<std::u32string, pFn> _fn;
public:
    AutomationObject();;
    virtual ~AutomationObject(){}
    virtual bool call([[maybe_unused]] const std::u32string &method,
                      [[maybe_unused]] Processor *p);
    void release()
    {
        delete this;
    }
    std::stack<StackValue> loadArguments(Processor *p) const;
private:
    void call_toString(Processor *p);
    static void buildFunctionsMap();
};


} // namespace escript

#endif // ESCRIPT_AUTOMATIONOBJECT_H
