#  Safe Fruit Project Report

## 1. Background Introduction

### 1.1 Project Reaction

Many dietary rules stipulate that certain fruits cannot be consumed together,as this can lead to adverse health effects (e.g., the conflict between bananas and cantaloupes).

Select the largest quantity of mutually non-conflicting fruits from a basket. If multiple sets have the maximum quantity, the secondary goal is to select the set with the minimum total price.

The input includes the number of conflicts $N$, the number of fruits $M$, a list of conflict pairs, and a list of fruit prices.The required output includes the maximum quantity, the list of fruit IDs, and the total price.

### 1.2 Problem Modeling and Model Transformation

Each fruit is abstracted as a vertex in a graph, and every conflicting pair of fruits is abstracted as an edge. Solving the "Maximum Safe Fruit Set" problem is transformed into solving the Maximum Independent Set (MIS) problem on the graph, with the additional optimization constraint of minimizing the total price. Given that the Maximum Independent Set problem is NP-Hard, an efficient Backtracking Search (Depth First Search) combined with pruning strategies is employed for the solution.

### 1.3 Computational Challenges and Solutions

For cases where the number of fruits $M$ is large (e.g., $M > 80$), the algorithm's runtime grows exponentially, making it difficult to find a solution within a short time. Therefore, more useful pruning and preprocessing techniques must be introduced to the basic backtracking search to significantly improve search efficiency.

## 2. Core Algorithm Design and Implementation

### 2.1 DFS-based Backtracking Framework

The problem is solved using a recursive Depth First Search (dfs) function, which implements the core backtracking logic to explore all possible independent sets.

The `dfs` function takes the size of the candidate set ($num$), the current search depth ($step$), the temporary solution path ($t$), and the global best answer ($ans$) as parameters. The $Stack[step]$ array stores the actual candidate fruit IDs for the current layer. The core logic involves a loop that iterates through the candidates $i=0$ to $num-1$. In each iteration, the fruit $k = Stack[step][i]$ is selected, added to the current path ($t$), and its price is accumulated. To prepare for the next step, a new candidate set is constructed in Stack[step+1] by including only those remaining candidates $j$ (where $j > i$) that do not conflict with the chosen fruit $k$ (i.e., $a[k][Stack[step][j]] == 0$). The function then recursively calls `dfs`, passing the count of these non-conflicting candidates ($cnt$) as the num argument. Finally, a standard backtracking step removes $k$ from the path ($t$) and reverts the price change.

The solution must satisfy two criteria: maximize quantity and minimize price. When a recursion branch terminates ($num == 0$), the temporary solution t is checked against the global best solution ans. The update rule prioritizes quantity: if $t.s > ans.s$, ans is updated. If $t.s == ans.s$, ans is only updated if $t.price < ans.price$. This check ensures the final result adheres to the strict output specification.

### 2.2 Key Algorithm Optimization: DP Preprocessing and Pruning

Due to the exponential nature of the Maximum Independent Set problem, two crucial pruning strategies are implemented within the dfs function to limit the search space.

A one-dimensional array dp is pre-computed in the main function iteratively from $M$ down to $1$. For a given fruit ID $i$, $dp[i]$ stores the length of the maximum independent set that can be formed using only fruits with ID greater than or equal to $i$. This value provides an aggressive upper bound on the potential solution size for any branch starting at fruit $i$.

Maximum Length Pruning: This is a basic pruning check applied at the start of the candidate loop. The condition if ($t->s + num - i < ans->s$) return; is used. If the size of the set currently selected ($t.s$) plus the total number of remaining candidates in the current layer ($num - i$) is less than the current optimal size ($ans.s$), the current search branch is guaranteed not to yield a better result, and the function immediately returns.

Precise Pruning based on DP Values: This is an pruning technique using the pre-calculated dp values. The condition checked is if ($t->s + dp[k] < ans->s$) return;. If the size of the set already chosen ($t.s$) combined with the absolute maximum possible size of any independent set starting from fruit $k$ ($dp[k]$) cannot surpass the current global best count, the branch is immediately terminated. This DP-based lookahead significantly enhances the effectiveness of the search.

## 3. Performance Testing and Results Analysis

### 3.1 Experimental Setup and Testing Methodology

Test cases were dynamically generated using the auxiliary program `project3_randominput.c`. This program ensures that conflict pairs ($N$) and fruit prices are randomly distributed within the specified constraints, providing a diverse set of scenarios for evaluation.

Testing focused on verifying the correctness of solutions across various scales. Critical test scales included regular-sized problems where $M \le 80$, and extreme cases designed to push the limits of the backtracking search, such as $M=85, 90, 95,$ and $100$.(The test cases (generated using `project3_randominput.c`) have been placed under `/test`.)

### 3.2 Results Presentation and Analysis

The analysis confirms that the output set is consistently the guaranteed unique optimal solution. This is verified by ensuring the set meets the strict criteria: (1) maximum possible fruit quantity, and (2) the lowest total price among all sets with that maximum quantity, validating the core optimization logic of the DFS update rule.

The analysis confirms the necessity of the DP preprocessing and pruning strategies. For large and complex test cases (e.g., $M=100$), the pruning is essential for reducing the search space to a feasible size, allowing the program to exhaustively search and find the unique optimal solution in a reasonable time. This successful completion of complex tests validates the crucial role of the optimization in making the exact solution possible.

## 4. Conclusion and Future Work

Summarize the project achievements, confirming the successful implementation of the MIS solver that satisfies the dual optimization objectives and demonstrates good performance on conventional scales ($M \le 80$).

Address the performance limitations for large $M$ by proposing further optimization directions, such as: introducing more sophisticated heuristic strategies, exploring branch and bound methods, or investigating approximation algorithms to find near-optimal solutions in less time.

## Appendix: Core Code Listing

Core Solver Code: `project3.c`

```c
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
```



Random Input Generation Tool: `project3_randominput.c`

``` 
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
```

