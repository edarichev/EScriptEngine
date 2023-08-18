#ifndef ESCRIPT_AUTOMATIONOBJECT_H
#define ESCRIPT_AUTOMATIONOBJECT_H

#include "EScript_global.h"


namespace escript {

class EScript;
class Processor;

/**
 * @brief Базовый класс для всех обёрток, в т.ч. классов в виртуальной машине.
 * @details Чтобы сделать свойство, имя метода должно начинаться с "get_"
 */
class ESCRIPT_EXPORT AutomationObject
{
public:
    AutomationObject(){};
    virtual ~AutomationObject(){}
    virtual bool call([[maybe_unused]] const std::u32string &method,
                      [[maybe_unused]] Processor *p)
    {
        return false;
    }
    void release()
    {
        delete this;
    }
};

#if 0
// пока заглушка
#define CALL_AUTO_METHOD1(returnType, name, param1type)

#else
// TODO: Открыть позже, когда будет разработка объектов. Это работает:
#define CALL_AUTO_METHOD1(returnType, name, param1type) \
    if (method == #name) { \
        param1type param1 = 0; /* из стека извлечь */ \
        returnType result = name(param1); \
        std::cout << result << std::endl; /*поместить результат в стек*/\
        return true; \
    } \

#endif


} // namespace escript

#endif // ESCRIPT_AUTOMATIONOBJECT_H
