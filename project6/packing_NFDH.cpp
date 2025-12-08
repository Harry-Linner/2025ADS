#include <iostream>
#include <vector>
#include <algorithm>
#include <ctime>
#include <iomanip>
using namespace std;

struct Rectangle {
    int id;
    int width;
    int height;
};

bool cmpHeight(const Rectangle &a, const Rectangle &b) {
    if (a.height != b.height) return a.height > b.height;
    return a.width > b.width;
}

int main() 
{
    int binWidth, n;
    cin >> binWidth >> n;
    
    vector<Rectangle> rects;
    for (int i = 0; i < n; ++i) {
        int w, h;
        cin >> w >> h;
        rects.push_back({i + 1, w, h});
    }
    
    // Step 1: 按高度降序排序
    sort(rects.begin(), rects.end(), cmpHeight);
    
    clock_t start = clock();
    
    // NFDH 核心：Next Fit（只关注当前货架）
    int currentShelfUsedWidth = 0;   // 当前货架已用宽度
    int currentShelfHeight = 0;      // 当前货架高度
    int totalHeight = 0;             // 总高度
    int shelfCount = 0;              // 货架数量
    
    for (size_t i = 0; i < rects.size(); ++i) {
        Rectangle &r = rects[i];
        
        // Next Fit: 只尝试放入当前货架
        if (currentShelfUsedWidth + r.width <= binWidth) {
            // 可以放入当前货架
            currentShelfUsedWidth += r.width;
            currentShelfHeight = max(currentShelfHeight, r.height);
        } else {
            // 当前货架放不下，结束当前货架，开启新货架
            if (currentShelfHeight > 0) {
                totalHeight += currentShelfHeight;
                shelfCount++;
            }
            // 开启新货架，放入当前矩形
            currentShelfUsedWidth = r.width;
            currentShelfHeight = r.height;
        }
    }
    
    // 处理最后一个货架
    if (currentShelfHeight > 0) {
        totalHeight += currentShelfHeight;
        shelfCount++;
    }
    
    clock_t end = clock();
    double timeTaken = double(end - start) / CLOCKS_PER_SEC * 1000.0;
    
    // 输出结果
    cout << totalHeight << endl;
    
    return 0;
}

