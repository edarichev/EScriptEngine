#ifndef ESCRIPT_CONSOLE_H
#define ESCRIPT_CONSOLE_H

#include <vector>
#include "automationobject.h"

namespace escript {

class ESCRIPT_EXPORT Console : public escript::AutomationObject
{
    using BaseClass = AutomationObject;
    std::ostream *_outStream = nullptr;
    bool _separateWithSpace = true;
public:
    /**
     * @brief Создаёт новый экземпляр класса Console
     */
    Console();
    /**
     * @brief Освобождает связанные с этим экземпляром ресурсы
     */
    virtual ~Console();
    void log(std::vector<std::u32string> &args);
    void setOutputStream(std::ostream &s);
    // AutomationObject interface
public:
    virtual bool call(const std::u32string &method, Processor *p) override;
    bool separateWithSpace() const;
    void setSeparateWithSpace(bool newSeparateWithSpace);
};

} // namespace escript

#endif // ESCRIPT_CONSOLE_H
