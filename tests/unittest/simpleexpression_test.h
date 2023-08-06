#ifndef SIMPLEEXPRESSION_TEST_H
#define SIMPLEEXPRESSION_TEST_H


class SimpleExpression_Test
{
public:
    void run();
private:
    void initTestCase();
    void cleanupTestCase();
    void test_simpleAssign();
    void test_unaryPlusMinus();
    void test_parenth();
    void test_integerExpressions1();
    void test_realAssign1();
    void test_realAssignChangeTypeToReal();
    void test_realAssignChangeTypeToInt();
    void test_realAssignChangeTypeToRealVar();
    void test_realAssignChangeTypeToIntVar();
    void test_chTypeBinaryOp1();
    void test_chTypeBinaryOp2();
    void test_sequentialRun();
    void test_braceSimple();
};

#endif // SIMPLEEXPRESSION_TEST_H
