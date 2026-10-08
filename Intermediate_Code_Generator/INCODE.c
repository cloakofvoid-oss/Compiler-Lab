#include <stdio.h>
#include <string.h>
#include <ctype.h>
char expr[100];
int temp = 1;
char *newtemp()
{
    static char t[20];
    sprintf(t, "t%d", temp++);
    return t;
}
int precedence(char op) {
    if (op == '*' || op == '/')
        return 2;
    if (op == '+' || op == '-')
        return 1;
    return 0;
}

void generate(char op) {
    char left[20], right[20], result[20];
    int i, j;
    while (1) {
        int found = 0;
        for (i = 0; expr[i] != '\0'; i++) {
            if (expr[i] == op) {
                found = 1;
                j = i - 1;
                while (j >= 0 && expr[j] == ' ')
                    j--;
                int end = j;
                while (j >= 0 && (isalnum(expr[j]) || expr[j] == '_'))
                    j--;
                int start = j + 1;
                strncpy(left, &expr[start], end - start + 1);
                left[end - start + 1] = '\0';
                j = i + 1;

                int k = 0;

                while (expr[j] != '\0' &&
                       (isalnum(expr[j]) || expr[j] == '_')) {
                    right[k++] = expr[j++];
                }
                right[k] = '\0';
                strcpy(result, newtemp());
                printf("%s = %s %c %s\n",
                       result, left, op, right);
                char newexpr[100];
                strncpy(newexpr, expr, start);
                newexpr[start] = '\0';
                strcat(newexpr, result);
                strcat(newexpr, &expr[j]);

                strcpy(expr, newexpr);

                break;
            }
        }

        if (!found)
            break;
    }
}

void process_parentheses() {
    int open, close;

    while (strchr(expr, '(') != NULL) {
        open = -1;
        close = -1;
        for (int i = 0; expr[i] != '\0'; i++) {
            if (expr[i] == '(')
                open = i;

            if (expr[i] == ')' && open != -1) {
                close = i;
                break;
            }
        }
        char sub[100];
        int k = 0;
        for (int i = open + 1; i < close; i++)
            sub[k++] = expr[i];
        sub[k] = '\0';
        strcpy(expr + open, sub);
        memmove(
            expr + open + strlen(sub),
            expr + close + 1,
            strlen(expr) - close
        );
    }
}

int main() {
    printf("Enter an expression: ");
    scanf("%s", expr);
    process_parentheses();
    generate('*');
    generate('/');
    generate('+');
    generate('-');
    return 0;
}
