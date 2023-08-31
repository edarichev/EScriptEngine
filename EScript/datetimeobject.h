/**
 * @file dateobject.h
 * @brief Класс для даты и времени
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
#ifndef ESCRIPT_DATEOBJECT_H
#define ESCRIPT_DATEOBJECT_H

#include "automationobject.h"
#include <chrono>

namespace escript {

/**
 * @brief Класс для даты и времени
 * @details Сильно отличается от традиционного Date в JavaScript.
 */
class ESCRIPT_EXPORT DateTimeObject final : public escript::AutomationObject
{
    using BaseClass = AutomationObject;
    typedef void (DateTimeObject::*pFn)(Processor *p);
    static std::map<std::u32string, pFn> _fn;
    std::chrono::time_point<std::chrono::system_clock> _value;
public:
    /**
     * @brief Создаёт новый экземпляр класса DateObject
     */
    DateTimeObject();
    DateTimeObject(long msSinceEpoch);
    DateTimeObject(int y, int mon = 1, int d = 1, int h = 0, int m = 0, int s = 0, int millis = 0);
    DateTimeObject(const std::chrono::system_clock::time_point &tp) noexcept;
    DateTimeObject(const std::u32string &s);
    DateTimeObject(const std::u32string &s, const std::u32string &format);
    DateTimeObject(const std::string &s);
    DateTimeObject(const std::string &s, const std::string &format);
    bool operator==(const DateTimeObject &dt) const;
    /**
     * @brief Освобождает связанные с этим экземпляром ресурсы
     */
    virtual ~DateTimeObject();
public:
    int year() const;
    int month() const;
    int day() const;
    int hour() const;
    int minute() const;
    int second() const;
    int ms() const;
    const std::u32string toString() const;
    // AutomationObject interface
public:
    virtual bool call(const std::u32string &method, Processor *p) override;
private:
    void buildFunctionsMap();
    void call_parse(Processor *p);
    void call_toString(Processor *p);
    void call_year(Processor *p);
    void call_month(Processor *p);
    void call_day(Processor *p);
    void call_hour(Processor *p);
    void call_minute(Processor *p);
    void call_second(Processor *p);
    void call_ms(Processor *p);
    void call_now(Processor *p);
    void call_utcNow(Processor *p);
    void call_create(Processor *p);
    void call_toLocale(Processor *p);
    void call_toUtc(Processor *p);
    static std::chrono::time_point<std::chrono::system_clock> fromString(const std::string &str, const std::string &fmt);
    static std::string timeToString(const std::chrono::time_point<std::chrono::system_clock> &t, const std::string &format);
};

} // namespace escript

#endif // ESCRIPT_DATEOBJECT_H
