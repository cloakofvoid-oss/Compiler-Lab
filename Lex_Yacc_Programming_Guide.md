# Lex & Yacc Programming Guide

A practical reference for writing common **Lex/Flex** and **Yacc/Bison**
programs for Compiler Design labs and exams.

------------------------------------------------------------------------

## Table of Contents

1.  [Lex Syntax](#1-lex-syntax)
2.  [How Lex Works](#2-how-lex-works)
3.  [Yacc Syntax](#3-yacc-syntax)
4.  [Yacc Grammar Rules](#4-yacc-grammar-rules)
5.  [Lex and Yacc Connection](#5-lex-and-yacc-connection)
6.  [Semantic Actions](#6-semantic-actions)
7.  [Semantic Values: `$1`, `$2`, `$3`, `$$`](#7-semantic-values)
8.  [Operator Precedence](#8-operator-precedence)
9.  [Important Lex/Yacc Functions](#9-important-lexyacc-functions)
10. [Common Lex Programs](#10-common-lex-programs)
11. [Common Yacc Programs](#11-common-yacc-programs)
12. [Combined Lex + Yacc Calculator](#12-combined-lex--yacc-calculator)
13. [What to Memorize](#13-what-to-memorize)
14. [Recommended Practice Order](#14-recommended-practice-order)

------------------------------------------------------------------------

# 1. Lex Syntax

A Lex/Flex file normally has **three sections**:

``` lex
%{
    /* C declarations */
%}

%%
    /* Lex rules */
%%

    /* C functions */
```

The `%%` separators are mandatory.

## General Lex Template

``` lex
%{
#include <stdio.h>
%}

%%
pattern     { action; }
pattern     { action; }
pattern     { action; }

%%

int main()
{
    yylex();
    return 0;
}
```

## Section 1 --- C Declarations

``` lex
%{
#include <stdio.h>

int count = 0;
%}
```

Everything between `%{` and `%}` is copied directly into the generated C
code.

Typical contents:

-   `#include` statements
-   Global variables
-   Function declarations
-   Constants

## Section 2 --- Lex Rules

The basic syntax is:

``` text
PATTERN     ACTION
```

Example:

``` lex
[0-9]+      { printf("Number"); }
```

This means:

> If one or more digits are found, execute the C action.

------------------------------------------------------------------------

# 2. How Lex Works

Suppose the input is:

``` text
abc 123 + xyz
```

Lex scans from left to right.

Possible matches:

``` text
abc  → [a-zA-Z]+
123  → [0-9]+
+    → "+"
xyz  → [a-zA-Z]+
```

For every match, the corresponding action executes.

## `yytext`

`yytext` contains the currently matched text.

``` lex
[0-9]+ {
    printf("Number = %s\n", yytext);
}
```

For input:

``` text
12345
```

the value of `yytext` is:

``` text
12345
```

## `yyleng`

`yyleng` contains the length of the matched text.

``` c
printf("%d", yyleng);
```

------------------------------------------------------------------------

## Common Lex Patterns

  Pattern       Meaning
  ------------- ----------------------------
  `[0-9]`       One digit
  `[0-9]+`      One or more digits
  `[0-9]*`      Zero or more digits
  `[a-z]`       One lowercase letter
  `[a-zA-Z]`    One alphabetic character
  `[a-zA-Z]+`   One or more letters
  `.`           Any single character
  `\n`          Newline
  `\t`          Tab
  `"+"`         Literal `+`
  `"*"`         Literal `*`
  `"("`         Literal `(`
  `")"`         Literal `)`
  `[ \t\n]+`    Whitespace
  `^abc`        `abc` at beginning of line
  `abc$`        `abc` at end of line

When a character has special meaning in regular expressions but you want
the literal character, quote it:

``` lex
"+"     { ... }
"*"     { ... }
"("     { ... }
")"     { ... }
```

------------------------------------------------------------------------

# 3. Yacc Syntax

Yacc/Bison files also have **three sections**:

``` yacc
%{
    /* C declarations */
%}

/* Yacc declarations */

%%
    /* Grammar rules */
%%

    /* C functions */
```

The structure is:

``` text
C declarations
      ↓
Yacc declarations
      ↓
Grammar rules
      ↓
C code
```

## Basic Yacc Template

``` yacc
%{
#include <stdio.h>
#include <stdlib.h>
%}

%token NUMBER

%%

expr : NUMBER
     ;

%%

int main()
{
    yyparse();
    return 0;
}

int yyerror(char *s)
{
    printf("Error\n");
    return 0;
}
```

------------------------------------------------------------------------

# 4. Yacc Grammar Rules

Suppose the grammar is:

``` text
E → E + T
E → T
T → NUMBER
```

The Yacc version is:

``` yacc
expr : expr '+' term
     | term
     ;

term : NUMBER
     ;
```

The general form is:

``` yacc
NON_TERMINAL
    : production
    | production
    | production
    ;
```

For example:

``` text
E → E + T
E → T
```

becomes:

``` yacc
E : E '+' T
  | T
  ;
```

------------------------------------------------------------------------

# 5. Lex and Yacc Connection

The most important concept is:

``` text
INPUT
  ↓
LEX/FLEX
  ↓
TOKENS
  ↓
YACC/BISON
  ↓
GRAMMAR
  ↓
SEMANTIC ACTION
  ↓
OUTPUT
```

Lex recognizes patterns and returns tokens.

Yacc receives those tokens and checks whether they satisfy the grammar.

## Example

Lex:

``` lex
[0-9]+ {
    yylval = atoi(yytext);
    return NUMBER;
}
```

For input:

``` text
123
```

Lex returns:

``` text
NUMBER
```

and stores the value:

``` text
123
```

Yacc can then use:

``` yacc
expr : NUMBER
     ;
```

------------------------------------------------------------------------

# 6. Semantic Actions

Yacc grammar rules can execute C code.

Example:

``` yacc
expr : expr '+' expr
       {
           printf("Addition\n");
       }
     ;
```

General syntax:

``` yacc
rule : production
       {
           /* C code */
       }
     ;
```

Semantic actions are used for:

-   Arithmetic calculations
-   Syntax-tree construction
-   Symbol-table operations
-   Code generation
-   Validation
-   Printing results

------------------------------------------------------------------------

# 7. Semantic Values

Consider:

``` yacc
expr : expr '+' expr
       {
           $$ = $1 + $3;
       }
     ;
```

The meaning is:

``` text
$1 → value of first symbol
$2 → value of second symbol
$3 → value of third symbol
$$ → value of the complete rule
```

So:

``` text
expr '+' expr
 ↑     ↑      ↑
$1    $2     $3
```

The `+` normally does not carry a useful semantic value.

Therefore:

``` c
$$ = $1 + $3;
```

means:

> The value of this `expr` is the value of the left expression plus the
> value of the right expression.

------------------------------------------------------------------------

# 8. Operator Precedence

For:

``` text
3 + 4 * 5
```

we normally want:

``` text
3 + (4 * 5)
```

not:

``` text
(3 + 4) * 5
```

Yacc can specify precedence:

``` yacc
%left '+'
%left '*'
```

The later declaration has higher precedence.

A typical declaration is:

``` yacc
%left '+' '-'
%left '*' '/'
```

Therefore:

``` text
* /   → higher precedence
+ -   → lower precedence
```

## `%left`

Used for left-associative operators.

``` yacc
%left '+' '-'
```

Then:

``` text
a - b - c
```

is interpreted as:

``` text
(a - b) - c
```

## `%right`

Used for right-associative operators.

``` yacc
%right '^'
```

Then:

``` text
a ^ b ^ c
```

is interpreted as:

``` text
a ^ (b ^ c)
```

## `%nonassoc`

Used when chaining should be rejected.

``` yacc
%nonassoc '<' '>'
```

## `%prec`

Used to assign a precedence to a particular grammar rule.

Example:

``` yacc
%right UMINUS

%%

E : '-' E %prec UMINUS
  ;
```

------------------------------------------------------------------------

# 9. Important Lex/Yacc Functions

## `yyparse()`

Starts the Yacc parser.

``` c
yyparse();
```

## `yylex()`

Gets the next token from Lex.

Usually Yacc calls it automatically.

``` c
yylex();
```

## `yyerror()`

Called when a syntax error occurs.

``` c
int yyerror(char *s)
{
    printf("Syntax error\n");
    return 0;
}
```

## `yytext`

Currently matched Lex string.

``` c
printf("%s", yytext);
```

## `yyleng`

Length of the currently matched string.

``` c
printf("%d", yyleng);
```

## `yylval`

Semantic value passed from Lex to Yacc.

Example:

``` lex
[0-9]+ {
    yylval = atoi(yytext);
    return NUMBER;
}
```

------------------------------------------------------------------------

# 10. Common Lex Programs

## 10.1 Count Vowels, Consonants, Digits and Spaces

``` lex
%{
#include <stdio.h>
int vowels=0, consonants=0, digits=0, spaces=0;
%}

%%
[aeiouAEIOU]       { vowels++; }
[a-zA-Z]           { consonants++; }
[0-9]              { digits++; }
[ \t\n]            { spaces++; }
%%

int main()
{
    yylex();

    printf("Vowels = %d\n", vowels);
    printf("Consonants = %d\n", consonants);
    printf("Digits = %d\n", digits);
    printf("Spaces = %d\n", spaces);

    return 0;
}

int yywrap()
{
    return 1;
}
```

------------------------------------------------------------------------

## 10.2 Count Words, Lines and Characters

``` lex
%{
#include <stdio.h>
int words=0, lines=0, chars=0;
%}

%%
\n              { lines++; chars++; }
[ \t]+          { chars += yyleng; }
[A-Za-z0-9]+    { words++; chars += yyleng; }
.               { chars++; }
%%

int main()
{
    yylex();

    printf("Lines = %d\n", lines);
    printf("Words = %d\n", words);
    printf("Characters = %d\n", chars);

    return 0;
}

int yywrap()
{
    return 1;
}
```

------------------------------------------------------------------------

## 10.3 Recognize an Identifier

Typical C identifier:

``` text
letter → letter/digit*
```

Lex:

``` lex
%{
#include <stdio.h>
%}

%%
[a-zA-Z_][a-zA-Z0-9_]* {
    printf("Valid Identifier: %s\n", yytext);
}

[ \t\n]+ ;

. {
    printf("Invalid character: %s\n", yytext);
}
%%

int main()
{
    yylex();
    return 0;
}

int yywrap()
{
    return 1;
}
```

------------------------------------------------------------------------

## 10.4 Recognize Integers and Floating-Point Numbers

``` lex
%{
#include <stdio.h>
%}

%%
[0-9]+              { printf("Integer: %s\n", yytext); }
[0-9]+\.[0-9]+      { printf("Float: %s\n", yytext); }
[ \t\n]+            ;
.                   { printf("Other: %s\n", yytext); }
%%

int main()
{
    yylex();
    return 0;
}

int yywrap()
{
    return 1;
}
```

------------------------------------------------------------------------

## 10.5 Recognize Keywords

``` lex
%{
#include <stdio.h>
%}

%%
"int"       { printf("Keyword: int\n"); }
"float"     { printf("Keyword: float\n"); }
"char"      { printf("Keyword: char\n"); }
"if"        { printf("Keyword: if\n"); }
"else"      { printf("Keyword: else\n"); }
"while"     { printf("Keyword: while\n"); }
"for"       { printf("Keyword: for\n"); }
"return"    { printf("Keyword: return\n"); }

[a-zA-Z_][a-zA-Z0-9_]* {
    printf("Identifier: %s\n", yytext);
}

[ \t\n]+ ;

. ;
%%

int main()
{
    yylex();
    return 0;
}

int yywrap()
{
    return 1;
}
```

------------------------------------------------------------------------

## 10.6 Identify Operators

``` lex
%{
#include <stdio.h>
%}

%%
"=="|"!="|"<="|">=" {
    printf("Relational Operator: %s\n", yytext);
}

"&&"|"||" {
    printf("Logical Operator: %s\n", yytext);
}

"++"|"--" {
    printf("Increment/Decrement: %s\n", yytext);
}

"+"|"-"|"*"|"/"|"%" {
    printf("Arithmetic Operator: %s\n", yytext);
}

"=" {
    printf("Assignment Operator\n");
}

[ \t\n]+ ;

. {
    printf("Special Symbol: %s\n", yytext);
}
%%

int main()
{
    yylex();
    return 0;
}

int yywrap()
{
    return 1;
}
```

------------------------------------------------------------------------

## 10.7 Count Positive and Negative Numbers

``` lex
%{
#include <stdio.h>
int positive=0, negative=0;
%}

%%
-[0-9]+       { negative++; }
[0-9]+        { positive++; }
[ \t\n]+      ;
.             ;
%%

int main()
{
    yylex();

    printf("Positive = %d\n", positive);
    printf("Negative = %d\n", negative);

    return 0;
}

int yywrap()
{
    return 1;
}
```

------------------------------------------------------------------------

## 10.8 Remove Comments from a C Program

``` lex
%{
#include <stdio.h>
%}

%%
"//".*                          ;
"/*"([^*]|\*+[^*/])*\*+"/"     ;
\n                              { printf("\n"); }
.                               { printf("%s", yytext); }
%%

int main()
{
    yylex();
    return 0;
}

int yywrap()
{
    return 1;
}
```

------------------------------------------------------------------------

## 10.9 Count Comments

``` lex
%{
#include <stdio.h>
int comments = 0;
%}

%%
"//".*                          { comments++; }
"/*"([^*]|\*+[^*/])*\*+"/"     { comments++; }
.|\n                            ;
%%

int main()
{
    yylex();

    printf("Number of comments = %d\n", comments);

    return 0;
}

int yywrap()
{
    return 1;
}
```

------------------------------------------------------------------------

## 10.10 Recognize an Email Address

``` lex
%{
#include <stdio.h>
%}

%%
[a-zA-Z0-9._%+-]+@[a-zA-Z0-9.-]+\.[a-zA-Z]+ {
    printf("Valid Email: %s\n", yytext);
}

[ \t\n]+ ;

. {
    printf("Invalid: %s\n", yytext);
}
%%

int main()
{
    yylex();
    return 0;
}

int yywrap()
{
    return 1;
}
```

------------------------------------------------------------------------

# 11. Common Yacc Programs

## 11.1 Validate an Arithmetic Expression

Grammar:

``` text
E → E + T
E → T

T → T * F
T → F

F → (E)
F → id
```

Yacc:

``` yacc
%{
#include <stdio.h>

int yylex();
int yyerror(char *s);
%}

%token ID

%%

E : E '+' T
  | T
  ;

T : T '*' F
  | F
  ;

F : '(' E ')'
  | ID
  ;

%%

int main()
{
    printf("Enter expression: ");
    yyparse();

    printf("Valid expression\n");

    return 0;
}

int yyerror(char *s)
{
    printf("Invalid expression\n");
    return 0;
}
```

------------------------------------------------------------------------

## 11.2 Arithmetic Expression Evaluation

### `calc.l`

``` lex
%{
#include "y.tab.h"
#include <stdlib.h>
%}

%%
[0-9]+ {
    yylval = atoi(yytext);
    return NUMBER;
}

[ \t]+ ;
\n      return '\n';

.       return yytext[0];
%%

int yywrap()
{
    return 1;
}
```

### `calc.y`

``` yacc
%{
#include <stdio.h>

int yylex();
int yyerror(char *s);
%}

%token NUMBER

%left '+' '-'
%left '*' '/'

%%

E : E '+' E     { $$ = $1 + $3; }
  | E '-' E     { $$ = $1 - $3; }
  | E '*' E     { $$ = $1 * $3; }
  | E '/' E     { $$ = $1 / $3; }
  | '(' E ')'   { $$ = $2; }
  | NUMBER      { $$ = $1; }
  ;

%%

int main()
{
    printf("Enter expression:\n");
    yyparse();
    return 0;
}

int yyerror(char *s)
{
    printf("Invalid expression\n");
    return 0;
}
```

> For a production-quality calculator, store the final semantic value
> explicitly rather than relying on the final `yylval`. The important
> lab concepts here are `%token`, precedence, `yylval`, `$1`, `$3`, and
> `$$`.

------------------------------------------------------------------------

## 11.3 Balanced Parentheses

Grammar:

``` text
S → (S)S
S → ε
```

Yacc:

``` yacc
%{
#include <stdio.h>

int yylex();
int yyerror(char *s);
%}

%%

S : '(' S ')' S
  |
  ;

%%

int main()
{
    if (yyparse() == 0)
        printf("Balanced\n");

    return 0;
}

int yyerror(char *s)
{
    printf("Not Balanced\n");
    return 0;
}
```

------------------------------------------------------------------------

## 11.4 Recognize `aⁿbⁿ`

Grammar:

``` text
S → aSb
S → ε
```

Yacc:

``` yacc
%{
#include <stdio.h>

int yylex();
int yyerror(char *s);
%}

%%

S : 'a' S 'b'
  |
  ;

%%

int main()
{
    if (yyparse() == 0)
        printf("Valid string\n");

    return 0;
}

int yyerror(char *s)
{
    printf("Invalid string\n");
    return 0;
}
```

Valid strings:

``` text
ab
aabb
aaabbb
```

Invalid strings:

``` text
aab
abb
abab
```

------------------------------------------------------------------------

## 11.5 Recognize Strings Ending in `abb`

``` yacc
%{
#include <stdio.h>

int yylex();
int yyerror(char *s);
%}

%%

S : A 'a' 'b' 'b'
  ;

A : A 'a'
  | A 'b'
  |
  ;

%%

int main()
{
    if (yyparse() == 0)
        printf("Valid string\n");

    return 0;
}

int yyerror(char *s)
{
    printf("Invalid string\n");
    return 0;
}
```

------------------------------------------------------------------------

## 11.6 Arithmetic Expression with Explicit Grammar

Grammar:

``` text
E → E + T
E → T

T → T * F
T → F

F → NUMBER
```

Yacc:

``` yacc
%{
#include <stdio.h>
#include <stdlib.h>

int yylex();
int yyerror(char *s);
%}

%token NUMBER

%%

E : E '+' T     { $$ = $1 + $3; }
  | T           { $$ = $1; }
  ;

T : T '*' F     { $$ = $1 * $3; }
  | F           { $$ = $1; }
  ;

F : NUMBER      { $$ = $1; }
  ;

%%

int main()
{
    yyparse();
    return 0;
}

int yyerror(char *s)
{
    printf("Error\n");
    return 0;
}
```

------------------------------------------------------------------------

## 11.7 Infix to Postfix Conversion

For:

``` text
a+b*c
```

the postfix form is:

``` text
abc*+
```

A typical Yacc grammar is:

``` yacc
%{
#include <stdio.h>

int yylex();
int yyerror(char *s);
%}

%token ID

%%

E : E '+' T     { printf("+"); }
  | E '-' T     { printf("-"); }
  | T
  ;

T : T '*' F     { printf("*"); }
  | T '/' F     { printf("/"); }
  | F
  ;

F : '(' E ')'
  | ID          { /* print identifier value here */ }
  ;

%%

int main()
{
    yyparse();
    return 0;
}

int yyerror(char *s)
{
    printf("Invalid expression\n");
    return 0;
}
```

The exact Lex scanner must supply the identifier value appropriately.

------------------------------------------------------------------------

# 12. Combined Lex + Yacc Calculator

This is one of the most useful programs to practice because it combines:

-   Lex patterns
-   Tokens
-   `yylval`
-   Yacc grammar
-   Semantic actions
-   Operator precedence
-   `%prec`
-   `$1`, `$2`, `$3`
-   `$$`

## `calc.l`

``` lex
%{
#include "y.tab.h"
#include <stdlib.h>
%}

%%
[0-9]+ {
    yylval = atoi(yytext);
    return NUMBER;
}

[ \t]       ;
\n          return '\n';

"+"         return '+';
"-"         return '-';
"*"         return '*';
"/"         return '/';
"("         return '(';
")"         return ')';

%%

int yywrap()
{
    return 1;
}
```

## `calc.y`

``` yacc
%{
#include <stdio.h>
#include <stdlib.h>

int yylex();
int yyerror(char *s);
%}

%token NUMBER

%left '+' '-'
%left '*' '/'
%right UMINUS

%%

E : E '+' E
      { $$ = $1 + $3; }

  | E '-' E
      { $$ = $1 - $3; }

  | E '*' E
      { $$ = $1 * $3; }

  | E '/' E
      { $$ = $1 / $3; }

  | '-' E %prec UMINUS
      { $$ = -$2; }

  | '(' E ')'
      { $$ = $2; }

  | NUMBER
      { $$ = $1; }
  ;

%%

int main()
{
    printf("Enter expression: ");
    yyparse();
    return 0;
}

int yyerror(char *s)
{
    printf("Syntax Error\n");
    return 0;
}
```

------------------------------------------------------------------------

# 13. What to Memorize

Do not memorize every program. Memorize the **building blocks**.

## Lex

``` text
%{
    C code
%}

%%
REGEX       { ACTION; }
REGEX       { ACTION; }
%%

main()
```

Important Lex concepts:

``` text
yytext
yyleng
yylex()
yylval
return TOKEN
```

## Yacc

``` text
%{
    C code
%}

%token ...
%left ...
%right ...

%%

E : E '+' T
  | T
  ;

%%

main()
yyerror()
```

Important Yacc concepts:

``` text
%token
yyparse()
yyerror()
$1
$2
$3
$$
%left
%right
%nonassoc
%prec
```

------------------------------------------------------------------------

# 14. Recommended Practice Order

For a Compiler Design lab/exam, practice in this order:

  Priority   Program                           Importance
  ---------- --------------------------------- ------------
  1          Lex identifier recognizer         Very high
  2          Lex word/line/character counter   Very high
  3          Lex keyword/operator recognizer   Very high
  4          Lex comment removal               High
  5          Yacc expression validation        Very high
  6          Yacc arithmetic calculator        Very high
  7          Yacc expression evaluation        Very high
  8          Infix → postfix                   Very high
  9          Balanced parentheses              High
  10         `aⁿbⁿ` recognizer                 High
  11         Identifier + symbol recognition   Medium
  12         Email/number recognizers          Medium

------------------------------------------------------------------------

# Core Mental Model

``` text
                     INPUT
                       │
                       ▼
                    LEX/FLEX
                       │
                Recognizes patterns
                       │
                       ▼
                     TOKEN
                       │
                       ▼
                   YACC/BISON
                       │
                 Checks grammar
                       │
                       ▼
                Semantic action
                       │
                       ▼
                    OUTPUT
```

The key idea is:

``` text
REGEX → TOKEN → GRAMMAR → SEMANTIC ACTION
```

If you understand that pipeline, most Lex/Yacc lab questions are
variations of the same structure.
