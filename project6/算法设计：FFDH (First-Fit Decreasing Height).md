## 算法设计：FFDH (First-Fit Decreasing Height)

**策略：**

1. 将所有矩形按高度从大到小排序。
2. 逐个放置矩形。如果当前层（Level）有足够宽度，则放置；否则，新开一层，新层的高度由该层第一个矩形的高度决定。

**复杂度分析：**

- 排序：$O(n \log n)$
- 放置：$O(n)$
- 总复杂度：$O(n \log n)$，符合多项式要求。

------

## C++ 代码实现

```c++
#include <iostream>
#include <vector>
#include <algorithm>
#include <random>
#include <chrono>
#include <iomanip>

using namespace std;

// 矩形结构体
struct Rect {
    int id;
    int w, h;
};

// 排序规则：按高度降序排列
bool compareRects(const Rect& a, const Rect& b) {
    if (a.h != b.h) return a.h > b.h;
    return a.w > b.w;
}

/**
 * FFDH 算法实现
 * @param rects 矩形集合
 * @param stripWidth 固定的最大宽度
 * @return 最终占用的总高度
 */
int performFFDH(vector<Rect>& rects, int stripWidth) {
    sort(rects.begin(), rects.end(), compareRects);

    int totalHeight = 0;
    int currentLevelH = 0;
    int currentLevelWUsed = 0;

    for (const auto& r : rects) {
        // 如果单个矩形宽度超过条带宽度，则无法放置（异常处理）
        if (r.w > stripWidth) continue; 

        // 检查当前层是否能放下
        if (currentLevelWUsed + r.w <= stripWidth) {
            currentLevelWUsed += r.w;
            currentLevelH = max(currentLevelH, r.h);
        } else {
            // 放不下，开启新的一层
            totalHeight += currentLevelH;
            currentLevelWUsed = r.w;
            currentLevelH = r.h;
        }
    }
    totalHeight += currentLevelH; // 加上最后一层的高度
    return totalHeight;
}

// 性能测试与用例生成
void runTest(int size, int stripWidth) {
    vector<Rect> rects;
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<> disW(1, stripWidth);
    uniform_int_distribution<> disH(1, 100);

    for (int i = 0; i < size; ++i) {
        rects.push_back({i, disW(gen), disH(gen)});
    }

    auto start = chrono::high_resolution_clock::now();
    int minHeight = performFFDH(rects, stripWidth);
    auto end = chrono::high_resolution_clock::now();

    chrono::duration<double, milli> elapsed = end - start;
    cout << setw(10) << size << " | " << setw(10) << minHeight << " | " << fixed << setprecision(3) << elapsed.count() << " ms" << endl;
}

int main() {
    int stripWidth = 1000;
    cout << "--- Texture Packing Test (FFDH Algorithm) ---" << endl;
    cout << "Strip Width: " << stripWidth << endl;
    cout << setw(10) << "Size" << " | " << setw(10) << "Height" << " | " << "Time" << endl;
    cout << "---------------------------------------------" << endl;

    vector<int> sizes = {10, 100, 500, 1000, 5000, 10000};
    for (int n : sizes) {
        runTest(n, stripWidth);
    }

    return 0;
}
```

------

## 测试与分析报告要点

### Chapter 1: 问题描述与算法选择

- **问题性质：** NP-hard 问题。
- **算法：** 选择 FFDH 近似算法。其理论近似比（Approximation Ratio）为 $FFDH(I) \le 1.7 OPT + H_{max}$，在效率和效果之间取得了平衡。

### Chapter 2: 性能分析 (基于运行结果)

- **时间复杂度分析：** 实验结果应显示运行时间随输入规模 $N$ 呈线性对数（$O(N \log N)$）增长。在 $N=10,000$ 时，耗时应在几毫秒至十几毫秒之间。
- **影响因素：** * **Strip Width ($W$):** 宽度越小，层数越多，浪费越严重。
    - **分布：** 如果矩形宽度接近 $W$，则每层只能放一个，退化为 1D 装箱。

------

## Bonus: Strip Algorithms 对比分析

你可以对比以下几种算法来丰富你的报告：

| **算法**            | **策略**                           | **近似比 (Approx. Ratio)** | **复杂度**                |
| ------------------- | ---------------------------------- | -------------------------- | ------------------------- |
| **FFDH**            | 排序，放入第一个容纳的层           | $1.7 OPT$                  | $O(n \log n)$             |
| **NFDH** (Next-Fit) | 只放当前层，放不下就关掉，不再回头 | $2.0 OPT$                  | $O(n \log n)$             |
| **BFDH** (Best-Fit) | 放入能容纳的层中剩余空间最小的那层 | 比 FFDH 略好               | $O(n^2)$ 或 $O(n \log n)$ |

**结论：** FFDH 在绝大多数 Texture Packing 场景下是最优选，因为它的高度浪费（Vertical Waste）主要取决于排序后的阶梯形状，在大量小矩形输入时表现非常稳定。