#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

char input[100];
int pos = 0;

void E();
void T();
void F();

void error()
{
    printf("Invalid expression!\n");
    exit(1);
}
void E()
{
    T();

    while (input[pos] == '+' || input[pos] == '-')
    {
        pos++;
        T();
    }
}
void T()
{
    F();

    while (input[pos] == '*' || input[pos] == '/')
    {
        pos++;
        F();
    }
}
void F()
{
    if (input[pos] == '(')
    {
        pos++;
        E();

        if (input[pos] == ')')
            pos++;
        else
            error();
    }
    else if (isdigit((unsigned char)input[pos]))
    {
        while (isdigit((unsigned char)input[pos]))
            pos++;
    }
    else
    {
        error();
    }
}
int main()
{
    printf("Enter an arithmetic expression: ");
    scanf("%99s", input);

    E();

    if (input[pos] == '\0')
        printf("Valid expression!\n");
    else
        error();

    return 0;
}
