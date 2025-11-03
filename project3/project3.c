#include<stdio.h>
#include<stdlib.h>
#include<string.h>
int **a;
int *dp;
int *prices;   
int n,m,x,y;
//signal each fruit as a point,and our goal is to connect as many points into a path as possible 
struct answer{
    int s;//length of path
    int price;//total cost
    int list[101];//record the path
};
struct answer *ans;//the answer path
struct answer *t;// temp path
int Stack[102][102];//Stack[step][i] stores the candidate fruits for the [step]th layer

void dfs(int num, int step, struct answer *t, struct answer *ans)
{
    //num is the number of candidate fruits for the current layer, step is the current depth
    if(num == 0) 
    {
      if(t->s>ans->s||(t->s == ans->s&&t->price<ans->price))//update ans if "t" is better than current ans
      {
          ans->s = t->s;
          ans->price = t->price;
          memcpy(ans->list, t->list, sizeof(int)*(t->s + 1));
      }
      return;
    }
    //traverse the candidate fruit of the current layer
    for(int i = 0; i < num; i++)
    {
      int k = Stack[step][i];
      //even if all the fruits numbered from i to num are selected, they cannot surpass the best answer
      if(t->s+num-i<ans->s)
        return;
      //pruning 2: use dp value to prune more precisely
      if(t->s + dp[k]<ans->s)
        return;
      // build the candidate set for the next layer: select the fruits that do not conflict with k from the remaining candidates
      int cnt = 0;
      for(int j=i+1;j<num; j++)
      {
        if(a[k][Stack[step][j]] == 0)
          Stack[step+1][cnt++] = Stack[step][j];
      }
      
      // choose fruit k into t->list
      t->list[++t->s] = k;
      t->price += prices[k];
      
      // recursively search the next layer
      dfs(cnt, step+1, t, ans);
      
      //backtrack
      t->s--;
      t->price -= prices[k];
    }
}
int main()
{
  //initialization
    t=(struct answer*)malloc(sizeof(struct answer));
    ans=(struct answer*)malloc(sizeof(struct answer));
    scanf("%d%d",&n,&m);
    a = (int**)malloc(sizeof(int*)*(m+1));
    dp= (int *)malloc(sizeof(int)*(m+1));
    for(int i=0;i<=m;i++)
    {
        a[i]=(int*)malloc(sizeof(int)*(m+1));
    }
    for(int i=0;i<=m;i++)
      for(int j=0;j<=m;j++)
        a[i][j]=0;
    prices=(int*)malloc(sizeof(int)*(m+1));
    memset(t, 0, sizeof(struct answer));
    memset(ans, 0, sizeof(struct answer));
    for(int i=1;i<=n;i++)
    {
        scanf("%d%d",&x,&y);
        a[x][y]=a[y][x]=1;//x and y are in contradiction
    }
    for(int i=1;i<=m;i++)
    {
        scanf("%d%d",&x,&y);
        prices[x]=y;
    }
    //from back to front to calculate dp values
    for(int i=m;i>=1;i--)
      {
      // build the first layer candidate set: all fruits with ID greater than i
      int s=0;
      for(int j=i+1;j<=m;j++)
      {
      //  printf("a[%d][%d]=%d\n",i,j,a[i][j]);
        if(a[i][j]==0)
          Stack[2][s++]=j;
      }
      
      // choose fruit i as the starting point
      t->list[++t->s] = i;
      t->price+=prices[i];
      dfs(s,2,t,ans);
      // backtrack
      t->s--;
      t->price-=prices[i];
      dp[i] = ans->s;//record the max length of path after i-th point,in order for pruning
    }
    printf("%d\n",ans->s);
    printf("%03d",ans->list[1]);
    for(int i=2;i<=ans->s;i++)
      printf(" %03d",ans->list[i]);
    printf("\n%d",ans->price);
    free(t);
    free(ans);
    return 0;
}