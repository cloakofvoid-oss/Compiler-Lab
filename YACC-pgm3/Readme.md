# implement calculator using Yacc
## Date : 07/09/26

##Algorithm :
**Algorithm: Arithmetic Expression Evaluator (Lex/Yacc)**

1. **Start**
2. Print prompt and call `yyparse()`
3. **Lexer (yylex):**
   a. Read next character
   b. If digit → convert to number, return `NUMBER`
   c. If space/tab → skip
   d. If `\n` → return `\n`
   e. Else → return character as operator token
4. **Parser (yacc rules):**
   a. Get tokens from lexer
   b. Apply grammar with precedence: `*, /, %` before `+, -`
   c. For `(E)` → evaluate inner `E`
   d. For `E op E` → compute result based on `op`
   e. For single `NUMBER` → value = number
5. **On `E '\n'`:**
   a. Print `Result = value`
   b. Return (end parsing)
6. **On invalid input:** call `yyerror()` → print "Invalid expression"
7. **Stop**

## Output :

<img width="533" height="196" alt="image" src="https://github.com/user-attachments/assets/867f32db-2917-46cc-894b-06c04e544cff" />

