#ifndef BIT_TEST_H
#define BIT_TEST_H


class Bit_Test
{
public:
    void run();
private:
    void initTestCase();
    void cleanupTestCase();
    void test_bitAnd();
    void test_bitOr();
    void test_bitXor();
    void test_unaryBitNot();
    void test_complexBitOr();
    void test_complexBitAnd();
    void test_complexBitXor();
};

#endif // BIT_TEST_H
