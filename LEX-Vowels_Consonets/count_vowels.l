%{
#include <stdio.h>
int v = 0;
int c = 0;
%}

%%
[aAeEiIoOuU]                      { v++; }
[b-df-hj-np-tv-zB-DF-HJ-NP-TV-Z]  { c++; }
.|\n                              ; /* Ignore all other characters */

%%

int yywrap() {
    return 1;
}

int main(int argc, char *argv[]) {
    char filename[256];
    FILE *file = NULL;

    if (argc > 1) {
        // Use file name provided via command line argument
        file = fopen(argv[1], "r");
        if (!file) {
            printf("Error opening file: %s\n", argv[1]);
            return 1;
        }
    } else {
        // Prompt user for file name interactively
        printf("Enter the file name: ");
        if (scanf("%255s", filename) != 1) {
            printf("Error reading input.\n");
            return 1;
        }

        file = fopen(filename, "r");
        if (!file) {
            printf("Error opening file: %s\n", filename);
            return 1;
        }
    }

    yyin = file;
    yylex(); 

    printf("\n--- Results ---\n");
    printf("Vowels: %d\n", v);
    printf("Consonants: %d\n", c);

    fclose(file);
    return 0;
}
