#ifndef ALGO_TEST_H
#define ALGO_TEST_H


class Algo_Test
{
public:
    void run();
private:
    void initTestCase();
    void cleanupTestCase();
    void test_sortInt();
    void test_sortReal();
    void test_bsearchRecursive();
    void test_qsort();
};

#endif // ALGO_TEST_H
