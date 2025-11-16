#include<iostream>
#include<algorithm>
#include<vector>
#include<iterator>

using namespace std;


const int N = 1e5 + 10, M = 1e5 + 10;
// N > n_max, M > n_max / m_min
const int MOD = 1e9 + 7;
// MOD : 模数

vector<vector<int>> F, T;

//int F[N][M], T[N][M];
// F[N][M] : 从第 N 个数开始长度为 M 的不可行数列数量
// T[N][M] : F[N][M] 的后缀和

int n, m;
vector<int> num;
// n : 数据个数
// m : 最大步长
// num[] : 原始数据
int max_amount, ans;
// max_amount : 最大不可行数列长度
// ans : 答案


void init () {

    F.resize(n + 1);
    T.resize(n + 1);
    for (int i = 0; i <= n; i++) {
        F[i].resize(max_amount + 2, 0);
        T[i].resize(max_amount + 2, 0);
    }

    for (int i = 0; i < n; i++) {
        F[i][1] = 1;
        T[i][1] = n - i;
    }

}

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
    ios::sync_with_stdio(0);
    cin.tie(0);

    cin >> n >> m;
    num.resize(n, 0);
    for (int i = 0; i < n; i++) {
        cin >> num[i];
    }
    sort(num.begin(), num.end());

    ans = (get_power_of_two(n) - n - 1) % MOD;

    int delta_max = num.back() - num.front();
    max_amount = delta_max / m + 1;

    if (delta_max <= m) {
        cout << ans;
        return 0;
    }

    init();

    int num_max = num.back();
    for (int l = 2; l <= max_amount; l++) {
        for (int i = n - 1; i >= 0; i--) {
            int i_next;
            if (num_max <= num[i] + m) {
                i_next = n;
            }else {
                i_next = distance(num.begin(), upper_bound(num.begin() + i + 1, num.end(), num[i] + m));
            }
            F[i][l] = T[i_next][l - 1];
            T[i][l] = (T[i + 1][l] + F[i][l]) % MOD;
        }
    }


    for (int l = 2; l <= max_amount; l++) {
        ans = (ans - T[0][l]) % MOD;
    }

    cout << ans;

    return 0;
}