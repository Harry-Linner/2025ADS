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