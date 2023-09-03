#ifndef BOOLEAN_TEST_H
#define BOOLEAN_TEST_H


class Boolean_Test
{
public:
    void run();
private:
    void initTestCase();
    void cleanupTestCase();
    void test_trueFalse();
    void test_less();
    void test_lessOrEqual();
    void test_greater();
    void test_greaterOrEqual();
    void test_equal();
    void test_arrayItemsLess();
    void test_compareToNaN();
    void test_complexExprOr();
    void test_complexExprAnd();
    void test_complexExprCalc();
};

#endif // BOOLEAN_TEST_H
