#include "stdafx.h"
#include "math_test.h"



void Math_Test::run()
{
    initTestCase();
    test_constants();
    cleanupTestCase();
}

void Math_Test::initTestCase()
{

}

void Math_Test::cleanupTestCase()
{

}

void Math_Test::test_constants()
{
    const char32_t *code1 = UR"(
ce = Math.E;
cln10 = Math.LN10;
cln2 = Math.LN2;
clog10e = Math.LOG10E;
clog2e = Math.LOG2E;
cpi = Math.PI;
csqrt12 = Math.SQRT1_2;
csqrt2 = Math.SQRT2;
)";
    EScript engine;
    engine.eval(code1);
    auto mainTable = engine.unit()->block()->symbolTable();
    auto ce = mainTable->find(U"ce");
    auto record = engine.getObjectRecord(ce);
    assert(record->type == SymbolType::Real);
    assert(Compare::equals_double(Math::E, record->data));

    auto cln10 = mainTable->find(U"cln10");
    record = engine.getObjectRecord(cln10);
    assert(record->type == SymbolType::Real);
    assert(Compare::equals_double(Math::LN10, record->data));

    auto cln2 = mainTable->find(U"cln2");
    record = engine.getObjectRecord(cln2);
    assert(record->type == SymbolType::Real);
    assert(Compare::equals_double(Math::LN2, record->data));

    auto clog10e = mainTable->find(U"clog10e");
    record = engine.getObjectRecord(clog10e);
    assert(record->type == SymbolType::Real);
    assert(Compare::equals_double(Math::LOG10E, record->data));

    auto clog2e = mainTable->find(U"clog2e");
    record = engine.getObjectRecord(clog2e);
    assert(record->type == SymbolType::Real);
    assert(Compare::equals_double(Math::LOG2E, record->data));

    auto cpi = mainTable->find(U"cpi");
    record = engine.getObjectRecord(cpi);
    assert(record->type == SymbolType::Real);
    assert(Compare::equals_double(Math::PI, record->data));

    auto csqrt12 = mainTable->find(U"csqrt12");
    record = engine.getObjectRecord(csqrt12);
    assert(record->type == SymbolType::Real);
    assert(Compare::equals_double(Math::SQRT1_2, record->data));

    auto csqrt2 = mainTable->find(U"csqrt2");
    record = engine.getObjectRecord(csqrt2);
    assert(record->type == SymbolType::Real);
    assert(Compare::equals_double(Math::SQRT2, record->data));
}
