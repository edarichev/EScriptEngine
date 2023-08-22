#ifndef NUMBER_TEST_H
#define NUMBER_TEST_H


class Number_Test
{
public:
    void run();
private:
    void initTestCase();
    void cleanupTestCase();
    void test_parseInt();
    void test_parseFloat();
    void test_toString10();
    void test_toString2();
    void test_toString8();
    void test_toString16();
    void test_realToString();
    void test_realParenthExprToString();
    void test_intParenthExprToString();
};

#endif // NUMBER_TEST_H
