#ifndef SHIFT_TEST_H
#define SHIFT_TEST_H


class Shift_Test
{
public:
    void run();
private:
    void initTestCase();
    void cleanupTestCase();
    void test_lshift();
    void test_rshift();
    void test_rshiftz();
};

#endif // SHIFT_TEST_H
