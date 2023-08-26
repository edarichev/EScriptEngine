#ifndef ARRAY_TEST_H
#define ARRAY_TEST_H


class Array_Test
{
public:
    void run();
private:
    void initTestCase();
    void cleanupTestCase();
    void test_arrayAccess();
    void test_arrayItemSet();
    void test_arrayItemGet();
    void test_arrayItemSwap();
    void test_arrayItemAdd();
    void test_arrayIfElse();
    void test_arrayWith1String();
    void test_arrayStringKeys();
    void test_arrayStringValues();
    void test_arrayRealKeys();
    void test_arrayRealValues();
    void test_callAccessOfFuncExpression();
    void test_callAccessOfParenthExpression();
    void test_callAccessOfArrayDeclExpression();
    void test_array2DGetItem();
    void test_array2DDeclGetItem();
    void test_array2DSetItem();
    void test_array3DGetItem();
    void test_array3DDeclGetItem();
    void test_array3DSetItem();
    void test_arrayFuncRetArrayGetItem();
    void test_arrayLength();
    void test_arrayLengthWithProps();
    void test_arrayPush();
    void test_arrayPushMultiple();
    void test_arrayPop();
    void test_arrayShift();
    void test_arraySpliceRemoveAll();
    void test_arraySpliceRemoveAllFrom();
    void test_arraySpliceRemoveAllFromEnd();
    void test_arraySpliceRemoveNFrom0();
    void test_arraySpliceRemoveNFromPos();
    void test_arraySpliceRemoveNFromEnd();
    void test_arraySpliceInsertAt0();
    void test_arraySpliceInsertAtN();
    void test_arraySpliceInsertAtEnd();
    void test_arraySpliceRemoveNInsertFrom0();
    void test_arraySpliceRemoveNInsertFromPos();
    void test_arraySpliceRemoveNInsertFromEnd();
    void test_arrayReverse();
    void test_arrayFill();
    void test_arraySort();
    void test_arrayUnshift();
    void test_arraySlice();
    void test_arrayJoin();
    void test_arrayFilter();
    void test_arrayFirstIndex();
    void test_arrayLastIndex();
    void test_arrayIncludes();
    void test_appendWithSplice();
    void test_arrayForEach();
};

#endif // ARRAY_TEST_H
