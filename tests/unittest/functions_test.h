#ifndef FUNCTIONS_TEST_H
#define FUNCTIONS_TEST_H


class Functions_Test
{
public:
    void run();
private:
    void initTestCase();
    void cleanupTestCase();
    void test_byReference();
    void test_recursionFactorial();
};

#endif // FUNCTIONS_TEST_H