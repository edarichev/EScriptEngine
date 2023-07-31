#include "stdafx.h"
#include "lexer_test.h"
#include "simpleexpression_test.h"
#include "codeunit_test.h"
#include "codegen_test.h"
#include "opcode_test.h"

using namespace std;

int main()
{
    Lexer_Test().run();
    SimpleExpression_Test().run();
    CodeUnit_Test().run();
    CodeGen_Test().run();
    OpCode_Test().run();
    cout << "Tests OK" << endl;
    return 0;
}
