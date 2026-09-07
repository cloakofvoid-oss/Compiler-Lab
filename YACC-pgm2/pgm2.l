%{
#include "y.tab.h"
#include <stdlib.h>
%}

%%

[0-9]+                  { return NUMBER; }

[a-zA-Z_][a-zA-Z0-9_]*  { return TEXT; }

[+*/()-]                { return yytext[0]; }

\n                      { return NL; }

[ \t]+                  { /* ignore spaces and tabs */ }

.                       { return yytext[0]; }

%%

int yywrap()
{
    return 1;
}
