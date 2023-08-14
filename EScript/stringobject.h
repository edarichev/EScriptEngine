#ifndef ESCRIPT_STRINGOBJECT_H
#define ESCRIPT_STRINGOBJECT_H

#include "automationobject.h"

namespace escript {

class StringObject : public AutomationObject
{
    std::u32string _s;
    using BaseClass = AutomationObject;
public:
    StringObject();
    StringObject(const std::u32string &s);

    virtual bool call(const std::u32string &method, Processor *p) override;

    int64_t at(int64_t i);

    int64_t get_length();

    StringObject (const char32_t *str);

    StringObject &operator=(const std::u32string &str);

    bool operator==(const std::u32string &s);

    bool operator==(const char32_t *s);

    const std::u32string &uString() const;

    static StringObject *concat(StringObject *s1, StringObject *s2);
};

} // namespace escript

#endif // ESCRIPT_STRINGOBJECT_H
