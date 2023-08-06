/*
expressions.g
Тест выражений
if-else:
https://www.epaperpress.com/lexandyacc/if.html
*/

%token Identifier
%token IntegerNumber
%token RealNumber
%token Assign
%token Equal
%nonassoc If
%nonassoc Else
%%

Program : StatementList
        ;

StatementList : Statement
              | Statement StatementList
              ;

Statement : AssignStatement
          | CompoundStatement
          | IfElseStatement
          ;

AssignStatement : AssignExpression ';'
                ;

CompoundStatement : '{' StatementList '}'
                  ;

IfElseStatement : If '(' Expression ')' Statement %prec If
                | If '(' Expression ')' Statement Else Statement
                ;

AssignExpression : Variable Assign Expression
                 ;

Variable : Identifier
         ;

Expression : SimpleExpression
           | AssignExpression
           ;

SimpleExpression : Term
                 | SimpleExpression '+' Term
                 | SimpleExpression '-' Term
                 ;

Term : Factor
     | Term '*' Factor
     | Term '/' Factor
     ;

Factor : Identifier
       | IntegerNumber
       | RealNumber
       | '-' Factor
       | '+' Factor
       ;

