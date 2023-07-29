#ifndef LEXER_TEST_H
#define LEXER_TEST_H

class Lexer_Test
{
public:
    void run();
private:
    void initTestCase();
    void cleanupTestCase();
    void test_allTokens();
    void test_realNumbers();
    void test_intNumbers();
    void test_identifiers();
    void test_quotedString();
};

#endif // LEXER_TEST_H
