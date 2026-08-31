%{
#include <stdio.h>
#include <stdlib.h>

int yylex(void);
void yyerror(const char *s);
%}

%token A B NL

%%
input:
    S NL { printf("Valid String\n"); exit(0); }
    ;

S:
    A S
  | B
  ;

%%

void yyerror(const char *s) {
    printf("Invalid String\n");
    exit(0);
}

int main() {
    printf("Enter the string: ");
    yyparse();
    return 0;
}
