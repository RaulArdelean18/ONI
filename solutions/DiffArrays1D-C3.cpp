/// Ardelean Raul, cerinta 3, smenul lui Mars pe linii => O(Q*K + N*M)
#include <fstream>
#include <algorithm>
 
using namespace std;
 
ifstream in("polen.in");
ofstream out("polen.out");
 
#define cin in
#define cout out
 
const int MAX_N = 1005;
 
long long diff[MAX_N][MAX_N];
int C, N, M, Q;
 
void add_interval(int row, int L, int R, long long val)
{
    if (row < 1 || row > N)
        return;
    if (L < 1)
        L = 1;
    if (R > M)
        R = M;
    if (L > R)
        return;
    diff[row][L] += val;
    diff[row][R + 1] -= val;
}
 
void apply_ne(int r, int c, int K, long long val)
{
    for (int i = r - K; i <= r; i++)
    {
        int dist = i - (r - K);
        int L = c;
        int R = c + dist;
        add_interval(i, L, R, val);
    }
}
 
void apply_nw(int r, int c, int K, long long val)
{
    for (int i = r - K; i <= r; i++)
    {
        int dist = i - (r - K);
        int L = c - dist;
        int R = c;
        add_interval(i, L, R, val);
    }
}
 
void apply_sw(int r, int c, int K, long long val)
{
    for (int i = r; i <= r + K; i++)
    {
        int dist = i - r;
        int L = c - K + dist;
        int R = c;
        add_interval(i, L, R, val);
    }
}
 
void apply_se(int r, int c, int K, long long val)
{
    for (int i = r; i <= r + K; i++)
    {
        int dist = i - r;
        int L = c;
        int R = c + K - dist;
        add_interval(i, L, R, val);
    }
}
 
int main()
{
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    cout.tie(NULL);
 
    cin >> C;
    cin >> N >> M >> Q;
 
    for (int q = 0; q < Q; q++)
    {
        int r, c, K, cheie;
        long long val;
        cin >> r >> c >> K >> val >> cheie;
 
        if (cheie & 1)
            apply_ne(r, c, K, val);
        if (cheie & 2)
            apply_sw(r, c, K, val);
        if (cheie & 4)
            apply_nw(r, c, K, val);
        if (cheie & 8)
            apply_se(r, c, K, val);
    }
 
    for (int i = 1; i <= N; i++, cout << '\n')
    {
        long long cur = 0;
        for (int j = 1; j <= M; j++)
        {
            cur += diff[i][j];
            cout << cur << ' ';
        }
    }
 
    return 0;
}