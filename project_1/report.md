# 1. Background Introduction
## 1.1 Binary Search Trees
- *Binary Search Trees* include a wide variety of algorithms whose implementations share a common feature: for a given node, all values in the left subtree are less than the node's value, and all values in the right subtree are greater than the node's value.
- There are various strategies for manipulating elements, and for restructuring the tree to improve overall performance.
## 1.2 Existing Problems
- If we use unbalanced binary search trees, the tree may degenerate into a structure resembling a linked list under certain insertion orders(increasing order for example), resulting in $O(N)$ time complexity in the worst case.
- This leads to unstable performance across different input patterns, which is undesirable for efficient data storage.
## 1.3 Possible Solutions
- To ensure performance stability, we aim to maintain the height of the tree within $O(\log N)$.
- Using basic operations such as `leftRotate()` and `rightRotate()`, balancing methods like these can be implemented to solve tree imbalance and preserve efficient operations.

# 2. Experiments and Performance Evaluation
## 2.1 Experiments Procedure
- We conducted experiments on three types of binary search trees: Unbalanced Binary Search Trees, AVL trees, and Splay trees.
- Firstly, we programmed several functions to perform insertions and deletions. "Insert"、"delete" are core functions for each,"rebalanceAVL"、"leftrotate"、"rightrotate" for AVL tree ,and "splay"、"leftrotate"、"rightrotate" for Splay trees.And then we used the "time" function to measure the execution time of each operation."Main.c" combines all the functions into one program,serving as the entry point of our program.Finally, we used "run_analyzer.py" to get the results.
- For each tree type, we performed insertions and deletions.Three types of orders are considered: "inc" for increasing order, "dec" for decreasing order, and "rand" for random order.We achieve this by reversing and shuffling the input data in "main.c".## 2.1 Experiments Results
-Considering that AVL Tree and Splay Tree both perform well with short execution time of insertions and deletions,we repeated the experiments 100 times for each tree type,in order to get a more accurate result.
Here are the results of our experiments,for deletion type "inc","dec" and "rand":
## 测试结果表格
####  Incremental Data Performance
| 数据规模(N) | BST运行时间(s) | AVL运行时间(s) | Splay运行时间(s) |
|---|---|---|---|
| 1000 | 0.361000 | 0.048000 | 0.020000 |
| 2000 | 1.403000 | 0.107000 | 0.034000 |
| 3000 | 3.354000 | 0.158000 | 0.061000 |
| 4000 | 5.872000 | 0.225000 | 0.091000 |
| 5000 | 9.061000 | 0.280000 | 0.103000 |
| 6000 | 13.907000 | 0.330000 | 0.127000 |
| 7000 | 19.189000 | 0.419000 | 0.143000 |
| 8000 | 24.591000 | 0.426000 | 0.151000 |
| 9000 | 30.795000 | 0.506000 | 0.168000 |
| 10000 | 38.366000 | 0.587000 | 0.175000 |
####  Decremental Data Performance
| 数据规模(N) | BST运行时间(s) | AVL运行时间(s) | Splay运行时间(s) |
|---|---|---|---|
| 1000 | 0.793000 | 0.054000 | 0.014000 |
| 2000 | 3.230000 | 0.106000 | 0.029000 |
| 3000 | 7.551000 | 0.169000 | 0.046000 |
| 4000 | 13.987000 | 0.252000 | 0.075000 |
| 5000 | 21.594000 | 0.290000 | 0.070000 |
| 6000 | 32.182000 | 0.408000 | 0.111000 |
| 7000 | 43.014000 | 0.429000 | 0.118000 |
| 8000 | 56.953000 | 0.480000 | 0.140000 |
| 9000 | 69.678000 | 0.574000 | 0.141000 |
| 10000 | 87.619000 | 0.645000 | 0.181000 |
####  Random Data Performance
| 数据规模(N) | BST运行时间(s) | AVL运行时间(s) | Splay运行时间(s) |
|---|---|---|---|
| 1000 | 0.018000 | 0.050000 | 0.044000 |
| 2000 | 0.063000 | 0.145000 | 0.111000 |
| 3000 | 0.117000 | 0.240000 | 0.174000 |
| 4000 | 0.148000 | 0.333000 | 0.249000 |
| 5000 | 0.206000 | 0.414000 | 0.311000 |
| 6000 | 0.264000 | 0.556000 | 0.401000 |
| 7000 | 0.293000 | 0.597000 | 0.471000 |
| 8000 | 0.355000 | 0.697000 | 0.555000 |
| 9000 | 0.418000 | 0.817000 | 0.648000 |
| 10000 | 0.496000 | 0.894000 | 0.689000 |
![alt text](image-1.png)
![alt text](image-2.png)
![alt text](image-3.png)
### 2.2.1 Unbalanced Binary Search Tree
- Unbalanced Binary Search Trees have poor performance in many scenarios, especially when there are many insertions or deletions.
### 2.2.2 AVL Tree

### 2.2.3 Splay Tree

## 2.3 Performance Evaluation & Analysis
####  Incremental Data Performance
- BST performs terribly bad. With incremental data,this tree has degenerated into a linear chain.The time complexity of every insertion or deletion is O(n),so totally its complexity is O(n²).
- For AVL tree and Splay tree,the time complexity of every insertion or deletion is O(log n),so totally its complexity is O(n log n).
- Splay tree consistently performs better than AVL tree..
- Analysis: 
Splay tree Advantages compared with AVL tree:
For monotonic data,insertions are concentrated on the left side of the tree,requiring fewer rotations and resulting in faster speed.AVL tree need frequent rotations to maintain balance,requiring more time. 


####  Decremental Data Performance
- Nearly the same as Incremental Data Performance
- The decremental data of deletion makes Splay tree even more faster,because the tree just need to delete the root in every operaion.
####  Random Data Performance
- All types of trees perform well,in which BST performs pretty excellent ,better than balanced trees.
- Splay Tree performance is intermediate between BST and AVL
- BST achieves its theoretical O(n log n) complexity
- Analysis
 Due to the randomness of the data, BST trees do not degenerate, so their simplicity makes them the fastest. Rules of AVL trees are the most strict.So AVL trees have the most frequent rotation operations. Splay have less rotation operations. Therefore, the required time for insertion and deletion: AVL > Splay > BST.
**Theoretical vs. Observed Complexity:**

| Tree Type | Data Distribution | Theoretical | Observed Behavior |
|-----------|------------------|-------------|-------------------|
| BST       | Random          | O(n log n)  | ✅ O(n log n)     |
| BST       | Ordered         | O(n²)       | ✅ O(n²)          |
| AVL       | All             | O(n log n)  | ✅ O(n log n)     |
| Splay     | All             | O(n log n) | ✅ O(n log n)     |
# 3. Conclusions
- There are many methods to address tree imbalance and inefficiencies in different operations. In our experiment, AVL Trees and Splay Trees show better performance compared to unbalanced binary search trees.
- However, each method has disadvantages. For instance, AVL Trees require frequent rotations to maintain balance, which means the time cost can be considerable when  there are multiple insertions or deletions. Splay Trees, while efficient in some cases, may have poor worst-case performance when elements are inserted in a specific order (increasing order for example).
- Therefore, no single algorithm is perfect for all cases. The choice of data structure should depend on the specific use case, and it's essential to understand the problem features to select the most suitable balancing strategy.
