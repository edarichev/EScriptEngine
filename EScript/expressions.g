/*
expressions.g
Тест выражений
*/

%token Identifier
%token IntegerNumber
%token Assign
%token Equal

%%

StatementList : Statement
              | Statement ';' StatementList
              |
              ;

Statement : AssignStatement
          ;

AssignStatement : Variable Assign Expression
                ;

Variable : Identifier
         ;

Expression : SimpleExpression
           | AssignStatement
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
       ;


