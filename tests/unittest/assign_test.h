#ifndef ASSIGN_TEST_H
#define ASSIGN_TEST_H


class Assign_Test
{
public:
    void run();
private:
    void initTestCase();
    void cleanupTestCase();
    void test_plusAssign();
    void test_minusAssign();
    void test_multiplyAssign();
    void test_divideAssign();
    void test_BitAndAssign();
    void test_BitOrAssign();
    void test_BitXorAssign();
    void test_LShiftAssign();
    void test_LRhiftAssign();
    void test_RShiftZeroAssign();
    void test_modAssign();
};

#endif // ASSIGN_TEST_H
