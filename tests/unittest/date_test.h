#ifndef DATE_TEST_H
#define DATE_TEST_H


class Date_Test
{
public:
    void run();
private:
    void initTestCase();
    void cleanupTestCase();
    void test_parse();
    void test_toString();
    void test_toStringFormatted();
    void test_timeParts();
    void test_now();
    void test_utcNow();
    void test_create();
    void test_toLocale();
    void test_toUtc();
};

#endif // DATE_TEST_H
