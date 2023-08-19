/*
expressions.g
Тест выражений
if-else:
https://www.epaperpress.com/lexandyacc/if.html
*/

%token Identifier
%token IntegerNumber Var
%token RealNumber QuotedString UnaryMinus UnaryPlus
%token Assign
%token Equal
%token While Do For Break Continue Return
%token Function
%token LShift RelOp EqualityOp And Or
%token NCO // ?? - Nullish coalescing operator
%token PlusPlus MinusMinus

%nonassoc If
%nonassoc Else



%start Program
%%

Program : StatementList
        ;

StatementList : Statement
              | Statement StatementList
              ;

Statement : CompoundStatement
          | IfElseStatement
          | WhileStatement
          | ForStatement
          | DoWhileStatement
          | BreakStatement
          | ContinueStatement
          | FunctionDeclStatement
          | AnyStatement
          | ReturnStatement ';'
          | VariableDeclBlock ';'
          ;

VariableDeclBlock : Var VariableDeclStatementList
                  ;

VariableDeclStatementList : VariableDeclStatement
                          | VariableDeclStatement ',' VariableDeclStatementList
                          ;

VariableDeclStatement : Identifier
                      | Identifier Assign Expression
                      ;

ReturnStatement : Return
                | Return Expression
                ;

AnyStatement : Expression ';'
             ;

CompoundStatement : '{' StatementList '}'
                  | '{' '}'
                  ;

IfElseStatement : If '(' Expression ')' Statement %prec If
                | If '(' Expression ')' Statement Else Statement
                ;

WhileStatement : While '(' Expression ')' OptionalStatement
               ;

ForStatement : For '(' OptionalExpressionList ';' Expression ';' OptionalExpressionList ')' OptionalStatement
             ;

OptionalStatement : Statement
                  | ';' // требуем для закрытия цикла
                  ;

OptionalExpressionList : ExpressionList
                       |
                       ;

ExpressionList : Expression
               | Expression ',' ExpressionList
               ;

DoWhileStatement : Do CompoundStatement While '(' Expression ')'
                 ;

BreakStatement : Break ';'
               ;

ContinueStatement : Continue ';'
                  ;

FunctionDeclExpression : Function Identifier '(' OptionalParameterDeclList ')' CompoundStatement
                       ;

FunctionDeclStatement : FunctionDeclExpression
                      ;

OptionalParameterDeclList : ParameterDeclList
                          |
                          ;

ParameterDeclList : Identifier
                  | Identifier ',' ParameterDeclList
                  ;

AssignExpression : Variable Assign Expression
                 | Identifier '[' Expression ']' Assign Expression
                 ;

Variable : Identifier
         ;

Expression : LogicalOrNCOExpression
           | AssignExpression
           | FunctionDeclExpression
           | ArrayDeclExpression
           ;

ArrayDeclExpression : '[' ArrayDeclItems ']'
                    ;

ArrayDeclItems : Expression
               | Expression ',' ArrayDeclItems
               ;


// Nullish coalescing operator
LogicalOrNCOExpression : LogicalOrOrAndExpression
                       | NCO LogicalOrOrAndExpression
                       ;

LogicalOrOrAndExpression : LogicalAndOrBitORExpression
                         | Or LogicalAndOrBitORExpression
                         ;

LogicalAndOrBitORExpression : BitwiseOROrXORExpression
                            | And BitwiseOROrXORExpression
                            ;

BitwiseOROrXORExpression : BitwiseXOROrAndExpression
                         | '|' BitwiseXOROrAndExpression
                         ;

BitwiseXOROrAndExpression : BitwiseAndOrEqualityExpression
                          | '^' BitwiseAndOrEqualityExpression
                          ;

BitwiseAndOrEqualityExpression : RelationOrEqualityExpression
                               | '&' RelationOrEqualityExpression
                               ;

RelationOrEqualityExpression : ShiftOrRelationExpression
                             | EqualityOp
                             ;

ShiftOrRelationExpression : SimpleOrShiftExpression
                          | RelOp SimpleOrShiftExpression
                          ;

SimpleOrShiftExpression : SimpleExpression
                        | LShift SimpleExpression
                        ;

SimpleExpression : Term
                 | SimpleExpression '+' Term
                 | SimpleExpression '-' Term
                 ;

Term : Factor
     | Term '*' Factor
     | Term '/' Factor
     ;

Factor : PostfixOperation
       | UnaryMinus PostfixOperation
       | UnaryPlus PostfixOperation
       | PlusPlus PostfixOperation
       | MinusMinus PostfixOperation
       ;

PostfixOperation : CallOrAccess
                 | CallOrAccess PlusPlus
                 | CallOrAccess MinusMinus
                 ;

CallOrAccess : Grouping
             | FunctionCallExpression
             | ArrayItemRefExpression
             | DotOperation
             ;

Grouping : Literals
         | '(' Expression ')'
         ;

Literals : Identifier
         | IntegerNumber
         | RealNumber
         | QuotedString
         ;

LeftDotPart : Identifier
            | QuotedString
            ;

// правая часть операции обращения к функции-члену
DotOperation : LeftDotPart '.' Identifier '(' ')'
             | LeftDotPart '.' Identifier '(' ArgumentList ')'
             | LeftDotPart '.' Identifier
             ;

ArrayItemRefExpression : Identifier '[' Expression ']'
                       ;

FunctionCallExpression : Identifier '(' ArgumentList ')'
                       | Identifier '(' ')'
                       ;

ArgumentList : Expression
             | Expression ',' ArgumentList
             ;
