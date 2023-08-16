#ifndef CONSOLE_TEST_H
#define CONSOLE_TEST_H


class Console_Test
{
public:
    void run();
private:
    void initTestCase();
    void cleanupTestCase();
    void test_consoleLogSimpleTypes();
    void test_consoleLogVariables();
};

#endif // CONSOLE_TEST_H
