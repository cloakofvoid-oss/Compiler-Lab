#include <stdio.h>

int main(){
    int n,i,j,k;
    int a[10][10][2] ={0};
    int e[10][10]={0};
    int cl[10][10]={0};
    int b[10][10][2]={0};

    printf("Enter the no of states:");
    scanf("%d", &n);
    for(i=0;i<n;i++){
        for(j=0;j<n;j++){
            printf("From %d to %d (a b): ", i, j);
            scanf("%d %d", &a[i][j][0], &a[i][j][1]);
            
            printf("From %d to %d (epsilon): ", i, j);
            scanf("%d", &e[i][j]);
        } 
    }
    //finde closure 
    for(i=0;i<n;i++){
        cl[i][j]=1;
        for(j=0;j<n;j++){
            cl[i][j] = cl[i][j] || e[i][j];
        
        }
    
    } 
    
    //e-closure
     for(k = 0; k < n; k++)
        for(i = 0; i < n; i++)
            for(j = 0; j < n; j++){
                cl[i][j] = cl[i][j] || (cl[i][k] &&cl[k][j]);
            }
            
    printf("\nEpsilon Closure:\n");
    for(i = 0; i < n; i++) {
        printf("E(q%d) = { ", i);
        for(j = 0; j < n; j++)
            if(cl[i][j])
                printf("q%d ", j);

        printf("}\n");
    }
    
   for(i = 0; i < n; i++)
        for(j = 0; j < n; j++)
            for(k = 0; k < n; k++)
                if(cl[i][k])
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
