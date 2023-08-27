#ifndef NULL_TEST_H
#define NULL_TEST_H


class Null_Test
{
public:
    void run();
private:
    void initTestCase();
    void cleanupTestCase();
    void test_compareWithNull();
    void test_assignToNull();
    void test_arrayFillEmptyItemsWithNull();
};

#endif // NULL_TEST_H
