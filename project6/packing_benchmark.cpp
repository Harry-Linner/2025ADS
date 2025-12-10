#include <iostream>
#include <vector>
#include <algorithm>
#include <chrono>
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

// NFDH算法：Next Fit Decreasing Height
int nfdh(vector<Rectangle> rects, int binWidth) {
    sort(rects.begin(), rects.end(), cmpHeight);
    
    int currentShelfUsedWidth = 0;
    int currentShelfHeight = 0;
    int totalHeight = 0;
    
    for (size_t i = 0; i < rects.size(); ++i) {
        Rectangle &r = rects[i];
        
        if (currentShelfUsedWidth + r.width <= binWidth) {
            currentShelfUsedWidth += r.width;
            currentShelfHeight = max(currentShelfHeight, r.height);
        } else {
            if (currentShelfHeight > 0) {
                totalHeight += currentShelfHeight;
            }
            currentShelfUsedWidth = r.width;
            currentShelfHeight = r.height;
        }
    }
    
    if (currentShelfHeight > 0) {
        totalHeight += currentShelfHeight;
    }
    
    return totalHeight;
}

// FFDH算法：First Fit Decreasing Height
int ffdh(vector<Rectangle> rects, int binWidth) {
    sort(rects.begin(), rects.end(), cmpHeight);
    
    // 货架列表：每个货架记录高度和已用宽度
    vector<pair<int, int>> shelves; // (height, usedWidth)
    
    for (size_t i = 0; i < rects.size(); ++i) {
        Rectangle &r = rects[i];
        
        bool placed = false;
        
        // First Fit: 从第一个货架开始找能放下的
        for (size_t j = 0; j < shelves.size(); ++j) {
            if (shelves[j].second + r.width <= binWidth) {
                shelves[j].second += r.width;
                shelves[j].first = max(shelves[j].first, r.height);
                placed = true;
                break;
            }
        }
        
        // 所有货架都放不下，创建新货架
        if (!placed) {
            shelves.push_back({r.height, r.width});
        }
    }
    
    // 计算总高度
    int totalHeight = 0;
    for (size_t i = 0; i < shelves.size(); ++i) {
        totalHeight += shelves[i].first;
    }
    
    return totalHeight;
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
    
    // 运行NFDH算法 - 使用高精度计时
    auto start_nfdh = chrono::high_resolution_clock::now();
    int heightNFDH = nfdh(rects, binWidth);
    auto end_nfdh = chrono::high_resolution_clock::now();
    chrono::duration<double, milli> timeNFDH = end_nfdh - start_nfdh;
    
    // 运行FFDH算法 - 使用高精度计时
    auto start_ffdh = chrono::high_resolution_clock::now();
    int heightFFDH = ffdh(rects, binWidth);
    auto end_ffdh = chrono::high_resolution_clock::now();
    chrono::duration<double, milli> timeFFDH = end_ffdh - start_ffdh;
    
    // 输出结果 - 6位小数精度
    cout << "NFDH: " << heightNFDH << " " << fixed << setprecision(6) << timeNFDH.count() << endl;
    cout << "FFDH: " << heightFFDH << " " << fixed << setprecision(6) << timeFFDH.count() << endl;
    
    return 0;
}
