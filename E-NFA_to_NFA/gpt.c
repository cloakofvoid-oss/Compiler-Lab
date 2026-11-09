#include <stdio.h>

int main()
{
    int n, i, j, k;
    int a[10][10][2] = {0};
    int e[10][10] = {0};
    int closure[10][10] = {0};
    int b[10][10][2] = {0};

    printf("Enter number of states: ");
    scanf("%d", &n);

    printf("Enter transitions (0 for a, 1 for b):\n");

    for(i = 0; i < n; i++)
        for(j = 0; j < n; j++)
        {
            printf("From %d to %d (a b): ", i, j);
            scanf("%d %d", &a[i][j][0], &a[i][j][1]);

            printf("From %d to %d (epsilon): ", i, j);
            scanf("%d", &e[i][j]);
        }

    // Find epsilon closure
    for(i = 0; i < n; i++)
    {
        closure[i][i] = 1;

        for(j = 0; j < n; j++)
            closure[i][j] = closure[i][j] || e[i][j];
    }

    // Transitive closure
    for(k = 0; k < n; k++)
        for(i = 0; i < n; i++)
            for(j = 0; j < n; j++)
                closure[i][j] = closure[i][j] ||
                                (closure[i][k] && closure[k][j]);

    // Remove epsilon transitions
    for(i = 0; i < n; i++)
        for(j = 0; j < n; j++)
            for(k = 0; k < n; k++)
                if(closure[i][k])
                {
                    b[i][j][0] |= a[k][j][0];
                    b[i][j][1] |= a[k][j][1];
                }

    printf("\nNFA without epsilon transitions:\n");

    for(i = 0; i < n; i++)
        for(j = 0; j < n; j++)
            if(b[i][j][0] || b[i][j][1])
                printf("q%d -> q%d : %s%s\n", i, j,
                       b[i][j][0] ? "a " : "",
                       b[i][j][1] ? "b" : "");

    return 0;
}
