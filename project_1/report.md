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
