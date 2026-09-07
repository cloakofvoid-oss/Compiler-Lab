%{
#include <stdio.h>
#include <stdlib.h>

int yylex(void);
void yyerror(const char *s);
%}

%token NUMBER TEXT NL

%left '+' '-'
%left '*' '/'

%%

input:
    expr NL
    {
        printf("Valid Arithmetic Expression\n");
        exit(0);
    }
    ;

expr:
      expr '+' expr
    | expr '-' expr
    | expr '*' expr
    | expr '/' expr
    | '(' expr ')'
    | NUMBER
    | TEXT
    ;

%%

void yyerror(const char *s)
{
    printf("Invalid Arithmetic Expression\n");
    exit(0);
}

int main()
{
    printf("Enter an arithmetic expression: ");
    yyparse();
    return 0;
}
