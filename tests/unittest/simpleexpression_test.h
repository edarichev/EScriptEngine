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
};

#endif // SIMPLEEXPRESSION_TEST_H
