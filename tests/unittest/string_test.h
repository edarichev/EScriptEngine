#ifndef STRING_TEST_H
#define STRING_TEST_H


class String_Test
{
public:
    void run();
private:
    void initTestCase();
    void cleanupTestCase();
    void test_stringLiteral();
    void test_stringPlus();
    void test_stringLength();
    void test_stringAt();
    void test_stringSubstring();
};

#endif // STRING_TEST_H
