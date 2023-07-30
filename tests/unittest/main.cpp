#include "stdafx.h"
#include "lexer_test.h"
#include "simpleexpression_test.h"
#include "codeunit_test.h"

using namespace std;

int main()
{
    Lexer_Test().run();
    SimpleExpression_Test().run();
    CodeUnit_Test().run();
    cout << "Tests OK" << endl;
    return 0;
}
