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
};

#endif // BOOLEAN_TEST_H