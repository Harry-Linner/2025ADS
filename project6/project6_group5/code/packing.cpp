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
// the struct to store the features of rectangle

bool cmpHeight(const Rectangle &a, const Rectangle &b) {
    if (a.height != b.height) return a.height > b.height;
    return a.width > b.width;
    // Helper function to compare the heights of rectangles
}

// NFDH Algorithm: Next Fit Decreasing Height
int nfdh(vector<Rectangle> &rects, int binWidth) {
    sort(rects.begin(), rects.end(), cmpHeight);
    
    int currentShelfUsedWidth = 0;
    int currentShelfHeight = 0;
    int totalHeight = 0;
    
    for (size_t i = 0; i < rects.size(); ++i) {
        Rectangle &r = rects[i];
        
        if (currentShelfUsedWidth + r.width <= binWidth) {
            currentShelfUsedWidth += r.width;
            currentShelfHeight = max(currentShelfHeight, r.height);
            // if the current rectangle can fit the width, then fit it and update our height
        } else {
            if (currentShelfHeight > 0) {
                totalHeight += currentShelfHeight;
                // we need to add another shelve
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

// FFDH: First Fit Decreasing Height
int ffdh(vector<Rectangle> &rects, int binWidth) {
    sort(rects.begin(), rects.end(), cmpHeight);
    
    // the array to store shelves
    vector<pair<int, int>> shelves; // (height, usedWidth)
    
    for (size_t i = 0; i < rects.size(); ++i) {
        Rectangle &r = rects[i];
        
        bool placed = false;
        
        // First Fit: we start from the first shelve to find the first fitting position
        for (size_t j = 0; j < shelves.size(); ++j) {
            if (shelves[j].second + r.width <= binWidth) {
                // this means that we can fit in the current shelve
                shelves[j].second += r.width;
                shelves[j].first = max(shelves[j].first, r.height);
                //update the 
                placed = true;
                break;
            }
        }
        
        // if it cant be placed now, then we need to create another shelve
        if (!placed) {
            shelves.push_back({r.height, r.width});
        }
    }
    
    // calculate the total height
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
    
    clock_t start, end;
    
    // execute the NFDH Algorithm
    vector<Rectangle> rectsNFDH = rects;
    start = clock();
    int heightNFDH = nfdh(rectsNFDH, binWidth);
    end = clock();
    double timeNFDH = double(end - start) / CLOCKS_PER_SEC * 1000.0;
    
    // execute the FFDH Algorithm
    vector<Rectangle> rectsFFDH = rects;
    start = clock();
    int heightFFDH = ffdh(rectsFFDH, binWidth);
    end = clock();
    double timeFFDH = double(end - start) / CLOCKS_PER_SEC * 1000.0;
    
    // Output the results
    cout << "NFDH: " << heightNFDH << " (Time: " << fixed << setprecision(6) << timeNFDH << " ms)" << endl;
    cout << "FFDH: " << heightFFDH << " (Time: " << fixed << setprecision(6) << timeFFDH << " ms)" << endl;
    
    return 0;
}