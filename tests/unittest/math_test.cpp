#include "stdafx.h"
#include "math_test.h"



void Math_Test::run()
{
    initTestCase();
    test_constants();
    test_abs();
    test_acos();
    test_acosh();
    test_asin();
    test_asinh();
    test_atan();
    test_atan2();
    test_atanh();
    test_cbrt();
    test_ceil();
    test_clz32();
    test_cos();
    test_cosh();
    test_exp();
    test_expm1();
    test_floor();
    test_fround();
    test_hypot();
    test_imul();
    test_log();
    test_log10();
    test_log1p();
    test_log2();
    test_max();
    test_min();
    test_pow();
    test_random();
    test_round();
    test_sign();
    test_sin();
    test_sinh();
    test_sqrt();
    test_tan();
    test_tanh();
    test_trunc();
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

void Math_Test::test_abs()
{
    const std::u32string code1 = U"x = Math.abs(-1784.8797);";
    EScript engine;
    engine.eval(code1);
    auto mainTable = engine.unit()->block()->symbolTable();
    auto x = mainTable->find(U"x");
    auto record = engine.getObjectRecord(x);
    assert(record->type == SymbolType::Real);
    assert(Compare::equals_double(1784.8797, record->data));

    const char32_t *code2 = U"x = Math.abs(-187);";
    engine.eval(code2);
    x = mainTable->find(U"x");
    record = engine.getObjectRecord(x);
    assert(record->type == SymbolType::Integer);
    assert(Compare::equals_int64(187, record->data));
}

void Math_Test::test_acos()
{
    const std::u32string code1 = U"x = Math.acos(0.7);";
    EScript engine;
    engine.eval(code1);
    auto mainTable = engine.unit()->block()->symbolTable();
    auto x = mainTable->find(U"x");
    auto record = engine.getObjectRecord(x);
    assert(record->type == SymbolType::Real);
    assert(Compare::equals_double(acos(0.7), record->data));
}

void Math_Test::test_acosh()
{
    const std::u32string code1 = U"x = Math.acosh(4.78);";
    EScript engine;
    engine.eval(code1);
    auto mainTable = engine.unit()->block()->symbolTable();
    auto x = mainTable->find(U"x");
    auto record = engine.getObjectRecord(x);
    assert(record->type == SymbolType::Real);
    assert(Compare::equals_double(acosh(4.78), record->data));
}

void Math_Test::test_asin()
{
    const std::u32string code1 = U"x = Math.asin(0.7);";
    EScript engine;
    engine.eval(code1);
    auto mainTable = engine.unit()->block()->symbolTable();
    auto x = mainTable->find(U"x");
    auto record = engine.getObjectRecord(x);
    assert(record->type == SymbolType::Real);
    assert(Compare::equals_double(asin(0.7), record->data));
}

void Math_Test::test_asinh()
{
    const std::u32string code1 = U"x = Math.asinh(4.78);";
    EScript engine;
    engine.eval(code1);
    auto mainTable = engine.unit()->block()->symbolTable();
    auto x = mainTable->find(U"x");
    auto record = engine.getObjectRecord(x);
    assert(record->type == SymbolType::Real);
    assert(Compare::equals_double(asinh(4.78), record->data));
}

void Math_Test::test_atan()
{
    const std::u32string code1 = U"x = Math.atan(2.246461965);";
    EScript engine;
    engine.eval(code1);
    auto mainTable = engine.unit()->block()->symbolTable();
    auto x = mainTable->find(U"x");
    auto record = engine.getObjectRecord(x);
    assert(record->type == SymbolType::Real);
    assert(Compare::equals_double(atan(2.246461965), record->data));
}

void Math_Test::test_atan2()
{
    const std::u32string code1 = U"x = Math.atan2(9.65, 0.34);";
    EScript engine;
    engine.eval(code1);
    auto mainTable = engine.unit()->block()->symbolTable();
    auto x = mainTable->find(U"x");
    auto record = engine.getObjectRecord(x);
    assert(record->type == SymbolType::Real);
    assert(Compare::equals_double(atan2(9.65, 0.34), record->data));
}

void Math_Test::test_atanh()
{
    const std::u32string code1 = U"x = Math.atanh(0.246461965);";
    EScript engine;
    engine.eval(code1);
    auto mainTable = engine.unit()->block()->symbolTable();
    auto x = mainTable->find(U"x");
    auto record = engine.getObjectRecord(x);
    assert(record->type == SymbolType::Real);
    assert(Compare::equals_double(atanh(0.246461965), record->data));
}

void Math_Test::test_cbrt()
{
    const std::u32string code1 = U"x = Math.cbrt(27);";
    EScript engine;
    engine.eval(code1);
    auto mainTable = engine.unit()->block()->symbolTable();
    auto x = mainTable->find(U"x");
    auto record = engine.getObjectRecord(x);
    assert(record->type == SymbolType::Real);
    assert(Compare::equals_double(cbrt(27), record->data));
}

void Math_Test::test_ceil()
{
    const std::u32string code1 = U"x = Math.ceil(27.8);";
    EScript engine;
    engine.eval(code1);
    auto mainTable = engine.unit()->block()->symbolTable();
    auto x = mainTable->find(U"x");
    auto record = engine.getObjectRecord(x);
    assert(record->type == SymbolType::Real);
    assert(Compare::equals_double(ceil(27.8), record->data));
}

void Math_Test::test_clz32()
{
    // 696320 == 0000 0000 0000 1010 1010 0000 0000 0000 = 12 нулей
    const std::u32string code1 = U"x = Math.clz32(696320);";
    EScript engine;
    engine.eval(code1);
    auto mainTable = engine.unit()->block()->symbolTable();
    auto x = mainTable->find(U"x");
    auto record = engine.getObjectRecord(x);
    assert(record->type == SymbolType::Integer);
    assert(Compare::equals_int64(12, record->data));
}

void Math_Test::test_cos()
{
    const std::u32string code1 = U"x = Math.cos(47.9);";
    EScript engine;
    engine.eval(code1);
    auto mainTable = engine.unit()->block()->symbolTable();
    auto x = mainTable->find(U"x");
    auto record = engine.getObjectRecord(x);
    assert(record->type == SymbolType::Real);
    assert(Compare::equals_double(cos(47.9), record->data));
}

void Math_Test::test_cosh()
{
    const std::u32string code1 = U"x = Math.cosh(4.77);";
    EScript engine;
    engine.eval(code1);
    auto mainTable = engine.unit()->block()->symbolTable();
    auto x = mainTable->find(U"x");
    auto record = engine.getObjectRecord(x);
    assert(record->type == SymbolType::Real);
    assert(Compare::equals_double(cosh(4.77), record->data));
}

void Math_Test::test_exp()
{
    const std::u32string code1 = U"x = Math.exp(4.77);";
    EScript engine;
    engine.eval(code1);
    auto mainTable = engine.unit()->block()->symbolTable();
    auto x = mainTable->find(U"x");
    auto record = engine.getObjectRecord(x);
    assert(record->type == SymbolType::Real);
    assert(Compare::equals_double(exp(4.77), record->data));
}

void Math_Test::test_expm1()
{
    const std::u32string code1 = U"x = Math.expm1(1.77);";
    EScript engine;
    engine.eval(code1);
    auto mainTable = engine.unit()->block()->symbolTable();
    auto x = mainTable->find(U"x");
    auto record = engine.getObjectRecord(x);
    assert(record->type == SymbolType::Real);
    assert(Compare::equals_double(expm1(1.77), record->data));
}

void Math_Test::test_floor()
{
    const std::u32string code1 = U"x = Math.floor(19.77);";
    EScript engine;
    engine.eval(code1);
    auto mainTable = engine.unit()->block()->symbolTable();
    auto x = mainTable->find(U"x");
    auto record = engine.getObjectRecord(x);
    assert(record->type == SymbolType::Real);
    assert(Compare::equals_double(floor(19.77), record->data));
}

void Math_Test::test_fround()
{
    const std::u32string code1 = U"x = Math.fround(197114284779824.5677);";
    EScript engine;
    engine.eval(code1);
    auto mainTable = engine.unit()->block()->symbolTable();
    auto x = mainTable->find(U"x");
    auto record = engine.getObjectRecord(x);
    assert(record->type == SymbolType::Real);
    assert(Compare::equals_double(round((float)197114284779824.5677), record->data));
}

void Math_Test::test_hypot()
{
    const std::u32string code1 = U"x = Math.hypot(19, 71);";
    EScript engine;
    engine.eval(code1);
    auto mainTable = engine.unit()->block()->symbolTable();
    auto x = mainTable->find(U"x");
    auto record = engine.getObjectRecord(x);
    assert(record->type == SymbolType::Real);
    assert(Compare::equals_double(hypot(19, 71), record->data));
}

void Math_Test::test_imul()
{
    const std::u32string code1 = U"x = Math.imul(19.1, 71);";
    EScript engine;
    engine.eval(code1);
    auto mainTable = engine.unit()->block()->symbolTable();
    auto x = mainTable->find(U"x");
    auto record = engine.getObjectRecord(x);
    assert(record->type == SymbolType::Integer);
    assert(Compare::equals_int64(19 * 71, record->data));
}

void Math_Test::test_log()
{
    const std::u32string code1 = U"x = Math.log(19.77);";
    EScript engine;
    engine.eval(code1);
    auto mainTable = engine.unit()->block()->symbolTable();
    auto x = mainTable->find(U"x");
    auto record = engine.getObjectRecord(x);
    assert(record->type == SymbolType::Real);
    assert(Compare::equals_double(log(19.77), record->data));
}

void Math_Test::test_log10()
{
    const std::u32string code1 = U"x = Math.log10(719.7);";
    EScript engine;
    engine.eval(code1);
    auto mainTable = engine.unit()->block()->symbolTable();
    auto x = mainTable->find(U"x");
    auto record = engine.getObjectRecord(x);
    assert(record->type == SymbolType::Real);
    assert(Compare::equals_double(log10(719.7), record->data));
}

void Math_Test::test_log1p()
{
    const std::u32string code1 = U"x = Math.log1p(9.7);";
    EScript engine;
    engine.eval(code1);
    auto mainTable = engine.unit()->block()->symbolTable();
    auto x = mainTable->find(U"x");
    auto record = engine.getObjectRecord(x);
    assert(record->type == SymbolType::Real);
    assert(Compare::equals_double(log1p(9.7), record->data));
}

void Math_Test::test_log2()
{
    const std::u32string code1 = U"x = Math.log2(19.17);";
    EScript engine;
    engine.eval(code1);
    auto mainTable = engine.unit()->block()->symbolTable();
    auto x = mainTable->find(U"x");
    auto record = engine.getObjectRecord(x);
    assert(record->type == SymbolType::Real);
    assert(Compare::equals_double(log2(19.17), record->data));
}

void Math_Test::test_max()
{
    const std::u32string code1 = U"x = Math.max(1, 9.17, 3, -9.5, 9.16);";
    EScript engine;
    engine.eval(code1);
    auto mainTable = engine.unit()->block()->symbolTable();
    auto x = mainTable->find(U"x");
    auto record = engine.getObjectRecord(x);
    assert(record->type == SymbolType::Real);
    assert(Compare::equals_double(9.17, record->data));
}

void Math_Test::test_min()
{
    const std::u32string code1 = U"x = Math.min(1, 9.17, 3, -9.5, 9.16);";
    EScript engine;
    engine.eval(code1);
    auto mainTable = engine.unit()->block()->symbolTable();
    auto x = mainTable->find(U"x");
    auto record = engine.getObjectRecord(x);
    assert(record->type == SymbolType::Real);
    assert(Compare::equals_double(-9.5, record->data));
}

void Math_Test::test_pow()
{
    const std::u32string code1 = U"x = Math.pow(7.111, 9.17);";
    EScript engine;
    engine.eval(code1);
    auto mainTable = engine.unit()->block()->symbolTable();
    auto x = mainTable->find(U"x");
    auto record = engine.getObjectRecord(x);
    assert(record->type == SymbolType::Real);
    assert(Compare::equals_double(pow(7.111, 9.17), record->data));
}

void Math_Test::test_random()
{
    const std::u32string code1 = U"x = Math.random();";
    EScript engine;
    engine.eval(code1);
    auto mainTable = engine.unit()->block()->symbolTable();
    auto x = mainTable->find(U"x");
    auto record = engine.getObjectRecord(x);
    assert(record->type == SymbolType::Real);
    double data = bit_cast<double>(record->data);
    assert(data >= 0 && data < 1);
}

void Math_Test::test_round()
{
    const std::u32string code1 = U"x = Math.round(7.711);";
    EScript engine;
    engine.eval(code1);
    auto mainTable = engine.unit()->block()->symbolTable();
    auto x = mainTable->find(U"x");
    auto record = engine.getObjectRecord(x);
    assert(record->type == SymbolType::Real);
    assert(Compare::equals_double(round(7.711), record->data));
}

void Math_Test::test_sign()
{
    const std::u32string code1 = U"x = Math.sign(-7.711);";
    EScript engine;
    engine.eval(code1);
    auto mainTable = engine.unit()->block()->symbolTable();
    auto x = mainTable->find(U"x");
    auto record = engine.getObjectRecord(x);
    assert(record->type == SymbolType::Integer);
    assert(Compare::equals_int64(-1, record->data));
}

void Math_Test::test_sin()
{
    const std::u32string code1 = U"x = Math.sin(17.711);";
    EScript engine;
    engine.eval(code1);
    auto mainTable = engine.unit()->block()->symbolTable();
    auto x = mainTable->find(U"x");
    auto record = engine.getObjectRecord(x);
    assert(record->type == SymbolType::Real);
    assert(Compare::equals_double(sin(17.711), record->data));
}

void Math_Test::test_sinh()
{
    const std::u32string code1 = U"x = Math.sinh(0.11);";
    EScript engine;
    engine.eval(code1);
    auto mainTable = engine.unit()->block()->symbolTable();
    auto x = mainTable->find(U"x");
    auto record = engine.getObjectRecord(x);
    assert(record->type == SymbolType::Real);
    assert(Compare::equals_double(sinh(0.11), record->data));
}

void Math_Test::test_sqrt()
{
    const std::u32string code1 = U"x = Math.sqrt(100);";
    EScript engine;
    engine.eval(code1);
    auto mainTable = engine.unit()->block()->symbolTable();
    auto x = mainTable->find(U"x");
    auto record = engine.getObjectRecord(x);
    assert(record->type == SymbolType::Real);
    assert(Compare::equals_double(sqrt(100), record->data));
}

void Math_Test::test_tan()
{
    const std::u32string code1 = U"x = Math.tan(0.5);";
    EScript engine;
    engine.eval(code1);
    auto mainTable = engine.unit()->block()->symbolTable();
    auto x = mainTable->find(U"x");
    auto record = engine.getObjectRecord(x);
    assert(record->type == SymbolType::Real);
    assert(Compare::equals_double(tan(0.5), record->data));
}

void Math_Test::test_tanh()
{
    const std::u32string code1 = U"x = Math.tanh(9.11);";
    EScript engine;
    engine.eval(code1);
    auto mainTable = engine.unit()->block()->symbolTable();
    auto x = mainTable->find(U"x");
    auto record = engine.getObjectRecord(x);
    assert(record->type == SymbolType::Real);
    assert(Compare::equals_double(tanh(9.11), record->data));
}

void Math_Test::test_trunc()
{
    const std::u32string code1 = U"x = Math.trunc(7.711);";
    EScript engine;
    engine.eval(code1);
    auto mainTable = engine.unit()->block()->symbolTable();
    auto x = mainTable->find(U"x");
    auto record = engine.getObjectRecord(x);
    assert(record->type == SymbolType::Real);
    assert(Compare::equals_double(trunc(7.711), record->data));
}
