#ifndef LOGICAL_TEST_H
#define LOGICAL_TEST_H


class Logical_Test
{
public:
    void run();
private:
    void initTestCase();
    void cleanupTestCase();
    void test_logAnd();
    void test_logOr();
    void test_logNot();
};

#endif // LOGICAL_TEST_H
