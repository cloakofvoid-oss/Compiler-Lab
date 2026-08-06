%{
/*#include <stdio.h>*/

int lines = 0;
int words = 0;
int chars = 0;
%}

%%
\n              { lines++; chars++; }
[ \t]+          { chars += yyleng; }
[A-Za-z0-9]+    { words++; chars += yyleng; }
.               { chars++; }
%%

int yywrap()
{
    return 1;
}

int main()
{
    printf("Enter text :\n\n");

    yylex();

    printf("\nLines      : %d\n", lines);
    printf("Words      : %d\n", words);
    printf("Characters : %d\n", chars);

    return 0;
}
