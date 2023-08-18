#ifndef ARRAY_TEST_H
#define ARRAY_TEST_H


class Array_Test
{
public:
    void run();
private:
    void initTestCase();
    void cleanupTestCase();
    void test_arrayAccess();
    void test_arrayItemSet();
    void test_arrayItemGet();
    void test_arrayItemSwap();
    void test_arrayItemAdd();
    void test_arrayIfElse();
    void test_arrayStringKeys();
    void test_arrayStringValues();
    void test_arrayRealKeys();
    void test_arrayRealValues();
};

#endif // ARRAY_TEST_H
