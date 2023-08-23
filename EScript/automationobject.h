#ifndef ESCRIPT_AUTOMATIONOBJECT_H
#define ESCRIPT_AUTOMATIONOBJECT_H

#include "EScript_global.h"
#include "stackvalue.h"

#define OBJECT_EXISTS_MARK 0xCCCCCCCC

namespace escript {

class EScript;
class Processor;

/**
 * @brief Базовый класс для всех обёрток, в т.ч. классов в виртуальной машине.
 * @details Чтобы сделать свойство, имя метода должно начинаться с "get_"
 */
class ESCRIPT_EXPORT AutomationObject
{
    unsigned int _mark = OBJECT_EXISTS_MARK;
    typedef void (AutomationObject::*pFn)(Processor *p);
    static std::map<std::u32string, pFn> _fn;
    int64_t _counter = 0;
protected:
    bool _destructible = false;
public:
    AutomationObject();
    virtual ~AutomationObject();
    virtual bool call([[maybe_unused]] const std::u32string &method,
                      [[maybe_unused]] Processor *p);
    void release();
    void addRef();
    int64_t counter() const;;

    std::stack<StackValue> loadArguments(Processor *p) const;
    static bool exists(AutomationObject *pObject);
    /**
     * @brief Возвращает true, если этот объект может быть удалён автоматически
     *        при разрушении объекта движка. Обычно true установлено у
     *        встроенных (стандартных) объектов.
     *        По умолчанию равно false,
     *        так что объекты производных пользовательских классов
     *        могут не беспокоиться, что они будут удалены движком.
     * @return
     */
    bool destructible() const;

private:
    void call_toString(Processor *p);
    static void buildFunctionsMap();
};


} // namespace escript

#endif // ESCRIPT_AUTOMATIONOBJECT_H
