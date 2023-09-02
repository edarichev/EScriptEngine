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
#include "console_test.h"
#include "conversions_test.h"
#include "incdec_test.h"
#include "shift_test.h"
#include "bit_test.h"
#include "logical_test.h"
#include "assign_test.h"
#include "functions_test.h"
#include "variabledecl_test.h"
#include "algo_test.h"
#include "forreadme_test.h"
#include "math_test.h"
#include "switchcase_test.h"
#include "dotoperation_test.h"
#include "number_test.h"
#include "callscriptfunction_test.h"
#include "null_test.h"
#include "nco_test.h"
#include "ternaryconditionaloperator_test.h"
#include "date_test.h"
#include "newexpression_test.h"

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
    Boolean_Test().run();
    Array_Test().run();
    IfElseStatement_Test().run();
    SwitchCase_Test().run();
    WhileStatement_Test().run();
    ForStatement_Test().run();
    FunctionDeclStatement_Test().run();
    String_Test().run();
    Console_Test().run();
    Conversions_Test().run();
    IncDec_Test().run();
    Shift_Test().run();
    Bit_Test().run();
    Logical_Test().run();
    Assign_Test().run();
    Functions_Test().run();
    VariableDecl_Test().run();
    Algo_Test().run();
    ForReadme_Test().run();
    Automation_Test().run();
    Math_Test().run();
    DotOperation_Test().run();
    Number_Test().run();
    CallScriptFunction_Test().run();
    Null_Test().run();
    NCO_Test().run();
    TernaryConditionalOperator_Test().run();
    Date_Test().run();
    NewExpression_Test().run();
    cout << "Tests OK" << endl;
    return 0;
}
