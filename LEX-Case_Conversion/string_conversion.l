%{
#include <stdio.h>
#include <ctype.h>
%}

%%

"abc" {
    for(int i = 0; yytext[i] != '\0'; i++) {
        putchar(toupper(yytext[i]));
    }
}

.|\n {
    /* Print all other characters as they are */
    ECHO; 
}

%%

int main() {
    yylex();
    return 0;
}

int yywrap() {
    return 1;
}
