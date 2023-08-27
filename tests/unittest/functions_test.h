#ifndef FUNCTIONS_TEST_H
#define FUNCTIONS_TEST_H


class Functions_Test
{
public:
    void run();
private:
    void initTestCase();
    void cleanupTestCase();
    void test_byReference();
    void test_recursionFactorial();
    void test_funcAlias();
    void test_funcAliasAssign();
    void test_functionAsParameter();
    void test_unnamedFuncAssign();
    void test_nestedFunction();
    void test_nestedFunctionVariable();
    void test_functionReturnsFunction();
    void test_function2ReturnsFunction();
    void test_functionFactory();
    void test_arrayOfFunctions();
};

#endif // FUNCTIONS_TEST_H
