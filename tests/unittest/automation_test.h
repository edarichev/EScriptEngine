#ifndef AUTOMATION_TEST_H
#define AUTOMATION_TEST_H


class Automation_Test
{
public:
    void run();
private:
    void initTestCase();
    void cleanupTestCase();
    void test_auto1();
    void test_autoPropSet();
    void test_autoAddAfterFirstRun();
};

#endif // AUTOMATION_TEST_H
