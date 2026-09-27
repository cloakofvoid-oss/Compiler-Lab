# implement calculator using Yacc
## Date : 07/09/26

## Algorithm : Arithmetic Calculator (Lex/Yacc)
**A. Lex Algorithm**

1. Start
2. Read input character(s)
3. If character(s) form digits → convert to integer using `atoi`, store in `yylval`, return token `NUMBER`
4. If character is space or tab → ignore, read next character
5. If character is newline (`\n`) → return `\n` as token
6. Else (character is `+`, `-`, `*`, `/`, `%`, `(`, `)`) → return that character as token
7. Repeat steps 2–6 until end of input
8. Stop

**B. Yacc Algorithm**

1. Start
2. Call `yylex()` to get next token from lexer
3. Match tokens against grammar rules based on precedence (`*`, `/`, `%` before `+`, `-`)
4. If rule is `(E)` → evaluate inner `E`, set result
5. If rule is `E op E` → compute result:
   - `+` → add
   - `-` → subtract
   - `*` → multiply
   - `/` → divide
   - `%` → modulus
6. If rule is `NUMBER` → set value = number
7. Repeat steps 2–6, reducing tokens until single expression `E` remains
8. When `E '\n'` is matched:
   - Print `Result = value`
   - Return (stop parsing)
9. If no rule matches → call `yyerror()` → print "Invalid expression"
10. Stop

## Output :

<img width="533" height="196" alt="image" src="https://github.com/user-attachments/assets/867f32db-2917-46cc-894b-06c04e544cff" />

