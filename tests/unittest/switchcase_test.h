#ifndef SWITCHCASE_TEST_H
#define SWITCHCASE_TEST_H


class SwitchCase_Test
{
public:
    void run();
private:
    void initTestCase();
    void cleanupTestCase();
    void test_emptySwitch();
    void test_switchOneCase();
    void test_switchOneCaseFallThrough();
    void test_switchTwoCaseFirstTrue();
    void test_switchTwoCaseFirstTrueFallThrough();
    void test_switchTwoCaseFirstTrue2FallThrough();
    void test_switchTwoCaseSecondTrue();
    void test_switchTwoCaseSecondTrueFallThrough();
    void test_switchTwoCaseSecondTrue2FallThrough();
    void test_switchTwoCaseNoTrue();
    void test_switchTwoCaseNoTrue1FallThrough();
    void test_switchTwoCaseNoTrue2FallThrough();
    void test_switchTwoCaseNoTrueBothFallThrough();
    void test_switch3CaseMiddleTrue();
    void test_switch3CaseMiddleTrue1FallThrough();
    void test_switch3CaseMiddleTrue2FallThrough();
    void test_switch3CaseMiddleTrue3FallThrough();
    void test_switch3CaseMiddleTrue12FallThrough();
    void test_switch3CaseMiddleTrue13FallThrough();
    void test_switch3CaseMiddleTrue23FallThrough();
    void test_switch3CaseFirstTrue();
    void test_switch3CaseFirstTrue1FallThrough();
    void test_switch3CaseFirstTrue2FallThrough();
    void test_switch3CaseFirstTrue3FallThrough();
    void test_switch3CaseFirstTrue12FallThrough();
    void test_switch3CaseFirstTrue13FallThrough();
    void test_switch3CaseFirstTrue23FallThrough();
    void test_switch3CaseFirstTrue123FallThrough();
    void test_switchAloneDefault();
    void test_switchAloneDefaultFallThrough();
    void test_switchTrueAndDefault();
    void test_switchTrueAndDefaultDefaultFallThrough();
    void test_switchTrueAndDefaultCaseFallThrough();
    void test_switchTrueAndDefaultBothFallThrough();
    void test_switchFalseAndDefault();
    void test_switchFalseAndDefaultDefaultFallThrough();
    void test_switchFalseAndDefaultCaseFallThrough();
    void test_switchFalseAndDefaultBothFallThrough();
    void test_switchDefaultAndTrue();
    void test_switchDefaultAndTrueDefaultFallThrough();
    void test_switchDefaultAndTrueCaseFallThrough();
    void test_switchDefaultAndFalse();
    void test_switchDefaultAndFalseCaseFallThrough();
    void test_switchDefaultAndFalseDefaultFallThrough();
    void test_switchDefaultAndFalseBothFallThrough();
    void test_switchCaseFalseDefaultCaseTrue();
    void test_switchCaseFalseDefaultCaseTrueDefaultFallThrough();
    void test_mixedCases();
    void test_switchInFunction();
    void test_expressionsInCases();
};

#endif // SWITCHCASE_TEST_H
