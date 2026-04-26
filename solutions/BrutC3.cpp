/// Ardelean Raul, cerinta 3, brut => O(Q*K^2)
#include <fstream>
#include <algorithm>
 
using namespace std;
 
ifstream in("polen.in");
ofstream out("polen.out");
 
#define cin in
#define cout out
 
const int MAX_N = 2505;
 
long long mat[MAX_N][MAX_N];
int C, N, M, Q;
 
void add_cell(int i, int j, long long val)
{
    if (i < 1 || i > N || j < 1 || j > M)
        return;
    mat[i][j] += val;
}
 
void apply_ne(int r, int c, int K, long long val)
{
    for (int i = r - K; i <= r; i++)
    {
        int dist = i - (r - K);
        for (int j = c; j <= c + dist; j++)
            add_cell(i, j, val);
    }
}
 
void apply_nw(int r, int c, int K, long long val)
{
    for (int i = r - K; i <= r; i++)
    {
        int dist = i - (r - K);
        for (int j = c - dist; j <= c; j++)
            add_cell(i, j, val);
    }
}
 
void apply_sw(int r, int c, int K, long long val)
{
    for (int i = r; i <= r + K; i++)
    {
        int dist = i - r;
        int L = c - K + dist;
        int R = c;
        for (int j = L; j <= R; j++)
            add_cell(i, j, val);
    }
}
 
void apply_se(int r, int c, int K, long long val)
{
    for (int i = r; i <= r + K; i++)
    {
        int dist = i - r;
        int L = c;
        int R = c + K - dist;
        for (int j = L; j <= R; j++)
            add_cell(i, j, val);
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
        for (int j = 1; j <= M; j++)
            cout << mat[i][j] << ' ';
 
    return 0;
}