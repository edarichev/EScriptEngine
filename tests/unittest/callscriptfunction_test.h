#ifndef CALLSCRIPTFUNCTION_TEST_H
#define CALLSCRIPTFUNCTION_TEST_H


class CallScriptFunction_Test
{
public:
    void run();
private:
    void initTestCase();
    void cleanupTestCase();
    void test_callFunction();
    void test_callFunctionArgString();
    void test_callFunctionArgArray();
};

#endif // CALLSCRIPTFUNCTION_TEST_H
