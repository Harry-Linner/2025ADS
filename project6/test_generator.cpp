#include <iostream>
#include <fstream>
#include <cstdlib>
#include <ctime>
#include <vector>
#include <string>

using namespace std;

int randomInt(int min, int max) {
    return min + rand() % (max - min + 1);
}

int main(int argc, char* argv[]) {
    srand(time(NULL));
    
    cout << "Texture Packing Test Data Generator" << endl;
    cout << "===================================" << endl << endl;
    
    // 不同的binWidth测试
    vector<int> binWidths = {1000, 2000, 5000};
    
    // 更大规模的测试用例
    vector<int> sizes = {1000, 3000, 5000, 8000, 10000, 30000, 50000};
    
    int fileCount = 0;
    
    for (int binWidth : binWidths) {
        for (int n : sizes) {
            string filename = "test_cases/test_w" + to_string(binWidth) + "_n" + to_string(n) + ".txt";
            ofstream outFile(filename);
            
            if (!outFile) {
                cerr << "Error: Cannot create file " << filename << endl;
                continue;
            }
            
            // 输出格式：binWidth n
            outFile << binWidth << " " << n << endl;
            
            // 生成n个矩形，每行：width height
            // 矩形宽度范围：10到binWidth的60%（确保大部分能放入）
            int maxRectWidth = binWidth * 60 / 100;
            
            for (int i = 0; i < n; ++i) {
                int width = randomInt(10, maxRectWidth);
                int height = randomInt(10, 500);
                outFile << width << " " << height << endl;
            }
            
            outFile.close();
            fileCount++;
            cout << "Generated: " << filename << " (binWidth=" << binWidth << ", n=" << n << ")" << endl;
        }
        cout << "---" << endl;
    }
    
    cout << "\nAll test cases generated successfully!" << endl;
    cout << "Total files: " << fileCount << endl;
    
    return 0;
}
