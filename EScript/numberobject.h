#ifndef NUMBEROBJECT_H
#define NUMBEROBJECT_H

#include "automationobject.h"

namespace escript {

class ESCRIPT_EXPORT Number : public AutomationObject
{
    using BaseClass = AutomationObject;
    typedef void (Number::*pFn)(Processor *p);
    static std::map<std::u32string, pFn> _fn;

    SymbolType _type;
    union {
        int64_t intValue = 0;
        double realValue;
    };

public:
    /**
     * @brief Создаёт новый экземпляр класса Number
     */
    Number();
    Number(int64_t i) : _type(SymbolType::Integer), intValue(i) { };
    Number(double d) : _type(SymbolType::Real), realValue(d) { };
    /**
     * @brief Освобождает связанные с этим экземпляром ресурсы
     */
    virtual ~Number();


    // AutomationObject interface
public:
    virtual bool call(const std::u32string &method, Processor *p) override;
private:
    static void buildFunctionsMap();
    void call_parseInt(Processor *p);
    void call_parseFloat(Processor *p);
    void call_toString(Processor *p);
    std::u32string toString(int64_t value, int radix);
    std::u32string toString(double value, int radix);
};


} // namespace escript

#endif // NUMBEROBJECT_H
