#ifndef CODEUNIT_TEST_H
#define CODEUNIT_TEST_H

class CodeUnit_Test
{
public:
    void run();
private:
    void initTestCase();
    void cleanupTestCase();
    void test_objectsWork();
};

#endif // CODEUNIT_TEST_H
