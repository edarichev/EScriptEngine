#ifndef STRING_TEST_H
#define STRING_TEST_H


class String_Test
{
public:
    void run();
private:
    void initTestCase();
    void cleanupTestCase();
    void test_literal();
    void test_plus();
    void test_length();
    void test_at();
    void test_charCodeAt();
    void test_charAt();
    void test_endsWith();
    void test_includes();
    void test_indexOf();
    void test_lastIndexOf();
    void test_padEnd();
    void test_padStart();
    void test_repeat();
    void test_replace();
    void test_replaceAll();
    void test_slice();
    void test_split();
    void test_startsWith();
    void test_substring();
    void test_toLowerCase();
    void test_toUpperCase();
    void test_trim();
    void test_trimEnd();
    void test_trimStart();
    void test_callMethodOfStringLiteral();
    void test_callMethodOfBQStringLiteral();
};

#endif // STRING_TEST_H
