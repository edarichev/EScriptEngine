/*
expressions.g
Тест выражений
*/

%token Identifier
%token IntegerNumber
%token RealNumber
%token Assign
%token Equal

%%

Program : StatementList
        ;

StatementList : Statement
              | Statement StatementList
              ;

Statement : AssignStatement
          | CompoundStatement
          ;

CompoundStatement : '{' StatementList '}'
                  ;

AssignStatement : AssignExpression ';'
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

