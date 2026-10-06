#include <stdio.h>
#include <string.h>

int n;
char from[20], symbol[20], to[20];

int closure[10][10];

void findClosure(int state)
{
    int i, next;
    closure[state][state] = 1;

    for(i = 0; i < n; i++)
    {
        if(from[i] - '0' == state && symbol[i] == '@')
        {
            next = to[i] - '0';

            if(closure[state][next] == 0)
            {
                closure[state][next] = 1;
                findClosure(next);
            }
        }
    }
}

int main()
{
    int states, i, j, k;
    char ch;

    printf("Enter number of states: ");
    scanf("%d", &states);

    printf("Enter number of transitions: ");
    scanf("%d", &n);

    printf("Enter transitions (from symbol to):\n");
    printf("Use @ for epsilon\n");

    for(i = 0; i < n; i++)
    {
        scanf("%c", &ch);
        scanf("%c %c %c", &from[i], &symbol[i], &to[i]);
    }

    for(i = 0; i < states; i++)
    {
        findClosure(i);
    }

    printf("\nEpsilon Closures:\n");

    for(i = 0; i < states; i++)
    {
        printf("E-closure(q%d) = { ", i);

        for(j = 0; j < states; j++)
        {
            if(closure[i][j] == 1)
                printf("q%d ", j);
        }

        printf("}\n");
    }

    printf("\nNFA transitions:\n");

    for(i = 0; i < states; i++)
    {
        for(ch = 'a'; ch <= 'z'; ch++)
        {
            int result[10] = {0};
            int found = 0;

            /* Step 1: epsilon closure of q_i */
            for(j = 0; j < states; j++)
            {
                if(closure[i][j] == 1)
                {
                    /* Step 2: take transition on symbol */
                    for(k = 0; k < n; k++)
                    {
                        if(from[k] - '0' == j &&
                           symbol[k] == ch)
                        {
                            int next = to[k] - '0';

                            /* Step 3: epsilon closure of destination */
                            int x;

                            for(x = 0; x < states; x++)
                            {
                                if(closure[next][x] == 1)
                                {
                                    result[x] = 1;
                                    found = 1;
                                }
                            }
                        }
                    }
                }
            }

            if(found)
            {
                printf("q%d --%c--> { ", i, ch);

                for(j = 0; j < states; j++)
                {
                    if(result[j] == 1)
                        printf("q%d ", j);
                }

                printf("}\n");
            }
        }
    }

    return 0;
}
