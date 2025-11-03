#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>    
int conflicts[100][2];
int prices[101];
int random_int(int min, int max) {
    return min + rand() % (max - min + 1);
}
int conflict_exists(int conflicts[][2], int count, int a, int b) 
{
    for (int i=0;i<count;i++)
        if((conflicts[i][0]==a && conflicts[i][1]==b) ||
            (conflicts[i][0]==b && conflicts[i][1]==a))
            return 1;
    return 0;
}

int main() 
{
    int N,M;
    srand(time(NULL));
    scanf("%d %d", &N, &M);
    int conflict_count=0;
    while(conflict_count<N) 
    {
        int a=random_int(1,M);
        int b=random_int(1,M);
        if(a!=b&&!conflict_exists(conflicts,conflict_count,a,b)) 
        {
            conflicts[conflict_count][0]=a;
            conflicts[conflict_count][1]=b;
            conflict_count++;
        }
    }
    for(int i=1;i<= M;i++)
        prices[i]=random_int(1,1000);
    printf("%d %d\n",N,M);
    for (int i=0;i<N;i++)
        printf("%03d %03d\n",conflicts[i][0],conflicts[i][1]);
    for (int i=1;i<=M;i++)
        printf("%03d %d\n",i,prices[i]);
    return 0;
}