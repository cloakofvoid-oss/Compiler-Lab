%{
#include <stdio.h>
%}

%%
.*\n    { printf("%s", yytext); }
.+      { printf("%s", yytext); }
%%

int yywrap()
{
    return 1;
}

int main(int argc, char *argv[])
{
    FILE *fp;

    if (argc != 2)
    {
        printf("Usage: %s <filename>\n", argv[0]);
        return 1;
    }

    fp = fopen(argv[1], "r");

    if (fp == NULL)
    {
        printf("Cannot open file.\n");
        return 1;
    }

    yyin = fp;
    yylex();

    fclose(fp);
    return 0;
}
