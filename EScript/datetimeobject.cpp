/**
 * @file dateobject.cpp
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
#include "stdafx.h"
#include "datetimeobject.h"
#include <ctime>
#include <iomanip>
#include "processor.h"

namespace escript {

std::map<std::u32string, DateTimeObject::pFn> DateTimeObject::_fn;

DateTimeObject::DateTimeObject()
{
    _managed = true;
    buildFunctionsMap();
}

DateTimeObject::DateTimeObject(long msSinceEpoch)
    : DateTimeObject()
{
    std::chrono::time_point<std::chrono::system_clock, std::chrono::milliseconds>
        tp{std::chrono::milliseconds{msSinceEpoch}};
    _value = tp;
}

DateTimeObject::DateTimeObject(int y, int mon, int d, int h, int m, int s, int millis)
    : DateTimeObject()
{
    tm timeinfo1 = tm();
    timeinfo1.tm_year = y - 1900;
    timeinfo1.tm_mon = mon - 1;
    timeinfo1.tm_mday = d;
    timeinfo1.tm_hour = h;
    timeinfo1.tm_min = m;
    timeinfo1.tm_sec = s;
    tm timeinfo = timeinfo1;
    time_t tt = timegm(&timeinfo);
    auto tp = std::chrono::system_clock::from_time_t(tt);
    tp += std::chrono::milliseconds(millis);
    _value = tp;
}

DateTimeObject::DateTimeObject(const std::chrono::system_clock::time_point &tp) noexcept
    : DateTimeObject()
{
    _value = tp;
}

DateTimeObject::DateTimeObject(const std::u32string &s)
    : DateTimeObject(to_utf8(s), "")
{

}

DateTimeObject::DateTimeObject(const std::u32string &s,
                               const std::u32string &format)
    : DateTimeObject(to_utf8(s), to_utf8(format))
{

}

DateTimeObject::DateTimeObject(const std::string &s)
    : DateTimeObject(s, "")
{

}

DateTimeObject::DateTimeObject(const std::string &s, const std::string &format)
    : DateTimeObject()
{
    _value = fromString(s, format);
}

bool DateTimeObject::operator==(const DateTimeObject &dt) const
{
    return _value == dt._value;
}

DateTimeObject::~DateTimeObject()
{

}

int DateTimeObject::year() const
{
    std::time_t tt = std::chrono::system_clock::to_time_t(_value);
    auto tm = std::gmtime(&tt);
    return 1900 + tm->tm_year;
}

int DateTimeObject::month() const
{
    std::time_t tt = std::chrono::system_clock::to_time_t(_value);
    auto tm = std::gmtime(&tt);
    return 1 + tm->tm_mon;
}

int DateTimeObject::day() const
{
    std::time_t tt = std::chrono::system_clock::to_time_t(_value);
    auto tm = std::gmtime(&tt);
    return tm->tm_mday;
}

int DateTimeObject::hour() const
{
    std::time_t tt = std::chrono::system_clock::to_time_t(_value);
    auto tm = std::gmtime(&tt);
    return tm->tm_hour;
}

int DateTimeObject::minute() const
{
    std::time_t tt = std::chrono::system_clock::to_time_t(_value);
    auto tm = std::gmtime(&tt);
    return tm->tm_min;
}

int DateTimeObject::second() const
{
    std::time_t tt = std::chrono::system_clock::to_time_t(_value);
    auto tm = std::gmtime(&tt);
    return tm->tm_sec;
}

int DateTimeObject::ms() const
{
    auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(_value.time_since_epoch()) -
              std::chrono::duration_cast<std::chrono::seconds>(_value.time_since_epoch());
    return ms.count();
}

const std::u32string DateTimeObject::toString() const
{
    return to_u32string(timeToString(_value, ""));
}

bool DateTimeObject::call(const std::u32string &method, Processor *p)
{
    if (method != U"toString" && BaseClass::call(method, p))
        return true;
    auto ptrToMethod = _fn.find(method);
    if (ptrToMethod == _fn.end())
        throw std::domain_error("Call of unknown method: Date." + to_utf8(method));
    (this->*ptrToMethod->second)(p);
    return true;
}

void DateTimeObject::buildFunctionsMap()
{
    if (!_fn.empty())
        return;
    _fn[U"parse"] = &DateTimeObject::call_parse;
    _fn[U"toString"] = &DateTimeObject::call_toString;
    _fn[U"year"] = &DateTimeObject::call_year;
    _fn[U"month"] = &DateTimeObject::call_month;
    _fn[U"day"] = &DateTimeObject::call_day;
    _fn[U"hour"] = &DateTimeObject::call_hour;
    _fn[U"minute"] = &DateTimeObject::call_minute;
    _fn[U"second"] = &DateTimeObject::call_second;
    _fn[U"ms"] = &DateTimeObject::call_ms;
    _fn[U"create"] = &DateTimeObject::call_create;
    _fn[U"now"] = &DateTimeObject::call_now;
    _fn[U"utcNow"] = &DateTimeObject::call_utcNow;
    _fn[U"toLocale"] = &DateTimeObject::call_toLocale;
    _fn[U"toUtc"] = &DateTimeObject::call_toUtc;
}

void DateTimeObject::call_parse(Processor *p)
{
    auto args = loadArguments(p);
    assert(!args.empty());
    std::u32string dtStr = args.top().getStringValue();
    args.pop();
    std::u32string format = args.empty() ? U"" : args.top().getStringValue();
    DateTimeObject *date = nullptr;
    try {
        date = new DateTimeObject(dtStr, format);
        p->pushObjectToStack(date);
    } catch (const std::exception &e) {
        delete date;
        throw;
    }
}

void DateTimeObject::call_toString(Processor *p)
{
    auto args = loadArguments(p);
    std::string format;
    if (args.empty())
        format = "%FT%TZ"; // ISO 8601
    else
        format = to_utf8(args.top().getStringValue());
    p->pushToStack(timeToString(_value, format));
}

void DateTimeObject::call_year(Processor *p)
{
    auto args = loadArguments(p);
    p->pushToStack(year());
}

void DateTimeObject::call_month(Processor *p)
{
    auto args = loadArguments(p);
    p->pushToStack(month());
}

void DateTimeObject::call_day(Processor *p)
{
    auto args = loadArguments(p);
    p->pushToStack(day());
}

void DateTimeObject::call_hour(Processor *p)
{
    auto args = loadArguments(p);
    p->pushToStack(hour());
}

void DateTimeObject::call_minute(Processor *p)
{
    auto args = loadArguments(p);
    p->pushToStack(minute());
}

void DateTimeObject::call_second(Processor *p)
{
    auto args = loadArguments(p);
    p->pushToStack(second());
}

void DateTimeObject::call_ms(Processor *p)
{
    auto args = loadArguments(p);
    p->pushToStack(ms());
}

DateTimeObject *escript::DateTimeObject::now()
{
    auto v = std::chrono::system_clock::now(); // это в UTC
    DateTimeObject dt0(v);
    auto ttUtc = std::chrono::system_clock::to_time_t(v);
    auto gt = localtime(&ttUtc);
    auto timetloc = timegm(gt);
    v = std::chrono::system_clock::from_time_t(timetloc);
    v += std::chrono::milliseconds(dt0.ms());
    DateTimeObject *dt = new DateTimeObject(v);
    return dt;
}

void DateTimeObject::call_now(Processor *p)
{
    auto args = loadArguments(p);
    p->pushObjectToStack(now());
}

void DateTimeObject::call_utcNow(Processor *p)
{
    auto args = loadArguments(p);
    DateTimeObject *dt = new DateTimeObject(std::chrono::system_clock::now());
    p->pushObjectToStack(dt);
}

void DateTimeObject::call_create(Processor *p)
{
    auto args = loadArguments(p);
    if (args.empty()) {
        p->pushObjectToStack(now());
        return;
    }
    const int n = 7;
    int parts[n] = {1900, 1, 1, 0, 0, 0, 0};
    int i = 0;
    while (i < n && !args.empty()) {
        parts[i] = args.top().getIntValue();
        args.pop();
        i++;
    }
    DateTimeObject *dt = nullptr;
    try {
        dt = new DateTimeObject(parts[0], // year
                parts[1], // month
                parts[2], // day
                parts[3], // hour
                parts[4], // minute
                parts[5], // sec
                parts[6]  // ms
                );
        p->pushObjectToStack(dt);
    } catch (const std::exception &e) {
        delete dt;
        throw;
    }
}

void DateTimeObject::call_toLocale(Processor *p)
{
    auto args = loadArguments(p);
    auto v = _value;
    auto ttUtc = std::chrono::system_clock::to_time_t(v);
    auto gt = localtime(&ttUtc);
    auto timetloc = timegm(gt);
    v = std::chrono::system_clock::from_time_t(timetloc);
    v += std::chrono::milliseconds(ms());
    DateTimeObject *dt = nullptr;
    try {
        dt = new DateTimeObject(v);
        p->pushObjectToStack(dt);
    } catch (const std::exception &e) {
        delete dt;
        throw;
    }
}

void DateTimeObject::call_toUtc(Processor *p)
{
    auto args = loadArguments(p);
    auto v = _value;
    auto ttUtc = std::chrono::system_clock::to_time_t(v);
    auto gt = gmtime(&ttUtc);
    auto timetloc = timegm(gt);
    v = std::chrono::system_clock::from_time_t(timetloc);
    v += std::chrono::milliseconds(ms());
    DateTimeObject *dt = nullptr;
    try {
        dt = new DateTimeObject(v);
        p->pushObjectToStack(dt);
    } catch (const std::exception &e) {
        delete dt;
        throw;
    }
}

std::chrono::time_point<std::chrono::system_clock> DateTimeObject::fromString(
        const std::string &str, const std::string &fmt)
{
    if (fmt.length()) {
        std::tm t {};
        std::istringstream ss(str);
        ss >> std::get_time(&t, fmt.c_str());
        if (!ss.fail() && ss.eof()) {
            std::time_t timet = timegm(&t);
            return std::chrono::system_clock::from_time_t(timet);
        }
        throw std::invalid_argument("Invalid datetime format");
    } else {
        const std::vector<std::string> formats = {
            "%Y-%m-%dT%H:%M:%SZ", // или "%FT%TZ", ISO 8601
            "%Y-%m-%d %H:%M:%S",  // без T и ms
            "%d.%m.%Y %H:%M:%S",  // RU
            "%d.%m.%Y",           // RU
            "%d %b %Y %H:%M:%S %Z",
            "%d %b %Y %H:%M:%S %z",
            "%Y/%m/%d %H:%M:%S",
            "%Y%m%d %H:%M:%S",
            "%Y-%m-%dT%H:%M:%S",
            "%Y/%m/%dT%H:%M:%S",
            "%Y-%m-%d",
            "%Y/%m/%d",
            "%Y%m%d"
        };
        for (const auto& format : formats) {
            std::tm t {};
            std::istringstream ss(str);
            ss >> std::get_time(&t, format.c_str());
            if (!ss.fail() && ss.eof()) {
                std::time_t timet = timegm(&t);
                return std::chrono::system_clock::from_time_t(timet);
            }
        }
        throw std::invalid_argument("Invalid datetime format");
    }
}

std::string DateTimeObject::timeToString(const std::chrono::time_point<std::chrono::system_clock> &t,
                                     const std::string &format)
{
    std::string fmt = format.length() ? format : "%Y-%m-%dT%H:%M:%SZ";
    std::time_t tt = std::chrono::system_clock::to_time_t(t);
    std::tm tm = *std::gmtime(&tt); //GMT (UTC)

    //std::tm tm = *std::localtime(&tt); //Locale time-zone
    std::stringstream ss;
    ss << std::put_time(&tm, fmt.c_str());
    auto s = ss.str();
    return s;
}


} // namespace escript
