#ifndef INCDEC_TEST_H
#define INCDEC_TEST_H


class IncDec_Test
{
public:
    void run();
private:
    void initTestCase();
    void cleanupTestCase();
    void test_prefixInc();
    void test_postfixInc();
    void test_prefixDec();
    void test_postfixDec();
};

#endif // INCDEC_TEST_H
