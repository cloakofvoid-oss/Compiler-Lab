#include <stdio.h>
#include <stdlib.h>

int n;
int from[20], to[20];
int t;

struct Node
{
    int state;
    struct Node *next;
};

struct Node *head = NULL;

int exists(int state)
{
    struct Node *temp = head;

    while(temp != NULL)
    {
        if(temp->state == state)
            return 1;

        temp = temp->next;
    }

    return 0;
}

void addState(int state)
{
    struct Node *newNode;

    newNode = (struct Node *)malloc(sizeof(struct Node));

    newNode->state = state;
    newNode->next = head;
    head = newNode;
}

void findClosure(int state)
{
    int i;

    if(!exists(state))
        addState(state);

    for(i = 0; i < t; i++)
    {
        if(from[i] == state && !exists(to[i]))
        {
            findClosure(to[i]);
        }
    }
}

void clearList()
{
    struct Node *temp;

    while(head != NULL)
    {
        temp = head;
        head = head->next;
        free(temp);
    }
}

int main()
{
    int i;
    struct Node *temp;

    printf("Enter number of states: ");
    scanf("%d", &n);

    printf("Enter number of epsilon transitions: ");
    scanf("%d", &t);

    printf("Enter epsilon transitions:\n");

    for(i = 0; i < t; i++)
    {
        scanf("%d %d", &from[i], &to[i]);
    }

    printf("\nEpsilon Closures:\n");

    for(i = 0; i < n; i++)
    {
        head = NULL;

        findClosure(i);

        printf("Epsilon closure of q%d = { ", i);

        temp = head;

        while(temp != NULL)
        {
            printf("q%d ", temp->state);
            temp = temp->next;
        }

        printf("}\n");

        clearList();
    }

    return 0;
}
