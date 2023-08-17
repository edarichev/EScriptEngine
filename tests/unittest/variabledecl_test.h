#ifndef VARIABLEDECL_TEST_H
#define VARIABLEDECL_TEST_H


class VariableDecl_Test
{
public:
    void run();
private:
    void initTestCase();
    void cleanupTestCase();
    void test_decl();
    void test_declComma();
    void test_declNested();
    void test_declNestedWithSameName();
};

#endif // VARIABLEDECL_TEST_H
