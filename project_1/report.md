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
## 2.1 Experiments Results
### 2.1.1 Unbalanced Binary Search Tree
- show the results...
- Embed graphs
### 2.1.2 AVL Tree

### 2.1.3 Splay Tree

## 2.2 Performance Evaluation & Analysis

# 3. Conclusions
- There are many methods to address tree imbalance and inefficiencies in different operations. In our experiment, AVL Trees and Splay Trees show better performance compared to unbalanced binary search trees.
- However, each method has disadvantages. For instance, AVL Trees require frequent rotations to maintain balance, which means the time cost can be considerable when  there are multiple insertions or deletions. Splay Trees, while efficient in some cases, may have poor worst-case performance when elements are inserted in a specific order (increasing order for example).
- Therefore, no single algorithm is perfect for all cases. The choice of data structure should depend on the specific use case, and it's essential to understand the problem features to select the most suitable balancing strategy.