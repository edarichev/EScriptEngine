#ifndef CONVERSIONS_TEST_H
#define CONVERSIONS_TEST_H


class Conversions_Test
{
public:
    void run();
private:
    void initTestCase();
    void cleanupTestCase();
    void test_convertToRealPlus();
    void test_convertToRealMinus();
    void test_convertToRealMultiply();
    void test_convertToRealDivide();
    void test_convertToString();
};

#endif // CONVERSIONS_TEST_H
