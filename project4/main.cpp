// 说明 ：为了方便注释，nb == no beautiful

#include<iostream>
#include<algorithm>
#include<vector>
#include<iterator>

using namespace std;

const int MOD = 1e9 + 7;
// MOD : 模数

vector<vector<int>> F, T;
//int F[N][M], T[N][M];
// F[N][M] : 从第 N 个数开始长度为 M 的不可行数列数量
// T[N][M] : F[N][M] 的后缀和

int n, m, max_amount, ans;
vector<int> num;
// n : 数据个数
// m : 最大步长
// num[N] : 原始数据
// max_amount : 最大不可行数列长度
// ans : 答案


void init () {

    F.resize(n);
    T.resize(n);
    for (int i = 0; i <= n; i++) {
        F[i].resize(max_amount + 2, 0);
        T[i].resize(max_amount + 2, 0);
    }
    // 初始化两个数组， 第一维第 i 个数下标从 0 开始，方便处理；第二维数列长度 l 从 1 开始，1 是边界情况，实际主函数中从 2 开始

    for (int i = 0; i < n; i++) {
        F[i][1] = 1;
        T[i][1] = n - i;
    }
    // 根据定义，当数列长度为 1 时，必然不符合要求，属于nb，所以 F[i][1] 置为 1 ，T[i][1] 置为 n - i

}

// 因为 n_max == 1e5, 2^n_max肯定会超 int 爆掉，所以采用取模乘方的形式， 至于为什么可以在计算过程中取模在此不证明
int get_power_of_two (int k) {
    int sum = 1;
    while (k) {
        sum <<= 1;
        sum = sum % MOD;
        k--;
    }
    return sum;
}

int main() {
    // 加速一下输入输出
    ios::sync_with_stdio(0);
    cin.tie(0);

    // 读取 + 排序，方便后面二分查找（upper_bound）
    cin >> n >> m;
    num.resize(n, 0);
    for (int i = 0; i < n; i++) {
        cin >> num[i];
    }
    sort(num.begin(), num.end());

    // 先计算所有可能的数列数量
    // 从组合数导出，一个长度为 n 的数列，有多少个非空子序列？答案是 2^n - 1 个，如果再去掉所有长度为 1 的子序列，那就是一共有 2^n - n - 1个可能的子序列
    ans = (get_power_of_two(n) - n - 1) % MOD;

    // 再计算最大数和最小数之差，如果这个差小于等于最大步长，那么所有的长度为 2 的数列都是有效的，进而可以推出所有任意长度（2 ~ n）都是有效的，所以一开始算出的ans就是最终解，直接输出
    int delta_max = num.back() - num.front();
    // 最大 nb 数列长度计算
    max_amount = delta_max / m + 1;

    if (delta_max <= m) {
        cout << ans;
        return 0;
    }

    init();

    // 先获取一下最大值方便后面反复调用（虽然可能也没有节约什么时间消耗就是了）
    int num_max = num.back();
    // 到了核心部分了，动规计算比模拟好的一点就是可以压一下复杂度，不需要暴力遍历所有可能性，代价就是我的头发（
    // 我们在初始化中已经初始化了 l = 1 的情况，所以这里直接从 l = 2 开始，也就是计算有两个数的 nb数列数量，最大值在前面计算过了
    for (int l = 2; l <= max_amount; l++) {
        // 如果只是维护 F 的话，什么顺序都可以；but 由于我们要维护后缀和二维向量 T ，所以要倒序
        for (int i = n - 1; i >= 0; i--) {
            int i_next;
            // i_next 表示从第 i 个数往后不小于 num[i] + m，也就是不在步长m范围内的第一个数的索引，如果 num[i] + m 超出了最大值，那么我们就取 n （实际上的第 n + 1个数，不存在）作为哨兵来计算 F，可以避免一些麻烦的处理
            if (num_max <= num[i] + m) {
                i_next = n;
            }else {
                //  num[i] + m 在数组范围内，直接查找，不可能出现越界问题（也就是 num.end() 指向 n 的位置）
                i_next = distance(num.begin(), upper_bound(num.begin() + i + 1, num.end(), num[i] + m));
            }
            // 最最核心的两句话！！！
            // 第 i 个数能够形成的长度为 l 的 nb 数列等于第 i_next 个数到第 n - 1 个数能够形成的长度为 l - 1 的 nb 数列
            // 例如，3 5 6 8（m = 2），3（第 0 个数）能够形成的长度为 2 的 nb 数列等于 6 到 8（> 3 + m）能够形成的长度为 1 的 nb 数列之和
            F[i][l] = T[i_next][l - 1];
            // 维护后缀和
            T[i][l] = (T[i + 1][l] + F[i][l]) % MOD;
        }
    }

    // 这里从所有可能答案中去掉所有长度（从 2 到 max_amount）的 nb 序列，这里后缀和也能帮助我们简化处理，T[0][l] 就是长度为 l 的所有 nb 序列数量之和了
    for (int l = 2; l <= max_amount; l++) {
        ans = (ans - T[0][l]) % MOD;
    }

    // 输出结果
    cout << ans;

    /*
    总体来看，这个程序的时间复杂度取决于那个double loop（双重循环），根据循环的界，我们可以得到这个程序的时间复杂度为 O(n^2/m)
    如果是极限情况（n == 1e5, m == 1）的话，这个时间复杂度就达到了可怕的 1e10
    但我认为这已经是很好的优化结果了，毕竟如果纯粹暴力（不做任何预处理的情况下）那个 n/m 应该在指数的位置上，那就坏太多了
    */

    return 0;
}