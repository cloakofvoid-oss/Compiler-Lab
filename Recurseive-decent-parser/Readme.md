# Prgram to implement recursive decent parser in c
## Date :
## Algorithm
 
 
 1. Start.
 2. Declare a global character array input and a global index pos, initialized to 0
 3. Prompt the user to enter an arithmetic expression and read it into input.
 4. Call the function E() to begin parsing.
    1. E(): Call T(). While the current character is + or -, advance pos and call T() again.
    2. T(): Call F(). While the current character is * or /, advance pos and call F() again.
    3. F(): If the current character is (, advance pos, call E(), and then check for ). If ) is present, advance pos; otherwise, report an error. If the current
    4. character is a digit, consume all consecutive digits. In any other case, report an error.
 6. After E() returns, check whether input[pos] is the end-of-string character '\0'.
 7. If yes, print “Valid expression!”
 8. If no, report an error because unconsumed characters remain.
 9. Error handling: Print “Invalid expression!” and terminate the program.
 10. Stop.

## Output

    student@student:~/cathereen_compiler$ gedit leftparser.c
    student@student:~/cathereen_compiler$ gcc leftparser.c
    student@student:~/cathereen_compiler$ ./a.out
    Enter an arithmetic expression: 3+2*5
    Valid expression!
    student@student:~/cathereen_compiler$ ./a.out
    Enter an arithmetic expression: 2++6
    Invalid expression!
