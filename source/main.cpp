#include <iostream>
#include <vector>
using namespace std;
const long long oo = (long long)1e18 + 5; // Vo cung lon
struct BinarySearchTree {
    int n, root; // So nut cua cay va nut goc cua cay nhi phan
    vector<int> key; // key[i]: khoa cua nut i
    vector<int> left, right; // left[i]: nut con ben trai i va ke voi i, right[i] tuong tu
    BinarySearchTree() {}
};
long long dfs(int u, int h, const BinarySearchTree &t, const vector<int> &freq) {
    // Tinh tong freq[u] * h[u] trong cay tim kiem nhi phan t
    long long sum = 1LL * freq[u] * h;
    if (t.left[u] != -1) sum += dfs(t.left[u], h + 1, t, freq);
    if (t.right[u] != -1) sum += dfs(t.right[u], h + 1, t, freq);
    return sum;
}
long long cost(const BinarySearchTree &t, const vector<int> &freq) {
    // Tinh tong chi phi truy cap cua cay tim kiem nhi phan t khi biet so lan truy cap vao cac khoa
    return dfs(t.root, 1, t, freq);
}
int trace(int l, int r, const vector<vector<int> > &opt, vector<int> &left, vector<int> &right) {
    // Truy vet mot cay nhi phan toi uu gom cac nut trong doan [l, r] luu vao left, right va tra ve nut goc
    if (l > r) return -1;
    int i = opt[l][r];
    left[i] = trace(l, i - 1, opt, left, right);
    right[i] = trace(i + 1, r, opt, left, right);
    return i;
}
BinarySearchTree findOptimalBinarySearchTree(int n, const vector<int> &key, const vector<int> &freq) {
    vector<vector<long long> > d(n, vector<long long>(n, 0)); // Bang quy hoach dong kich thuoc nxn
    vector<vector<int> > opt(n, vector<int>(n, 0)); // Bang opt kich thuoc nxn luu lai nut goc toi uu
    vector<long long> f(n, 0); // Mang tien to cua freq
    f[0] = freq[0];
    for (int i = 1; i < n; ++i) {
        f[i] = f[i - 1] + freq[i];
    }
    for (int i = 0; i < n; ++i) {
        d[i][i] = freq[i];
        opt[i][i] = i;
    }
    for (int len = 2; len <= n; ++len) {
        for (int l = 0; l <= n - len; ++l) {
            int r = l + len - 1;
            d[l][r] = oo;
            for (int i = opt[l][r - 1]; i <= opt[l + 1][r]; ++i) {
                long long sum_freq = f[r];
                if (l > 0) sum_freq -= f[l - 1]; // freq[l] + freq[l + 1] + ... + freq[r]
                long long cur_cost = sum_freq;
                if (l <= i - 1) cur_cost += d[l][i - 1];
                if (i + 1 <= r) cur_cost += d[i + 1][r];
                if (d[l][r] > cur_cost) {
                    d[l][r] = cur_cost;
                    opt[l][r] = i;
                }
            }
        }
    }
    BinarySearchTree t;
    t.n = n;
    t.key = key;
    t.left = vector<int>(n, -1);
    t.right = vector<int>(n, -1);
    t.root = trace(0, n - 1, opt, t.left, t.right);
    return t;
}
signed main() {
    int n;
    cout << "Nhap vao so luong nut cua cay nhi phan can xay: ";
    cin >> n;
    vector<int> key(n), freq(n);
    cout << "Nhap vao gia tri cua cac khoa theo thu tu tang dan: ";
    for (int i = 0; i < n; ++i) {
        cin >> key[i];
    }
    cout << "Nhap vao gia tri cua so lan truy cap cho tung khoa: ";
    for (int i = 0; i < n; ++i) {
        cin >> freq[i];
    }
    BinarySearchTree t = findOptimalBinarySearchTree(n, key, freq);
    cout << "Tong chi phi truy cap cua cay tim kiem nhi phan toi uu la: " << cost(t, freq) << '\n';
    cout << "Mot cay tim kiem nhi phan toi uu:\n";
    cout << "Nut goc: " << t.root << '\n';
    cout << "Khoa cua tung nut, nut con ben trai va nut con ben phai (-1 trong truong hop khong ton tai) ke voi no theo thu tu cac nut tu 0 den " 
    << n - 1 << ":\n";
    for (int i = 0; i < n; ++i) {
        cout << "Khoa, nut con ben trai, phai cua nut thu " << i << ":\n";
        cout << t.key[i] << ' ' << t.left[i] << ' ' << t.right[i] << '\n';
    }
    return 0;
}