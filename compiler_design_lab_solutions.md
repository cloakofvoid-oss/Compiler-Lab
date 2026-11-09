# Compiler Design Lab -- Lex, Yacc and Parsing Programs

## Build environment

On Kali, Debian, Ubuntu or Parrot:

``` bash
sudo apt update
sudo apt install flex byacc gcc
```

`flex` supplies both `flex` and the compatibility command `lex`. `byacc`
supplies `yacc`.

Typical Lex build:

``` bash
lex file.l
gcc lex.yy.c -o output -ll
./output
```

Typical Yacc + Lex build:

``` bash
yacc -d file.y
lex file.l
gcc y.tab.c lex.yy.c -o output -ll
./output
```

For a Yacc file that contains its own `yylex()` function, compile only
`y.tab.c`.

------------------------------------------------------------------------

# Part A -- YACC programs

For each `.y` file below, save it using the shown filename. Unless
stated otherwise, run:

``` bash
yacc -d filename.y
gcc y.tab.c -o output
./output
```

## 1. Recognize `a^n b^n`, n \>= 1

**File: `anbn.y`**

``` yacc
%{
#include <stdio.h>
#include <stdlib.h>
int yylex(void);
void yyerror(const char *s) { printf("Invalid string\n"); }
%}
%%
S : A ;
A : 'a' A 'b'
  | 'a' 'b'
  ;
%%
int yylex(void) {
    int c = getchar();
    if (c == EOF || c == '\n') return 0;
    return c;
}
int main(void) {
    printf("Enter string: ");
    if (yyparse() == 0) printf("Valid string\n");
    return 0;
}
```

Accepted: `ab`, `aabb`, `aaabbb`. Rejected: `aab`, `abb`, empty input.

## 2. Recognize `a^n b^n c^n`, n \>= 0

**This cannot be implemented as a pure Yacc context-free grammar.** The
language `{a^n b^n c^n | n >= 0}` is not context-free, while Yacc
generates context-free parsers. Use a semantic counter/action or a
hand-written recognizer if the lab expects this language.

**File: `anbncn.y` (Yacc with semantic validation)**

``` yacc
%{
#include <stdio.h>
int na=0, nb=0, nc=0, phase=0, bad=0;
int yylex(void);
void yyerror(const char *s) { bad=1; }
%}
%%
input : /* empty */ | input ch ;
ch : 'a' { if (phase != 0) bad=1; na++; }
   | 'b' { if (phase > 1) bad=1; phase=1; nb++; }
   | 'c' { phase=2; nc++; }
   ;
%%
int yylex(void) {
    int c=getchar();
    if(c==EOF || c=='\n') return 0;
    return c;
}
int main(void) {
    printf("Enter string (a^n b^n c^n): ");
    yyparse();
    if(!bad && na==nb && nb==nc) printf("Valid\n");
    else printf("Invalid\n");
    return 0;
}
```

This uses Yacc to drive input processing, but the counters enforce a
condition beyond context-free grammar recognition.

## 3. Recognize `a^n b^m c^n`, n \> 1, m \>= 0

**File: `anbmcn.y`**

``` yacc
%{
#include <stdio.h>
int yylex(void);
void yyerror(const char *s) { }
%}
%%
S : 'a' 'a' A 'c' 'c' ;
A : /* empty */
  | 'a' A 'c'
  | 'b' A
  ;
%%
int yylex(void) {
    int c=getchar();
    if(c==EOF || c=='\n') return 0;
    return c;
}
int main(void) {
    printf("Enter a^n b^m c^n: ");
    if(yyparse()==0) printf("Valid\n");
    else printf("Invalid\n");
    return 0;
}
```

The grammar above is not correctly ordered for arbitrary `b^m` between
the a-block and c-block. Use the corrected grammar below instead.

``` yacc
%{
#include <stdio.h>
int yylex(void);
void yyerror(const char *s) { }
%}
%%
S : A B C ;
A : 'a' 'a' A1 ;
A1 : /* empty */ | 'a' A1 ;
B : /* empty */ | 'b' B ;
C : 'c' 'c' C1 ;
C1 : /* empty */ | 'c' C1 ;
%%
int yylex(void) {
    int c=getchar();
    if(c==EOF || c=='\n') return 0;
    return c;
}
int main(void) {
    printf("Enter string: ");
    if(yyparse()==0) printf("Valid (syntax only)\n");
    else printf("Invalid\n");
    return 0;
}
```

**Important correction:** the second grammar only checks at least two
a's, any number of b's, and at least two c's. It does not enforce equal
a/c counts. To enforce equality, use the counter-based Yacc solution in
item 2, adapted to require `na > 1` and `na == nc`.

## 4. Recognize `a^n b^m c^n`, n \> 1, m \>= 1

Use the counter-based method: require `na > 1`, `nb >= 1`, and
`na == nc`. The grammar-only approach cannot conveniently enforce
equality while allowing an independent middle block.

**File: `anbmcn1.y`**

``` yacc
%{
#include <stdio.h>
int na=0,nb=0,nc=0,phase=0,bad=0;
int yylex(void);
void yyerror(const char *s) { bad=1; }
%}
%%
input : /* empty */ | input ch ;
ch : 'a' { if(phase!=0) bad=1; na++; }
   | 'b' { if(phase>1) bad=1; phase=1; nb++; }
   | 'c' { phase=2; nc++; }
   ;
%%
int yylex(void) {
    int c=getchar();
    if(c==EOF || c=='\n') return 0;
    return c;
}
int main(void) {
    printf("Enter string: ");
    yyparse();
    if(!bad && na>1 && nb>=1 && na==nc) printf("Valid\n");
    else printf("Invalid\n");
    return 0;
}
```

## 5. Recognize `a^n b^n`, n \> 1

**File: `anbn_gt1.y`**

``` yacc
%{
#include <stdio.h>
int yylex(void);
void yyerror(const char *s) { }
%}
%%
S : 'a' 'a' A 'b' 'b' ;
A : /* empty */
  | 'a' A 'b'
  ;
%%
int yylex(void) {
    int c=getchar();
    if(c==EOF || c=='\n') return 0;
    return c;
}
int main(void) {
    printf("Enter string: ");
    if(yyparse()==0) printf("Valid\n");
    else printf("Invalid\n");
    return 0;
}
```

## 6. Recognize `a^n b^n`, n \>= 0

**File: `anbn_ge0.y`**

``` yacc
%{
#include <stdio.h>
int yylex(void);
void yyerror(const char *s) { }
%}
%%
S : /* empty */
  | 'a' S 'b'
  ;
%%
int yylex(void) {
    int c=getchar();
    if(c==EOF || c=='\n') return 0;
    return c;
}
int main(void) {
    printf("Enter string: ");
    if(yyparse()==0) printf("Valid\n");
    else printf("Invalid\n");
    return 0;
}
```

## 7. YACC calculator with precedence

**File: `calc.y`**

``` yacc
%{
#include <stdio.h>
int yylex(void);
void yyerror(const char *s) { printf("Invalid expression\n"); }
%}
%token NUM
%left '+' '-'
%left '*' '/'
%right UMINUS
%%
input : expr { printf("Result = %d\n", $1); } ;
expr  : NUM
      | expr '+' expr { $$=$1+$3; }
      | expr '-' expr { $$=$1-$3; }
      | expr '*' expr { $$=$1*$3; }
      | expr '/' expr { if($3==0) YYERROR; $$=$1/$3; }
      | '-' expr %prec UMINUS { $$=-$2; }
      | '(' expr ')' { $$=$2; }
      ;
%%
#include <ctype.h>
int yylex(void) {
    int c;
    while((c=getchar())==' ' || c=='\t');
    if(isdigit(c)) {
        yylval=c-'0';
        while(isdigit(c=getchar())) yylval=yylval*10+c-'0';
        ungetc(c,stdin);
        return NUM;
    }
    return c;
}
int main(void) { printf("Enter expression: "); return yyparse(); }
```

Compile: `yacc -d calc.y && gcc y.tab.c -o calc`

## 8. YACC arithmetic expression evaluator

This is the same arithmetic-expression evaluator as item 7. Use
`calc.y`; it supports `+ - * /`, parentheses, unary minus, and
precedence. Avoid maintaining two nearly identical lab programs.

## 9. Count and replace `scanf` / `printf` in a C source file

**File: `replace_io.l`**

``` lex
%{
#include <stdio.h>
int scanf_count=0, printf_count=0;
%}
%%
"scanf"  { printf("readf"); scanf_count++; }
"printf" { printf("writef"); printf_count++; }
.|\n     { ECHO; }
%%
int main(int argc,char **argv) {
    if(argc!=2) { fprintf(stderr,"Usage: %s input.c\n",argv[0]); return 1; }
    yyin=fopen(argv[1],"r");
    if(!yyin) { perror(argv[1]); return 1; }
    yylex();
    fprintf(stderr,"\nscanf count = %d\nprintf count = %d\n",scanf_count,printf_count);
    fclose(yyin);
    return 0;
}
int yywrap(void) { return 1; }
```

Build: `lex replace_io.l && gcc lex.yy.c -o replace_io -ll`

Run: `./replace_io source.c > converted.c`

Note: This simple lab solution replaces matching text even inside
comments or string literals. A production source-to-source transformer
must tokenize C correctly.

## 10. Count comment lines, remove comments, write cleaned source

**File: `remove_comments.l`**

``` lex
%{
#include <stdio.h>
int comment_lines=0;
int line_has_comment=0;
FILE *out;
%}
%x CMT
%%
"/*"        { BEGIN(CMT); line_has_comment=1; }
<CMT>"*/"   { BEGIN(INITIAL); }
<CMT>\n     { comment_lines++; }
<CMT>.      { }
"//".*      { comment_lines++; }
\n          { fputc('\n',out); }
.           { fputc(yytext[0],out); }
%%
int main(int argc,char **argv) {
    if(argc!=3) { fprintf(stderr,"Usage: %s input.c output.c\n",argv[0]); return 1; }
    yyin=fopen(argv[1],"r"); out=fopen(argv[2],"w");
    if(!yyin || !out) { perror("file"); return 1; }
    yylex();
    fclose(yyin); fclose(out);
    printf("Comment lines counted (approx.) = %d\n",comment_lines);
    return 0;
}
int yywrap(void) { return 1; }
```

This counts newline characters inside block comments and each `//`
comment as one line. It is a lab-level filter, not a full C lexer;
comment markers inside string literals need additional rules in a robust
implementation.

## 11. Recognize `xxxy`, `xyyy`, `xy`, and `x` using `L = {x^n y^n | n >= 0}`

The requested examples contradict the stated language: `xxxy` and `xyyy`
are **not** in `{x^n y^n}`. Only `xy` is valid among the listed nonempty
examples; `x` is invalid. The grammar below recognizes the stated
language.

**File: `xnyn.y`**

``` yacc
%{
#include <stdio.h>
int yylex(void);
void yyerror(const char *s) { }
%}
%%
S : /* empty */
  | 'x' S 'y'
  ;
%%
int yylex(void) {
    int c=getchar();
    if(c==EOF || c=='\n') return 0;
    return c;
}
int main(void) {
    printf("Enter string: ");
    if(yyparse()==0) printf("Accepted by x^n y^n\n");
    else printf("Rejected\n");
    return 0;
}
```

------------------------------------------------------------------------

# Part B -- LEX programs

For each `.l` file:

``` bash
lex filename.l
gcc lex.yy.c -o output -ll
./output
```

## 1. Validate a 6-digit Indian PIN code

**File: `pincode.l`**

``` lex
%{
#include <stdio.h>
%}
%%
[1-9][0-9]{5}    { printf("Valid PIN code\n"); }
[0-9]+           { printf("Invalid PIN code\n"); }
.|\n             { }
%%
int main(void) { printf("Enter PIN: "); yylex(); return 0; }
int yywrap(void) { return 1; }
```

This checks the common six-digit Indian PIN format (first digit 1--9).
It does not verify whether the PIN is assigned.

## 2. Extract HTML tags

**File: `html_tags.l`**

``` lex
%{
#include <stdio.h>
%}
%%
"<"[^>\n]+">"    { printf("%s\n",yytext); }
.|\n              { }
%%
int main(int argc,char **argv) {
    if(argc==2) { yyin=fopen(argv[1],"r"); if(!yyin){perror(argv[1]);return 1;} }
    yylex(); return 0;
}
int yywrap(void) { return 1; }
```

Run: `./html_tags page.html`

## 3. Recognize arithmetic expressions; identify identifiers and operators

**File: `arith_tokens.l`**

``` lex
%{
#include <stdio.h>
%}
ID  [A-Za-z_][A-Za-z0-9_]*
NUM [0-9]+(\.[0-9]+)?
%%
{ID}             { printf("IDENTIFIER: %s\n",yytext); }
{NUM}            { printf("NUMBER: %s\n",yytext); }
[+\-*/%=]        { printf("OPERATOR: %s\n",yytext); }
[(){}]           { printf("DELIMITER: %s\n",yytext); }
[ \t\r\n]+       ;
.                { printf("INVALID CHARACTER: %s\n",yytext); }
%%
int main(void) { printf("Enter expression: "); yylex(); return 0; }
int yywrap(void) { return 1; }
```

This identifies tokens; it does not prove that the complete token
sequence is syntactically valid. A parser is required for that.

## 4. Validate URL from a file

**File: `url.l`**

``` lex
%{
#include <stdio.h>
%}
%%
https?://[A-Za-z0-9.-]+\.[A-Za-z]{2,}(:[0-9]+)?(/[A-Za-z0-9._~:/?#\[\]@!$&'()*+,;=%-]*)?  { printf("Valid URL: %s\n",yytext); }
[^ \t\r\n]+ { printf("Invalid URL: %s\n",yytext); }
[ \t\r\n]+  ;
%%
int main(int argc,char **argv) {
    if(argc!=2){fprintf(stderr,"Usage: %s input.txt\n",argv[0]);return 1;}
    yyin=fopen(argv[1],"r"); if(!yyin){perror(argv[1]);return 1;}
    yylex(); fclose(yyin); return 0;
}
int yywrap(void){return 1;}
```

This is a simplified URL pattern, not a complete RFC URL validator.

## 5. Validate e-mail IDs from a file

**File: `email.l`**

``` lex
%{
#include <stdio.h>
%}
%%
[A-Za-z0-9._%+-]+@[A-Za-z0-9.-]+\.[A-Za-z]{2,} { printf("Valid email: %s\n",yytext); }
[^ \t\r\n]+ { printf("Invalid email: %s\n",yytext); }
[ \t\r\n]+  ;
%%
int main(int argc,char **argv) {
    if(argc!=2){fprintf(stderr,"Usage: %s input.txt\n",argv[0]);return 1;}
    yyin=fopen(argv[1],"r"); if(!yyin){perror(argv[1]);return 1;}
    yylex(); fclose(yyin); return 0;
}
int yywrap(void){return 1;}
```

## 6. Validate a date in DD/MM/YYYY format

**File: `date.l`**

``` lex
%{
#include <stdio.h>
#include <stdlib.h>
int valid_date(int d,int m,int y) {
    int days[]={0,31,28,31,30,31,30,31,31,30,31,30,31};
    if(y<1 || m<1 || m>12) return 0;
    if((y%400==0)||(y%4==0 && y%100!=0)) days[2]=29;
    return d>=1 && d<=days[m];
}
%}
%%
[0-9]{2}/[0-9]{2}/[0-9]{4} {
    int d,m,y;
    sscanf(yytext,"%d/%d/%d",&d,&m,&y);
    if(valid_date(d,m,y)) printf("Valid date\n");
    else printf("Invalid date\n");
}
[^ \t\r\n]+ { printf("Invalid format: %s\n",yytext); }
[ \t\r\n]+  ;
%%
int main(void) { printf("Enter date (DD/MM/YYYY): "); yylex(); return 0; }
int yywrap(void){return 1;}
```

## 7. Convert substring `abc` to `ABC`

**File: `replace_abc.l`**

``` lex
%{
#include <stdio.h>
%}
%%
abc       { printf("ABC"); }
.|\n      { ECHO; }
%%
int main(void) { yylex(); return 0; }
int yywrap(void) { return 1; }
```

## 8. Count vowels and consonants

**File: `vowels.l`**

``` lex
%{
#include <stdio.h>
int vowels=0, consonants=0;
%}
%%
[AEIOUaeiou]       { vowels++; }
[A-Za-z]           { consonants++; }
.|\n               ;
%%
int main(void) {
    yylex();
    printf("Vowels = %d\nConsonants = %d\n",vowels,consonants);
    return 0;
}
int yywrap(void) { return 1; }
```

------------------------------------------------------------------------

# Part C -- Combined LEX and YACC

## 9. Accept the snippet `while(a>b) { printf("a is greater"); }`

This recognizes the structure of this specific statement, not arbitrary
C.

**File: `while.y`**

``` yacc
%{
#include <stdio.h>
int yylex(void);
void yyerror(const char *s) { printf("Rejected\n"); }
%}
%token WHILE ID PRINTF STRING
%%
program : WHILE '(' ID '>' ID ')' '{' PRINTF '(' STRING ')' ';' '}' {
            printf("Accepted\n");
         }
        ;
%%
int main(void) { return yyparse(); }
```

**File: `while.l`**

``` lex
%{
#include "y.tab.h"
%}
%%
"while"       { return WHILE; }
"printf"      { return PRINTF; }
[a-zA-Z_][a-zA-Z0-9_]* { return ID; }
\"([^\\\"]|\\.)*\"      { return STRING; }
[ \t\r\n]+    ;
.             { return yytext[0]; }
%%
int yywrap(void) { return 1; }
```

Build:

``` bash
yacc -d while.y
lex while.l
gcc y.tab.c lex.yy.c -o while_parser -ll
./while_parser
```

## 10. Accept `main() { printf("hello"); }`

**File: `main.y`**

``` yacc
%{
#include <stdio.h>
int yylex(void);
void yyerror(const char *s) { printf("Rejected\n"); }
%}
%token MAIN PRINTF STRING
%%
program : MAIN '(' ')' '{' PRINTF '(' STRING ')' ';' '}' {
            printf("Accepted\n");
         }
        ;
%%
int main(void) { return yyparse(); }
```

**File: `main.l`**

``` lex
%{
#include "y.tab.h"
%}
%%
"main"        { return MAIN; }
"printf"      { return PRINTF; }
\"([^\\\"]|\\.)*\" { return STRING; }
[ \t\r\n]+    ;
.             { return yytext[0]; }
%%
int yywrap(void) { return 1; }
```

Build:

``` bash
yacc -d main.y
lex main.l
gcc y.tab.c lex.yy.c -o main_parser -ll
./main_parser
```

## 11. YACC program to recognize binary numbers

**File: `binary.y`**

``` yacc
%{
#include <stdio.h>
int yylex(void);
void yyerror(const char *s) { printf("Invalid binary number\n"); }
%}
%%
S : B ;
B : '0' | '1' | B '0' | B '1' ;
%%
int yylex(void) {
    int c=getchar();
    if(c==EOF || c=='\n') return 0;
    return c;
}
int main(void) {
    printf("Enter binary number: ");
    if(yyparse()==0) printf("Valid binary number\n");
    return 0;
}
```

## 12. YACC program to convert binary to decimal

**File: `bin_decimal.y`**

``` yacc
%{
#include <stdio.h>
int yylex(void);
void yyerror(const char *s) { printf("Invalid binary number\n"); }
%}
%type <ival> B
%union { long ival; }
%%
S : B { printf("Decimal = %ld\n",$1); } ;
B : '0' { $$=0; }
  | '1' { $$=1; }
  | B '0' { $$=$1*2; }
  | B '1' { $$=$1*2+1; }
  ;
%%
int yylex(void) {
    int c=getchar();
    if(c==EOF || c=='\n') return 0;
    return c;
}
int main(void) { printf("Enter binary number: "); return yyparse(); }
```

Build: `yacc -d bin_decimal.y && gcc y.tab.c -o bin_decimal`

------------------------------------------------------------------------

# Part D -- FIRST and FOLLOW

## 1. FIRST and FOLLOW for the given grammar

Grammar:

``` text
S -> A | B
A -> a S | b
B -> b S | c
```

**Result:**

``` text
FIRST(S) = { a, b, c }
FIRST(A) = { a, b }
FIRST(B) = { b, c }

FOLLOW(S) = { $ }
FOLLOW(A) = { $ }
FOLLOW(B) = { $ }
```

**C program (`first_follow.c`)** -- this program is specialized to the
grammar above:

``` c
#include <stdio.h>
int main(void) {
    printf("FIRST(S)  = { a, b, c }\n");
    printf("FIRST(A)  = { a, b }\n");
    printf("FIRST(B)  = { b, c }\n");
    printf("FOLLOW(S) = { $ }\n");
    printf("FOLLOW(A) = { $ }\n");
    printf("FOLLOW(B) = { $ }\n");
    return 0;
}
```

## 2. FIRST and FOLLOW with nullable non-terminals

Grammar:

``` text
S -> A B | C
A -> a | ε
B -> b | ε
C -> c
```

**Result:**

``` text
FIRST(A) = { a, ε }
FIRST(B) = { b, ε }
FIRST(C) = { c }
FIRST(S) = { a, b, c }

FOLLOW(S) = { $ }
FOLLOW(A) = { b, $ }
FOLLOW(B) = { $ }
FOLLOW(C) = { $ }
```

**C program (`nullable_first_follow.c`)** -- specialized to this
grammar:

``` c
#include <stdio.h>
int main(void) {
    printf("FIRST(A)  = { a, epsilon }\n");
    printf("FIRST(B)  = { b, epsilon }\n");
    printf("FIRST(C)  = { c }\n");
    printf("FIRST(S)  = { a, b, c }\n");
    printf("FOLLOW(S) = { $ }\n");
    printf("FOLLOW(A) = { b, $ }\n");
    printf("FOLLOW(B) = { $ }\n");
    printf("FOLLOW(C) = { $ }\n");
    return 0;
}
```

These two short C programs print the correct sets for the two supplied
grammars; they are not general-purpose grammar-analysis engines.

------------------------------------------------------------------------

# Part E -- Recursive descent parser

Grammar:

``` text
E  -> T E'
E' -> + T E' | epsilon
T  -> F T'
T' -> * F T' | epsilon
F  -> ( E ) | id
```

**File: `rd_parser.c`**

``` c
#include <stdio.h>
#include <ctype.h>
#include <string.h>

char input[200];
int pos=0;

void E(void);
void T(void);
void F(void);

void error(void) {
    printf("Invalid expression\n");
    exit(1);
}

void skip_spaces(void) {
    while(input[pos]==' ' || input[pos]=='\t') pos++;
}

void E(void) {
    T();
    skip_spaces();
    while(input[pos]=='+') {
        pos++;
        T();
        skip_spaces();
    }
}

void T(void) {
    F();
    skip_spaces();
    while(input[pos]=='*') {
        pos++;
        F();
        skip_spaces();
    }
}

void F(void) {
    skip_spaces();
    if(input[pos]=='(') {
        pos++;
        E();
        skip_spaces();
        if(input[pos]!=')') error();
        pos++;
    } else if(isalpha((unsigned char)input[pos])) {
        while(isalnum((unsigned char)input[pos]) || input[pos]=='_') pos++;
    } else {
        error();
    }
}

int main(void) {
    printf("Enter expression: ");
    if(!fgets(input,sizeof(input),stdin)) return 1;
    E();
    skip_spaces();
    if(input[pos]=='\n' || input[pos]=='\0') printf("Valid expression\n");
    else error();
    return 0;
}
```

Add `#include <stdlib.h>` at the top for the `exit()` declaration.

------------------------------------------------------------------------

# Part F -- Intermediate code generation

## 1. Generate three-address code for `(a+b)*(c+d)`

**Output:**

``` text
t1 = a + b
t2 = c + d
t3 = t1 * t2
```

## 2. Generate three-address code for `2+3*5+7`

Multiplication has higher precedence:

``` text
t1 = 3 * 5
t2 = 2 + t1
t3 = t2 + 7
```

**Simple C program (`tac.c`)** -- prints the intermediate code for both
fixed expressions:

``` c
#include <stdio.h>
int main(void) {
    printf("Expression: (a+b)*(c+d)\n");
    printf("t1 = a + b\n");
    printf("t2 = c + d\n");
    printf("t3 = t1 * t2\n\n");

    printf("Expression: 2+3*5+7\n");
    printf("t1 = 3 * 5\n");
    printf("t2 = 2 + t1\n");
    printf("t3 = t2 + 7\n");
    return 0;
}
```

This is a fixed-output lab demonstration, not a general
expression-to-TAC compiler.

------------------------------------------------------------------------

# Part G -- Lexical analyzer in C

Recognizes identifiers, keywords, numbers, operators, and special
symbols. Ignores spaces, tabs, newlines, and C comments.

**File: `lexer.c`**

``` c
#include <stdio.h>
#include <ctype.h>
#include <string.h>

int is_keyword(const char *s) {
    const char *kw[]={"int","float","char","double","if","else","while",
        "for","return","void","break","continue","switch","case","long",
        "short","do","struct","const","sizeof"};
    int n=sizeof(kw)/sizeof(kw[0]);
    for(int i=0;i<n;i++) if(strcmp(s,kw[i])==0) return 1;
    return 0;
}

int main(void) {
    int c,d;
    char token[256];
    while((c=getchar())!=EOF) {
        if(isspace(c)) continue;

        if(c=='/') {
            d=getchar();
            if(d=='/') {
                while((c=getchar())!=EOF && c!='\n');
                continue;
            }
            if(d=='*') {
                int prev=0;
                while((c=getchar())!=EOF) {
                    if(prev=='*' && c=='/') break;
                    prev=c;
                }
                continue;
            }
            if(d!=EOF) ungetc(d,stdin);
            printf("OPERATOR: /\n");
            continue;
        }

        if(isalpha(c) || c=='_') {
            int i=0;
            do {
                if(i<255) token[i++]=(char)c;
                c=getchar();
            } while(isalnum(c) || c=='_');
            token[i]='\0';
            if(c!=EOF) ungetc(c,stdin);
            printf("%s: %s\n",is_keyword(token)?"KEYWORD":"IDENTIFIER",token);
            continue;
        }

        if(isdigit(c)) {
            int i=0;
            do {
                if(i<255) token[i++]=(char)c;
                c=getchar();
            } while(isdigit(c));
            if(c=='.') {
                if(i<255) token[i++]=(char)c;
                c=getchar();
                while(isdigit(c)) {
                    if(i<255) token[i++]=(char)c;
                    c=getchar();
                }
            }
            token[i]='\0';
            if(c!=EOF) ungetc(c,stdin);
            printf("NUMBER: %s\n",token);
            continue;
        }

        if(c=='"' || c=='\\'') {
            int quote=c, i=0;
            token[i++]=(char)c;
            while((c=getchar())!=EOF) {
                if(i<254) token[i++]=(char)c;
                if(c=='\\\\') {
                    c=getchar();
                    if(c==EOF) break;
                    if(i<254) token[i++]=(char)c;
                } else if(c==quote) break;
            }
            token[i]='\0';
            printf("LITERAL: %s\n",token);
            continue;
        }

        d=getchar();
        if((c=='=' && d=='=') || (c=='!' && d=='=') ||
           (c=='<' && d=='=') || (c=='>' && d=='=') ||
           (c=='+' && d=='+') || (c=='-' && d=='-') ||
           (c=='+' && d=='=') || (c=='-' && d=='=') ||
           (c=='*' && d=='=') || (c=='/' && d=='=') ||
           (c=='&' && d=='&') || (c=='|' && d=='|')) {
            printf("OPERATOR: %c%c\n",c,d);
        } else {
            if(d!=EOF) ungetc(d,stdin);
            if(strchr("+-*%=<>!&|^~",c)) printf("OPERATOR: %c\n",c);
            else if(strchr("(){}[];,.:?",c)) printf("SPECIAL SYMBOL: %c\n",c);
            else printf("UNKNOWN: %c\n",c);
        }
    }
    return 0;
}
```

Compile: `gcc lexer.c -o lexer`

Run: `./lexer < source.c`

------------------------------------------------------------------------

## Important lab notes

1.  In a Yacc file, `%{ ... %}` contains C declarations; `%token`,
    `%left`, `%right`, and `%start` are Yacc declarations.
2.  `%%` separates the declaration section, grammar rules, and user C
    code.
3.  `lex file.l` generates `lex.yy.c`.
4.  `yacc -d file.y` generates `y.tab.c` and `y.tab.h`.
5.  For combined Lex/Yacc programs, include `y.tab.h` in the Lex file
    and compile both generated C files.
6.  Items 2--4 involve equal-count constraints that are not all
    expressible by a context-free grammar alone. Use semantic
    checks/counters where needed; do not claim a plain CFG handles a
    non-context-free language.
