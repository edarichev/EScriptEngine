#ifndef DOTOPERATION_TEST_H
#define DOTOPERATION_TEST_H


class DotOperation_Test
{
public:
    void run();
private:
    void initTestCase();
    void cleanupTestCase();
    void test_dotParenthExpression();
    void test_dotArrayDeclExpression();
    void test_dotArrayItemRefExpression();
    void test_dotStringLiteralExpression();
    void test_dotFunctionCallExpression();
};

#endif // DOTOPERATION_TEST_H
