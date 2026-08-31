%{
#include "y.tab.h"
%}

%%
a       { return A; }
b       { return B; }
\n      { return NL; }
[ \t]   ; 
.       { return yytext[0]; }
%%

int yywrap() {
    return 1;
}
