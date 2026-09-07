%{
#include <stdio.h>
#include <stdlib.h>

int yylex(void);
void yyerror(const char *s);
%}

%token NUMBER

%left '+' '-'
%left '*' '/' '%'

%%

input:
      E '\n'
      {
          printf("Result = %d\n", $1);
          return 0;
      }
      ;

E:
      E '+' E     { $$ = $1 + $3; }
    | E '-' E     { $$ = $1 - $3; }
    | E '*' E     { $$ = $1 * $3; }
    | E '/' E     { $$ = $1 / $3; }
    | E '%' E     { $$ = $1 % $3; }
    | '(' E ')'   { $$ = $2; }
    | NUMBER      { $$ = $1; }
    ;

%%

void yyerror(const char *s)
{
    printf("Invalid expression\n");
}

int main()
{
    printf("Enter arithmetic expression: ");
    yyparse();
    return 0;
}
