%{
#include <stdio.h>
%}

%%

[A-Z][a-zA-Z ]*                { printf("Valid Name: %s\n", yytext); }

[6-9][0-9]{9}                  { printf("Valid Phone Number: %s\n", yytext); }

[0-9]{6}                       { printf("Valid PIN Code: %s\n", yytext); }

[a-zA-Z0-9._%+-]+@[a-zA-Z0-9.-]+\.[a-zA-Z]{2,}
                                { printf("Valid Email: %s\n", yytext); }

[a-zA-Z0-9./?&_=:-]+  { printf("Valid URL: %s\n", yytext); }

[\n]                            { return 0 ; }

.                              { printf("Invalid Input: %s\n", yytext); }
%%

int yywrap()
{
    return 1;
}

int main()
{
    printf("Enter Name, Phone, PIN, Email, and URL (one per line):\n");
    yylex();
    return 0;
}
