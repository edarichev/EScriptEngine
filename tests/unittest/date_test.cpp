#include "stdafx.h"
#include "date_test.h"




void Date_Test::run()
{
    initTestCase();
    test_parse();
    test_toString();
    test_toStringFormatted();
    test_timeParts();
    test_now();
    test_utcNow();
    test_create();
    test_toLocale();
    test_toUtc();
    cleanupTestCase();
}

void Date_Test::initTestCase()
{

}

void Date_Test::cleanupTestCase()
{

}

void Date_Test::test_parse()
{
    const u32string code1 = UR"(
x = DateTime.parse('04 Dec 1995 00:12:00 GMT');
y = DateTime.parse('04.12.1995 00:12:00');
z = DateTime.parse('04.12.1995 00:12:00', '%d.%M.%Y %H:%m:%S');
)";
    EScript engine;
    engine.eval(code1);
    auto mainTable = engine.unit()->block()->symbolTable();
    auto x = mainTable->find(U"x");
    auto record = engine.getObjectRecord(x);
    assert(record->type == SymbolType::Object);
    DateTimeObject d1("04 Dec 1995 00:12:00 GMT");
    DateTimeObject d2("04.12.1995 00:12:00");
    DateTimeObject *objX = (DateTimeObject*)record->data;
    assert(d1 == d2);
    assert(d1 == *objX);
    auto y = mainTable->find(U"y");
    record = engine.getObjectRecord(y);
    assert(record->type == SymbolType::Object);
    assert(d1 == *(DateTimeObject*)record->data);
    auto z = mainTable->find(U"z");
    record = engine.getObjectRecord(z);
    assert(record->type == SymbolType::Object);
    assert(d1 == *(DateTimeObject*)record->data);
}

void Date_Test::test_toString()
{
    const u32string code1 = UR"(
x = DateTime.parse('04 Dec 1995 00:12:00 GMT');
s = x.toString();
)";
    EScript engine;
    engine.eval(code1);
    auto mainTable = engine.unit()->block()->symbolTable();
    auto x = mainTable->find(U"s");
    auto record = engine.getObjectRecord(x);
    assert(record->type == SymbolType::String);
    const std::u32string sResult1 = U"1995-12-04T00:12:00Z";
    assert(Compare::equals_string(sResult1, record->data));
}

void Date_Test::test_toStringFormatted()
{
    const u32string code1 = UR"(
x = DateTime.parse('04 Dec 1995 00:12:00');
s = x.toString('%d.%M.%Y %H:%m:%S');
)";
    EScript engine;
    engine.eval(code1);
    auto mainTable = engine.unit()->block()->symbolTable();
    auto x = mainTable->find(U"s");
    auto record = engine.getObjectRecord(x);
    assert(record->type == SymbolType::String);
    const std::u32string sResult1 = U"04.12.1995 00:12:00";
    assert(Compare::equals_string(sResult1, record->data));
}

void Date_Test::test_timeParts()
{
    const u32string code1 = UR"(
x = DateTime.parse('04.12.1995 01:12:45', '%d.%M.%Y %H:%m:%S');
year = x.year();
mon = x.month();
day = x.day();
hour = x.hour();
minute = x.minute();
sec = x.second();
ms = x.ms();
)";
    EScript engine;
    engine.eval(code1);
    auto mainTable = engine.unit()->block()->symbolTable();

    auto record = engine.getObjectRecord(mainTable->find(U"year"));
    assert(record->type == SymbolType::Integer);
    assert(Compare::equals_int64(1995, record->data));

    record = engine.getObjectRecord(mainTable->find(U"mon"));
    assert(record->type == SymbolType::Integer);
    assert(Compare::equals_int64(12, record->data));

    record = engine.getObjectRecord(mainTable->find(U"day"));
    assert(record->type == SymbolType::Integer);
    assert(Compare::equals_int64(4, record->data));

    record = engine.getObjectRecord(mainTable->find(U"hour"));
    assert(record->type == SymbolType::Integer);
    assert(Compare::equals_int64(1, record->data));

    record = engine.getObjectRecord(mainTable->find(U"minute"));
    assert(record->type == SymbolType::Integer);
    assert(Compare::equals_int64(12, record->data));

    record = engine.getObjectRecord(mainTable->find(U"sec"));
    assert(record->type == SymbolType::Integer);
    assert(Compare::equals_int64(45, record->data));

    record = engine.getObjectRecord(mainTable->find(U"ms"));
    assert(record->type == SymbolType::Integer);
    assert(Compare::equals_int64(0, record->data));
}

void Date_Test::test_now()
{
    const u32string code1 = UR"(
x = DateTime.now();
)";
    EScript engine;
    engine.eval(code1);
    auto mainTable = engine.unit()->block()->symbolTable();
    auto record = engine.getObjectRecord(mainTable->find(U"x"));
    assert(record->type == SymbolType::Object);
    DateTimeObject *objX = (DateTimeObject*)record->data;
    assert(objX->year());
}

void Date_Test::test_utcNow()
{ // в отладке посмотреть, а как ещё?
    const u32string code1 = UR"(
x = DateTime.utcNow();
)";
    EScript engine;
    engine.eval(code1);
    auto mainTable = engine.unit()->block()->symbolTable();
    auto record = engine.getObjectRecord(mainTable->find(U"x"));
    assert(record->type == SymbolType::Object);
    DateTimeObject *objX = (DateTimeObject*)record->data;
    assert(objX->year());
}

void Date_Test::test_create()
{
    const u32string code1 = UR"(
x = DateTime.create(2023, 8, 31, 19, 57, 45, 123);
y = DateTime.create();
)";
    EScript engine;
    engine.eval(code1);
    auto mainTable = engine.unit()->block()->symbolTable();
    auto record = engine.getObjectRecord(mainTable->find(U"x"));
    assert(record->type == SymbolType::Object);
    DateTimeObject *dt = (DateTimeObject*)record->data;
    assert(dt->year() == 2023);
    assert(dt->month() == 8);
    assert(dt->day() == 31);
    assert(dt->hour() == 19);
    assert(dt->minute() == 57);
    assert(dt->second() == 45);
    assert(dt->ms() == 123);
}

void Date_Test::test_toLocale()
{
    const u32string code1 = UR"(
x = DateTime.now();
y = DateTime.utcNow();
z = y.toLocale();
t = x.toUtc();
)";
    EScript engine;
    engine.eval(code1);
    auto mainTable = engine.unit()->block()->symbolTable();

    auto record = engine.getObjectRecord(mainTable->find(U"x"));
    DateTimeObject *x = (DateTimeObject*)record->data;

    record = engine.getObjectRecord(mainTable->find(U"z"));
    DateTimeObject *z = (DateTimeObject*)record->data;

    assert(*x == *z);
}

void Date_Test::test_toUtc()
{
    const u32string code1 = UR"(
x = DateTime.now();
y = DateTime.utcNow();
t = x.toUtc();
)";
    EScript engine;
    engine.eval(code1);
    auto mainTable = engine.unit()->block()->symbolTable();

    auto record = engine.getObjectRecord(mainTable->find(U"x"));
    DateTimeObject *y = (DateTimeObject*)record->data;

    record = engine.getObjectRecord(mainTable->find(U"t"));
    DateTimeObject *t = (DateTimeObject*)record->data;
    assert(*y == *t);
}
