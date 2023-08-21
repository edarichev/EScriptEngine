#include "stdafx.h"
#include "switchcase_test.h"


void SwitchCase_Test::run()
{
    initTestCase();
    test_emptySwitch();
    test_switchOneCase();
    test_switchOneCaseFallThrough();
    test_switchTwoCaseFirstTrue();
    test_switchTwoCaseFirstTrueFallThrough();
    test_switchTwoCaseFirstTrue2FallThrough();
    test_switchTwoCaseSecondTrue();
    test_switchTwoCaseSecondTrueFallThrough();
    test_switchTwoCaseSecondTrue2FallThrough();
    test_switchTwoCaseNoTrue();
    test_switchTwoCaseNoTrue1FallThrough();
    test_switchTwoCaseNoTrue2FallThrough();
    test_switchTwoCaseNoTrueBothFallThrough();
    test_switch3CaseMiddleTrue();
    test_switch3CaseMiddleTrue1FallThrough();
    test_switch3CaseMiddleTrue2FallThrough();
    test_switch3CaseMiddleTrue3FallThrough();
    test_switch3CaseMiddleTrue12FallThrough();
    test_switch3CaseMiddleTrue13FallThrough();
    test_switch3CaseMiddleTrue23FallThrough();
    test_switch3CaseFirstTrue();
    test_switch3CaseFirstTrue1FallThrough();
    test_switch3CaseFirstTrue2FallThrough();
    test_switch3CaseFirstTrue3FallThrough();
    test_switch3CaseFirstTrue12FallThrough();
    test_switch3CaseFirstTrue13FallThrough();
    test_switch3CaseFirstTrue23FallThrough();
    test_switch3CaseFirstTrue123FallThrough();
    test_switchAloneDefault();
    test_switchAloneDefaultFallThrough();
    test_switchTrueAndDefault();
    test_switchTrueAndDefaultCaseFallThrough();
    test_switchTrueAndDefaultDefaultFallThrough();
    test_switchTrueAndDefaultBothFallThrough();
    test_switchFalseAndDefault();
    test_switchFalseAndDefaultDefaultFallThrough();
    test_switchFalseAndDefaultCaseFallThrough();
    test_switchFalseAndDefaultBothFallThrough();
    test_switchDefaultAndTrue();
    test_switchDefaultAndTrueDefaultFallThrough();
    test_switchDefaultAndTrueCaseFallThrough();
    test_switchDefaultAndFalse();
    test_switchDefaultAndFalseCaseFallThrough();
    test_switchDefaultAndFalseDefaultFallThrough();
    test_switchDefaultAndFalseBothFallThrough();
    test_switchCaseFalseDefaultCaseTrue();
    test_switchCaseFalseDefaultCaseTrueDefaultFallThrough();
    test_mixedCases();
    test_expressionsInCases();
    cleanupTestCase();
}

void SwitchCase_Test::initTestCase()
{

}

void SwitchCase_Test::cleanupTestCase()
{

}

void SwitchCase_Test::test_emptySwitch()
{
    const std::u32string code1 = UR"(
x = 1;
switch (x) {
}
)";
    EScript engine;
    engine.setShowDisassembleListing(false);
    engine.setShowTCode(false);
    engine.eval(code1);
    auto mainTable = engine.unit()->block()->symbolTable();
    auto x = mainTable->find(U"x");
    auto record = engine.getObjectRecord(x);
    assert(record->type == SymbolType::Integer);
    assert(Compare::equals_int64(1, record->data));
}

void SwitchCase_Test::test_switchOneCase()
{
    const std::u32string code1 = UR"(
x = 1;
switch (x) {
case 1:
    x = 2;
    break;
}
)";
    EScript engine;
    engine.setShowDisassembleListing(false);
    engine.setShowTCode(false);
    engine.eval(code1);
    auto mainTable = engine.unit()->block()->symbolTable();
    auto x = mainTable->find(U"x");
    auto record = engine.getObjectRecord(x);
    assert(record->type == SymbolType::Integer);
    assert(Compare::equals_int64(2, record->data));
}

void SwitchCase_Test::test_switchOneCaseFallThrough()
{
    const std::u32string code1 = UR"(
x = 1;
switch (x) {
case 1:
    x = 2;
}
)";
    EScript engine;
    engine.setShowDisassembleListing(false);
    engine.setShowTCode(false);
    engine.eval(code1);
    auto mainTable = engine.unit()->block()->symbolTable();
    auto x = mainTable->find(U"x");
    auto record = engine.getObjectRecord(x);
    assert(record->type == SymbolType::Integer);
    assert(Compare::equals_int64(2, record->data));
}

void SwitchCase_Test::test_switchTwoCaseFirstTrue()
{
    const std::u32string code1 = UR"(
x = 1;
switch (x) {
case 1:
    x = 2;
    break;
case 2:
    x = 3;
    break;
}
)";
    EScript engine;
    engine.setShowDisassembleListing(false);
    engine.setShowTCode(false);
    engine.eval(code1);
    auto mainTable = engine.unit()->block()->symbolTable();
    auto x = mainTable->find(U"x");
    auto record = engine.getObjectRecord(x);
    assert(record->type == SymbolType::Integer);
    assert(Compare::equals_int64(2, record->data));
}

void SwitchCase_Test::test_switchTwoCaseFirstTrueFallThrough()
{
    const std::u32string code1 = UR"(
x = 1;
switch (x) {
case 1:
    x = 2;
case 2:
    x = 3;
    break;
}
)";
    EScript engine;
    engine.setShowDisassembleListing(false);
    engine.setShowTCode(false);
    engine.eval(code1);
    auto mainTable = engine.unit()->block()->symbolTable();
    auto x = mainTable->find(U"x");
    auto record = engine.getObjectRecord(x);
    assert(record->type == SymbolType::Integer);
    assert(Compare::equals_int64(3, record->data));
}

void SwitchCase_Test::test_switchTwoCaseFirstTrue2FallThrough()
{
    const std::u32string code1 = UR"(
x = 1;
switch (x) {
case 1:
    x = 2;
case 2:
    x = 3;
}
)";
    EScript engine;
    engine.setShowDisassembleListing(false);
    engine.setShowTCode(false);
    engine.eval(code1);
    auto mainTable = engine.unit()->block()->symbolTable();
    auto x = mainTable->find(U"x");
    auto record = engine.getObjectRecord(x);
    assert(record->type == SymbolType::Integer);
    assert(Compare::equals_int64(3, record->data));
}

void SwitchCase_Test::test_switchTwoCaseSecondTrue()
{
    const std::u32string code1 = UR"(
x = 1;
switch (x) {
case 2:
    x = 2;
    break;
case 1:
    x = 3;
    break;
}
)";
    EScript engine;
    engine.setShowDisassembleListing(false);
    engine.setShowTCode(false);
    engine.eval(code1);
    auto mainTable = engine.unit()->block()->symbolTable();
    auto x = mainTable->find(U"x");
    auto record = engine.getObjectRecord(x);
    assert(record->type == SymbolType::Integer);
    assert(Compare::equals_int64(3, record->data));
}

void SwitchCase_Test::test_switchTwoCaseSecondTrueFallThrough()
{
    const std::u32string code1 = UR"(
x = 1;
switch (x) {
case 2:
    x = 2;
    break;
case 1:
    x = 3;
}
)";
    EScript engine;
    engine.setShowDisassembleListing(false);
    engine.setShowTCode(false);
    engine.eval(code1);
    auto mainTable = engine.unit()->block()->symbolTable();
    auto x = mainTable->find(U"x");
    auto record = engine.getObjectRecord(x);
    assert(record->type == SymbolType::Integer);
    assert(Compare::equals_int64(3, record->data));
}

void SwitchCase_Test::test_switchTwoCaseSecondTrue2FallThrough()
{
    const std::u32string code1 = UR"(
x = 1;
switch (x) {
case 2:
    x = 2;
case 1:
    x = 3;
}
)";
    EScript engine;
    engine.setShowDisassembleListing(false);
    engine.setShowTCode(false);
    engine.eval(code1);
    auto mainTable = engine.unit()->block()->symbolTable();
    auto x = mainTable->find(U"x");
    auto record = engine.getObjectRecord(x);
    assert(record->type == SymbolType::Integer);
    assert(Compare::equals_int64(3, record->data));
}

void SwitchCase_Test::test_switchTwoCaseNoTrue()
{
    const std::u32string code1 = UR"(
x = 1;
switch (x) {
case 2:
    x = 2;
    break;
case 3:
    x = 3;
    break;
}
)";
    EScript engine;
    engine.setShowDisassembleListing(false);
    engine.setShowTCode(false);
    engine.eval(code1);
    auto mainTable = engine.unit()->block()->symbolTable();
    auto x = mainTable->find(U"x");
    auto record = engine.getObjectRecord(x);
    assert(record->type == SymbolType::Integer);
    assert(Compare::equals_int64(1, record->data));
}

void SwitchCase_Test::test_switchTwoCaseNoTrue1FallThrough()
{
    const std::u32string code1 = UR"(
x = 1;
switch (x) {
case 2:
    x = 2;
case 3:
    x = 3;
    break;
}
)";
    EScript engine;
    engine.setShowDisassembleListing(false);
    engine.setShowTCode(false);
    engine.eval(code1);
    auto mainTable = engine.unit()->block()->symbolTable();
    auto x = mainTable->find(U"x");
    auto record = engine.getObjectRecord(x);
    assert(record->type == SymbolType::Integer);
    assert(Compare::equals_int64(1, record->data));
}

void SwitchCase_Test::test_switchTwoCaseNoTrue2FallThrough()
{
    const std::u32string code1 = UR"(
x = 1;
switch (x) {
case 2:
    x = 2;
    break;
case 3:
    x = 3;
}
)";
    EScript engine;
    engine.setShowDisassembleListing(false);
    engine.setShowTCode(false);
    engine.eval(code1);
    auto mainTable = engine.unit()->block()->symbolTable();
    auto x = mainTable->find(U"x");
    auto record = engine.getObjectRecord(x);
    assert(record->type == SymbolType::Integer);
    assert(Compare::equals_int64(1, record->data));
}

void SwitchCase_Test::test_switchTwoCaseNoTrueBothFallThrough()
{
    const std::u32string code1 = UR"(
x = 1;
switch (x) {
case 2:
    x = 2;
case 3:
    x = 3;
}
)";
    EScript engine;
    engine.setShowDisassembleListing(false);
    engine.setShowTCode(false);
    engine.eval(code1);
    auto mainTable = engine.unit()->block()->symbolTable();
    auto x = mainTable->find(U"x");
    auto record = engine.getObjectRecord(x);
    assert(record->type == SymbolType::Integer);
    assert(Compare::equals_int64(1, record->data));
}

void SwitchCase_Test::test_switch3CaseMiddleTrue()
{
    const std::u32string code1 = UR"(
x = 1;
switch (x) {
case 2:
    x = 2;
    break;
case 1:
    x = 11;
    break;
case 3:
    x = 3;
    break;
}
)";
    EScript engine;
    engine.setShowDisassembleListing(false);
    engine.setShowTCode(false);
    engine.eval(code1);
    auto mainTable = engine.unit()->block()->symbolTable();
    auto x = mainTable->find(U"x");
    auto record = engine.getObjectRecord(x);
    assert(record->type == SymbolType::Integer);
    assert(Compare::equals_int64(11, record->data));
}

void SwitchCase_Test::test_switch3CaseMiddleTrue1FallThrough()
{
    const std::u32string code1 = UR"(
x = 1;
switch (x) {
case 2:
    x = 2;
case 1:
    x = 11;
    break;
case 3:
    x = 3;
    break;
}
)";
    EScript engine;
    engine.setShowDisassembleListing(false);
    engine.setShowTCode(false);
    engine.eval(code1);
    auto mainTable = engine.unit()->block()->symbolTable();
    auto x = mainTable->find(U"x");
    auto record = engine.getObjectRecord(x);
    assert(record->type == SymbolType::Integer);
    assert(Compare::equals_int64(11, record->data));
}

void SwitchCase_Test::test_switch3CaseMiddleTrue2FallThrough()
{
    const std::u32string code1 = UR"(
x = 1;
switch (x) {
case 2:
    x = 2;
    break;
case 1:
    x = 11;
case 3:
    x = 3;
    break;
}
)";
    EScript engine;
    engine.setShowDisassembleListing(false);
    engine.setShowTCode(false);
    engine.eval(code1);
    auto mainTable = engine.unit()->block()->symbolTable();
    auto x = mainTable->find(U"x");
    auto record = engine.getObjectRecord(x);
    assert(record->type == SymbolType::Integer);
    assert(Compare::equals_int64(3, record->data));
}

void SwitchCase_Test::test_switch3CaseMiddleTrue3FallThrough()
{
    const std::u32string code1 = UR"(
x = 1;
switch (x) {
case 2:
    x = 2;
    break;
case 1:
    x = 11;
    break;
case 3:
    x = 3;
}
)";
    EScript engine;
    engine.setShowDisassembleListing(false);
    engine.setShowTCode(false);
    engine.eval(code1);
    auto mainTable = engine.unit()->block()->symbolTable();
    auto x = mainTable->find(U"x");
    auto record = engine.getObjectRecord(x);
    assert(record->type == SymbolType::Integer);
    assert(Compare::equals_int64(11, record->data));
}

void SwitchCase_Test::test_switch3CaseMiddleTrue12FallThrough()
{
    const std::u32string code1 = UR"(
x = 1;
switch (x) {
case 2:
    x = 2;
case 1:
    x = 11;
case 3:
    x = 3;
    break;
}
)";
    EScript engine;
    engine.setShowDisassembleListing(false);
    engine.setShowTCode(false);
    engine.eval(code1);
    auto mainTable = engine.unit()->block()->symbolTable();
    auto x = mainTable->find(U"x");
    auto record = engine.getObjectRecord(x);
    assert(record->type == SymbolType::Integer);
    assert(Compare::equals_int64(3, record->data));
}

void SwitchCase_Test::test_switch3CaseMiddleTrue13FallThrough()
{
    const std::u32string code1 = UR"(
x = 1;
switch (x) {
case 2:
    x = 2;
case 1:
    x = 11;
    break;
case 3:
    x = 3;
}
)";
    EScript engine;
    engine.setShowDisassembleListing(false);
    engine.setShowTCode(false);
    engine.eval(code1);
    auto mainTable = engine.unit()->block()->symbolTable();
    auto x = mainTable->find(U"x");
    auto record = engine.getObjectRecord(x);
    assert(record->type == SymbolType::Integer);
    assert(Compare::equals_int64(11, record->data));
}

void SwitchCase_Test::test_switch3CaseMiddleTrue23FallThrough()
{
    const std::u32string code1 = UR"(
x = 1;
switch (x) {
case 2:
    x = 2;
    break;
case 1:
    x = 11;
case 3:
    x = 3;
}
)";
    EScript engine;
    engine.setShowDisassembleListing(false);
    engine.setShowTCode(false);
    engine.eval(code1);
    auto mainTable = engine.unit()->block()->symbolTable();
    auto x = mainTable->find(U"x");
    auto record = engine.getObjectRecord(x);
    assert(record->type == SymbolType::Integer);
    assert(Compare::equals_int64(3, record->data));
}

void SwitchCase_Test::test_switch3CaseFirstTrue()
{
    const std::u32string code1 = UR"(
x = 1;
switch (x) {
case 1:
    x = 11;
    break;
case 2:
    x = 2;
    break;
case 3:
    x = 3;
    break;
}
)";
    EScript engine;
    engine.setShowDisassembleListing(false);
    engine.setShowTCode(false);
    engine.eval(code1);
    auto mainTable = engine.unit()->block()->symbolTable();
    auto x = mainTable->find(U"x");
    auto record = engine.getObjectRecord(x);
    assert(record->type == SymbolType::Integer);
    assert(Compare::equals_int64(11, record->data));
}

void SwitchCase_Test::test_switch3CaseFirstTrue1FallThrough()
{
    const std::u32string code1 = UR"(
x = 1;
switch (x) {
case 1:
    x = 11;
case 2:
    x = 2;
    break;
case 3:
    x = 3;
    break;
}
)";
    EScript engine;
    engine.setShowDisassembleListing(false);
    engine.setShowTCode(false);
    engine.eval(code1);
    auto mainTable = engine.unit()->block()->symbolTable();
    auto x = mainTable->find(U"x");
    auto record = engine.getObjectRecord(x);
    assert(record->type == SymbolType::Integer);
    assert(Compare::equals_int64(2, record->data));
}

void SwitchCase_Test::test_switch3CaseFirstTrue2FallThrough()
{
    const std::u32string code1 = UR"(
x = 1;
switch (x) {
case 1:
    x = 11;
    break;
case 2:
    x = 2;
case 3:
    x = 3;
    break;
}
)";
    EScript engine;
    engine.setShowDisassembleListing(false);
    engine.setShowTCode(false);
    engine.eval(code1);
    auto mainTable = engine.unit()->block()->symbolTable();
    auto x = mainTable->find(U"x");
    auto record = engine.getObjectRecord(x);
    assert(record->type == SymbolType::Integer);
    assert(Compare::equals_int64(11, record->data));
}

void SwitchCase_Test::test_switch3CaseFirstTrue3FallThrough()
{
    const std::u32string code1 = UR"(
x = 1;
switch (x) {
case 1:
    x = 11;
    break;
case 2:
    x = 2;
    break;
case 3:
    x = 3;
}
)";
    EScript engine;
    engine.setShowDisassembleListing(false);
    engine.setShowTCode(false);
    engine.eval(code1);
    auto mainTable = engine.unit()->block()->symbolTable();
    auto x = mainTable->find(U"x");
    auto record = engine.getObjectRecord(x);
    assert(record->type == SymbolType::Integer);
    assert(Compare::equals_int64(11, record->data));
}

void SwitchCase_Test::test_switch3CaseFirstTrue12FallThrough()
{
    const std::u32string code1 = UR"(
x = 1;
switch (x) {
case 1:
    x = 11;
case 2:
    x = 2;
case 3:
    x = 3;
    break;
}
)";
    EScript engine;
    engine.setShowDisassembleListing(false);
    engine.setShowTCode(false);
    engine.eval(code1);
    auto mainTable = engine.unit()->block()->symbolTable();
    auto x = mainTable->find(U"x");
    auto record = engine.getObjectRecord(x);
    assert(record->type == SymbolType::Integer);
    assert(Compare::equals_int64(3, record->data));
}

void SwitchCase_Test::test_switch3CaseFirstTrue13FallThrough()
{
    const std::u32string code1 = UR"(
x = 1;
switch (x) {
case 1:
    x = 11;
case 2:
    x = 2;
    break;
case 3:
    x = 3;
}
)";
    EScript engine;
    engine.setShowDisassembleListing(false);
    engine.setShowTCode(false);
    engine.eval(code1);
    auto mainTable = engine.unit()->block()->symbolTable();
    auto x = mainTable->find(U"x");
    auto record = engine.getObjectRecord(x);
    assert(record->type == SymbolType::Integer);
    assert(Compare::equals_int64(2, record->data));
}

void SwitchCase_Test::test_switch3CaseFirstTrue23FallThrough()
{
    const std::u32string code1 = UR"(
x = 1;
switch (x) {
case 1:
    x = 11;
    break;
case 2:
    x = 2;
case 3:
    x = 3;
}
)";
    EScript engine;
    engine.setShowDisassembleListing(false);
    engine.setShowTCode(false);
    engine.eval(code1);
    auto mainTable = engine.unit()->block()->symbolTable();
    auto x = mainTable->find(U"x");
    auto record = engine.getObjectRecord(x);
    assert(record->type == SymbolType::Integer);
    assert(Compare::equals_int64(11, record->data));
}

void SwitchCase_Test::test_switch3CaseFirstTrue123FallThrough()
{
    const std::u32string code1 = UR"(
x = 1;
switch (x) {
case 1:
    x = 11;
case 2:
    x = 2;
case 3:
    x = 3;
}
)";
    EScript engine;
    engine.setShowDisassembleListing(false);
    engine.setShowTCode(false);
    engine.eval(code1);
    auto mainTable = engine.unit()->block()->symbolTable();
    auto x = mainTable->find(U"x");
    auto record = engine.getObjectRecord(x);
    assert(record->type == SymbolType::Integer);
    assert(Compare::equals_int64(3, record->data));
}

void SwitchCase_Test::test_switchAloneDefault()
{
    const std::u32string code1 = UR"(
x = 1;
switch (x) {
default:
    x = 3;
    break;
}
)";
    EScript engine;
    engine.setShowDisassembleListing(false);
    engine.setShowTCode(false);
    engine.eval(code1);
    auto mainTable = engine.unit()->block()->symbolTable();
    auto x = mainTable->find(U"x");
    auto record = engine.getObjectRecord(x);
    assert(record->type == SymbolType::Integer);
    assert(Compare::equals_int64(3, record->data));
}

void SwitchCase_Test::test_switchAloneDefaultFallThrough()
{
    const std::u32string code1 = UR"(
x = 1;
switch (x) {
default:
    x = 3;
}
)";
    EScript engine;
    engine.setShowDisassembleListing(false);
    engine.setShowTCode(false);
    engine.eval(code1);
    auto mainTable = engine.unit()->block()->symbolTable();
    auto x = mainTable->find(U"x");
    auto record = engine.getObjectRecord(x);
    assert(record->type == SymbolType::Integer);
    assert(Compare::equals_int64(3, record->data));
}

void SwitchCase_Test::test_switchTrueAndDefault()
{
    const std::u32string code1 = UR"(
x = 1;
switch (x) {
case 1:
    x = 2;
    break;
default:
    x = 3;
    break;
}
)";
    EScript engine;
    engine.setShowDisassembleListing(false);
    engine.setShowTCode(false);
    engine.eval(code1);
    auto mainTable = engine.unit()->block()->symbolTable();
    auto x = mainTable->find(U"x");
    auto record = engine.getObjectRecord(x);
    assert(record->type == SymbolType::Integer);
    assert(Compare::equals_int64(2, record->data));
}

void SwitchCase_Test::test_switchTrueAndDefaultDefaultFallThrough()
{
    const std::u32string code1 = UR"(
x = 1;
switch (x) {
case 1:
    x = 2;
    break;
default:
    x = 3;
}
)";
    EScript engine;
    engine.setShowDisassembleListing(false);
    engine.setShowTCode(false);
    engine.eval(code1);
    auto mainTable = engine.unit()->block()->symbolTable();
    auto x = mainTable->find(U"x");
    auto record = engine.getObjectRecord(x);
    assert(record->type == SymbolType::Integer);
    assert(Compare::equals_int64(2, record->data));
}

void SwitchCase_Test::test_switchTrueAndDefaultCaseFallThrough()
{
    const std::u32string code1 = UR"(
x = 1;
switch (x) {
case 1:
    x = 2;
default:
    x = 3;
    break;
}
)";
    EScript engine;
    engine.setShowDisassembleListing(false);
    engine.setShowTCode(false);
    engine.eval(code1);
    auto mainTable = engine.unit()->block()->symbolTable();
    auto x = mainTable->find(U"x");
    auto record = engine.getObjectRecord(x);
    assert(record->type == SymbolType::Integer);
    assert(Compare::equals_int64(3, record->data));
}

void SwitchCase_Test::test_switchTrueAndDefaultBothFallThrough()
{
    const std::u32string code1 = UR"(
x = 1;
switch (x) {
case 1:
    x = 2;
default:
    x = 3;
}
)";
    EScript engine;
    engine.setShowDisassembleListing(false);
    engine.setShowTCode(false);
    engine.eval(code1);
    auto mainTable = engine.unit()->block()->symbolTable();
    auto x = mainTable->find(U"x");
    auto record = engine.getObjectRecord(x);
    assert(record->type == SymbolType::Integer);
    assert(Compare::equals_int64(3, record->data));
}

void SwitchCase_Test::test_switchFalseAndDefault()
{
    const std::u32string code1 = UR"(
x = 1;
switch (x) {
case 5:
    x = 2;
    break;
default:
    x = 3;
    break;
}
)";
    EScript engine;
    engine.setShowDisassembleListing(false);
    engine.setShowTCode(false);
    engine.eval(code1);
    auto mainTable = engine.unit()->block()->symbolTable();
    auto x = mainTable->find(U"x");
    auto record = engine.getObjectRecord(x);
    assert(record->type == SymbolType::Integer);
    assert(Compare::equals_int64(3, record->data));
}

void SwitchCase_Test::test_switchFalseAndDefaultCaseFallThrough()
{
    const std::u32string code1 = UR"(
x = 1;
switch (x) {
case 5:
    x = 2;
default:
    x = 3;
    break;
}
)";
    EScript engine;
    engine.setShowDisassembleListing(false);
    engine.setShowTCode(false);
    engine.eval(code1);
    auto mainTable = engine.unit()->block()->symbolTable();
    auto x = mainTable->find(U"x");
    auto record = engine.getObjectRecord(x);
    assert(record->type == SymbolType::Integer);
    assert(Compare::equals_int64(3, record->data));
}

void SwitchCase_Test::test_switchFalseAndDefaultDefaultFallThrough()
{
    const std::u32string code1 = UR"(
x = 1;
switch (x) {
case 5:
    x = 2;
    break;
default:
    x = 3;
}
)";
    EScript engine;
    engine.setShowDisassembleListing(false);
    engine.setShowTCode(false);
    engine.eval(code1);
    auto mainTable = engine.unit()->block()->symbolTable();
    auto x = mainTable->find(U"x");
    auto record = engine.getObjectRecord(x);
    assert(record->type == SymbolType::Integer);
    assert(Compare::equals_int64(3, record->data));
}

void SwitchCase_Test::test_switchFalseAndDefaultBothFallThrough()
{
    const std::u32string code1 = UR"(
x = 1;
switch (x) {
case 5:
    x = 2;
default:
    x = 3;
}
)";
    EScript engine;
    engine.setShowDisassembleListing(false);
    engine.setShowTCode(false);
    engine.eval(code1);
    auto mainTable = engine.unit()->block()->symbolTable();
    auto x = mainTable->find(U"x");
    auto record = engine.getObjectRecord(x);
    assert(record->type == SymbolType::Integer);
    assert(Compare::equals_int64(3, record->data));
}

void SwitchCase_Test::test_switchDefaultAndTrue()
{
    const std::u32string code1 = UR"(
x = 1;
switch (x) {
default:
    x = 3;
    break;
case 1:
    x = 2;
    break;
}
)";
    EScript engine;
    engine.setShowDisassembleListing(false);
    engine.setShowTCode(false);
    engine.eval(code1);
    auto mainTable = engine.unit()->block()->symbolTable();
    auto x = mainTable->find(U"x");
    auto record = engine.getObjectRecord(x);
    assert(record->type == SymbolType::Integer);
    assert(Compare::equals_int64(2, record->data));
}

void SwitchCase_Test::test_switchDefaultAndTrueDefaultFallThrough()
{
    const std::u32string code1 = UR"(
x = 1;
switch (x) {
default:
    x = 3;
case 1:
    x = 2;
    break;
}
)";
    EScript engine;
    engine.setShowDisassembleListing(false);
    engine.setShowTCode(false);
    engine.eval(code1);
    auto mainTable = engine.unit()->block()->symbolTable();
    auto x = mainTable->find(U"x");
    auto record = engine.getObjectRecord(x);
    assert(record->type == SymbolType::Integer);
    assert(Compare::equals_int64(2, record->data));
}

void SwitchCase_Test::test_switchDefaultAndTrueCaseFallThrough()
{
    const std::u32string code1 = UR"(
x = 1;
switch (x) {
default:
    x = 3;
    break;
case 1:
    x = 2;
}
)";
    EScript engine;
    engine.setShowDisassembleListing(false);
    engine.setShowTCode(false);
    engine.eval(code1);
    auto mainTable = engine.unit()->block()->symbolTable();
    auto x = mainTable->find(U"x");
    auto record = engine.getObjectRecord(x);
    assert(record->type == SymbolType::Integer);
    assert(Compare::equals_int64(2, record->data));
}

void SwitchCase_Test::test_switchDefaultAndFalse()
{
    const std::u32string code1 = UR"(
x = 1;
switch (x) {
default:
    x = 3;
    break;
case 0:
    x = 2;
    break;
}
)";
    EScript engine;
    engine.setShowDisassembleListing(false);
    engine.setShowTCode(false);
    engine.eval(code1);
    auto mainTable = engine.unit()->block()->symbolTable();
    auto x = mainTable->find(U"x");
    auto record = engine.getObjectRecord(x);
    assert(record->type == SymbolType::Integer);
    assert(Compare::equals_int64(3, record->data));
}

void SwitchCase_Test::test_switchDefaultAndFalseCaseFallThrough()
{
    const std::u32string code1 = UR"(
x = 1;
switch (x) {
default:
    x = 3;
    break;
case 0:
    x = 2;
}
)";
    EScript engine;
    engine.setShowDisassembleListing(false);
    engine.setShowTCode(false);
    engine.eval(code1);
    auto mainTable = engine.unit()->block()->symbolTable();
    auto x = mainTable->find(U"x");
    auto record = engine.getObjectRecord(x);
    assert(record->type == SymbolType::Integer);
    assert(Compare::equals_int64(3, record->data));
}

void SwitchCase_Test::test_switchDefaultAndFalseDefaultFallThrough()
{
    const std::u32string code1 = UR"(
x = 1;
switch (x) {
default:
    x = 3;
case 0:
    x = 2;
    break;
}
)";
    EScript engine;
    engine.setShowDisassembleListing(false);
    engine.setShowTCode(false);
    engine.eval(code1);
    auto mainTable = engine.unit()->block()->symbolTable();
    auto x = mainTable->find(U"x");
    auto record = engine.getObjectRecord(x);
    assert(record->type == SymbolType::Integer);
    assert(Compare::equals_int64(2, record->data));
}

void SwitchCase_Test::test_switchDefaultAndFalseBothFallThrough()
{
    const std::u32string code1 = UR"(
x = 1;
switch (x) {
default:
    x = 3;
case 0:
    x = 2;
}
)";
    EScript engine;
    engine.setShowDisassembleListing(false);
    engine.setShowTCode(false);
    engine.eval(code1);
    auto mainTable = engine.unit()->block()->symbolTable();
    auto x = mainTable->find(U"x");
    auto record = engine.getObjectRecord(x);
    assert(record->type == SymbolType::Integer);
    assert(Compare::equals_int64(2, record->data));
}

void SwitchCase_Test::test_switchCaseFalseDefaultCaseTrue()
{
    const std::u32string code1 = UR"(
x = 1;
switch (x) {
case 0:
    x = 7;
    break;
default:
    x = 3;
    break;
case 1:
    x = 6;
    break;
}
)";
    EScript engine;
    engine.setShowDisassembleListing(false);
    engine.setShowTCode(false);
    engine.eval(code1);
    auto mainTable = engine.unit()->block()->symbolTable();
    auto x = mainTable->find(U"x");
    auto record = engine.getObjectRecord(x);
    assert(record->type == SymbolType::Integer);
    assert(Compare::equals_int64(6, record->data));
}

void SwitchCase_Test::test_switchCaseFalseDefaultCaseTrueDefaultFallThrough()
{
    const std::u32string code1 = UR"(
x = 1;
switch (x) {
case 0:
    x = 7;
    break;
default:
    x = 3;
case 1:
    x = 6;
    break;
}
)";
    EScript engine;
    engine.setShowDisassembleListing(false);
    engine.setShowTCode(false);
    engine.eval(code1);
    auto mainTable = engine.unit()->block()->symbolTable();
    auto x = mainTable->find(U"x");
    auto record = engine.getObjectRecord(x);
    assert(record->type == SymbolType::Integer);
    assert(Compare::equals_int64(6, record->data));
}

void SwitchCase_Test::test_mixedCases()
{
    const std::u32string code1 = UR"(
x = true;
y = 'a';
switch (x) {
    default:
        x = 15;
        y = 's';
        break;
    case false: {
        x = 1;
    }
        break;
    case '1': {
        x = '2';
        break;
    }
    case '2': {
        x = '3';
        break;
    }
    {
        y = '7';
        break;
    }
}
)";
    EScript engine;
    engine.eval(code1);
    auto mainTable = engine.unit()->block()->symbolTable();
    auto x = mainTable->find(U"x");
    auto record = engine.getObjectRecord(x);
    assert(record->type == SymbolType::Integer);
    assert(Compare::equals_int64(15, record->data));
}

void SwitchCase_Test::test_expressionsInCases()
{
    const std::u32string code1 = UR"(
x = 'hello, world';
y = 6;
a = [4, 11, 3, ', world'];
switch (x) {
    case a[0] + y:
        x = 1;
        break;
    default:
        x = 15;
        break;
    case 'hello' + a[3]: { // x == 'hello, world'
        x = 2;
        break;
    }
}
)";
    EScript engine;
    engine.eval(code1);
    auto mainTable = engine.unit()->block()->symbolTable();
    auto x = mainTable->find(U"x");
    auto record = engine.getObjectRecord(x);
    assert(record->type == SymbolType::Integer);
    assert(Compare::equals_int64(2, record->data));
}
