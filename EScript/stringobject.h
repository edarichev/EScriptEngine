/**
 * @file stringobject.h
 * @brief Объект для хранения строки.
 *
 * Eugen-Script
 * The ECMAScript-like engine for C++ programs.
 * Copyright (C) 2023, Evgeny Darichev
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 */
#ifndef ESCRIPT_STRINGOBJECT_H
#define ESCRIPT_STRINGOBJECT_H

#include "automationobject.h"
#include <map>


namespace escript {

class Array;

class StringObject : public AutomationObject
{
    std::u32string _s;
    using BaseClass = AutomationObject;
    typedef void (StringObject::*pFn)(Processor *p);
    static std::map<std::u32string, pFn> _fn;
public:
    StringObject();
    StringObject(const std::u32string &s);
    StringObject(std::u32string &&s);
    StringObject (const char32_t *str);

    virtual bool call(const std::u32string &method, Processor *p) override;

    int64_t at(int64_t i) const;
    int64_t charCodeAt(int64_t i) const;
    std::u32string charAt(int64_t i) const;
    bool endsWith(const std::u32string &s) const;
    int compare(StringObject *s) const;
    int compare(const std::u32string &s) const;
    bool includes(const std::u32string &s) const;
    int64_t indexOf(const std::u32string &s) const;
    int64_t lastIndexOf(const std::u32string &s) const;
    int64_t length() const;
    std::u32string padEnd(int64_t width, const std::u32string &s) const;
    std::u32string padStart(int64_t width, const std::u32string &s) const;
    std::u32string repeat(int64_t i) const;
    std::u32string replace(const std::u32string &what, const std::u32string &with) const;
    std::u32string replaceAll(const std::u32string &what, const std::u32string &with) const;
    std::u32string slice(int64_t from, int64_t to) const;
    bool startsWith(const std::u32string &s, int64_t from) const;
    std::u32string toLowerCase() const;
    std::u32string toUpperCase() const;
    std::u32string trim() const;
    std::u32string trimStart() const;
    std::u32string trimEnd() const;
    Array *split(const std::u32string &sep);
    Array *split();


    StringObject &operator=(const std::u32string &str);

    bool operator==(const std::u32string &s) const;

    bool operator==(const char32_t *s) const;

    const std::u32string toString() const override;
    /**
     * @brief Версия toString, но возвращает ссылку, чтобы не делать лишних копий.
     * @return
     */
    const std::u32string &refString() const;

    static StringObject *concat(StringObject *s1, StringObject *s2);
    std::u32string substring(int64_t fromIndex, int64_t toIndex) const;
private:
    void call_get_length(Processor *p);
    void call_at(Processor *p);
    void call_charAt(Processor *p);
    void call_charCodeAt(Processor *p);
    void call_endsWith(Processor *p);
    void call_get(Processor *p);
    void call_set(Processor *p);
    void call_includes(Processor *p);
    void call_indexOf(Processor *p);
    void call_lastIndexOf(Processor *p);
    void call_padEnd(Processor *p);
    void call_padStart(Processor *p);
    void call_repeat(Processor *p);
    void call_replace(Processor *p);
    void call_replaceAll(Processor *p);
    void call_slice(Processor *p);
    void call_split(Processor *p);
    void call_startsWith(Processor *p);
    void call_substring(Processor *p);
    void call_toLowerCase(Processor *p);
    void call_toUpperCase(Processor *p);
    void call_trim(Processor *p);
    void call_trimStart(Processor *p);
    void call_trimEnd(Processor *p);
    static void buildFunctionsMap();
};

} // namespace escript

#endif // ESCRIPT_STRINGOBJECT_H
