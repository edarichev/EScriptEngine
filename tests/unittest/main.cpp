#include "stdafx.h"
#include "pvalue_test.h"
#include "lexer_test.h"
#include "simpleexpression_test.h"
#include "codeunit_test.h"
#include "codegen_test.h"
#include "opcode_test.h"
#include "assembler_test.h"
#include "automation_test.h"
#include "boolean_test.h"
#include "ifelsestatement_test.h"
#include "whilestatement_test.h"
#include "forstatement_test.h"
#include "dowhilestatement_test.h"
#include "functiondeclstatement_test.h"
#include "array_test.h"
#include "string_test.h"


using namespace std;

int main()
{
    PValue_Test().run();
    Lexer_Test().run();
    SimpleExpression_Test().run();
    CodeUnit_Test().run();
    CodeGen_Test().run();
    OpCode_Test().run();
    Assembler_Test().run();
    Automation_Test().run();
    Boolean_Test().run();
    IfElseStatement_Test().run();
    WhileStatement_Test().run();
    ForStatement_Test().run();
    FunctionDeclStatement_Test().run();
    //Array_Test().run();
    String_Test().run();
    cout << "Tests OK" << endl;
    return 0;
}
