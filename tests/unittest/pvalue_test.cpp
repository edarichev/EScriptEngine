#include "stdafx.h"
#include "pvalue_test.h"

using namespace std;
using namespace escript;

void PValue_Test::run()
{
    initTestCase();
    test_operatorLess();
    cleanupTestCase();
}

void PValue_Test::initTestCase()
{

}

void PValue_Test::cleanupTestCase()
{

}

void PValue_Test::test_operatorLess()
{
    assert(PValue(5) < PValue(6));
    assert(PValue(5.1) < PValue(6));
    assert(PValue(5.1) < PValue(6.1));
    assert(PValue(5) < PValue(6.3));
    assert(PValue(false) < PValue(true));
    assert(PValue(false) < PValue(6));
    assert(PValue(false) < PValue(6.7));
    assert(PValue(0) < PValue(true));
    assert(PValue(0.1) < PValue(true));
}
