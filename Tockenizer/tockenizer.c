#include <stdio.h>
#include <string.h>
#include <ctype.h>

int isKeyword(char *str) {
char *k[] = {"if", "else", "while", "for", "int", "float","char", "double", "return", "void","break", "continue" };
for (int i = 0; i < 12; i++) {
if (strcmp(str, k[i]) == 0)
return 1;
}
return 0; 
}
int isOperator(char *str) {
char *o[] = {"+", "-", "*", "/", "="};
for (int i = 0; i < 5; i++) {
if (strcmp(str, o[i]) == 0)
            return 1;
}
return 0;
}
int isNumber(char str[]) {
for (int i = 0; str[i] != '\0'; i++) {
if (!isdigit(str[i]))
return 0;
}
return 1;
}
int isIdentifier(char str[]) {
if (!(isalpha(str[0]) || str[0] == '_'))
return 0;
for (int i = 1; str[i] != '\0'; i++) {
if (!(isalnum(str[i]) || str[i] == '_'))
return 0;
}
return 1;
}
int main() {
FILE *fp;
char line[200];
char *token;
fp = fopen("sample.c", "r");
if (fp == NULL) {
printf("File not found!\n");
return 1;
}
while (fgets(line, sizeof(line), fp) != NULL) {
token = strtok(line, " \t\n;,(){}");
while (token != NULL) {
if (isKeyword(token))
printf("%s --> Keyword\n", token);
else if (isOperator(token))
printf("%s --> Operator\n", token);
else if (isNumber(token))
printf("%s --> Number\n", token);
else if (isIdentifier(token))
printf("%s --> Identifier\n", token);
else
printf("%s --> Unknown Token\n", token);
token = strtok(NULL, " \t\n;,(){}");
}
}
fclose(fp);
return 0;
}
