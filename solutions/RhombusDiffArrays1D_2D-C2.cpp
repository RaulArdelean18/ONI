/// Ardelean Raul, cerinta 2, suma partiala pe Romb => O(Q + N*M)
#include <fstream>
#include <algorithm>
 
using namespace std;
 
ifstream in("polen.in");
ofstream out("polen.out");
 
#define cin in
#define cout out
 
const int MAX_N = 1005;
const int MAX_U = 2005;
 
long long diff_rc[MAX_N + 2][MAX_N + 2];
long long diff_uv[MAX_U + 2][MAX_U + 2];
int C, N, M, Q;
 
void add_rect_rc(int r1, int c1, int r2, int c2, long long val)
{
    if (r1 > r2 || c1 > c2)
        return;
    if (r2 < 1 || c2 < 1 || r1 > N || c1 > M)
        return;
 
    if (r1 < 1)
        r1 = 1;
    if (c1 < 1)
        c1 = 1;
    if (r2 > N)
        r2 = N;
    if (c2 > M)
        c2 = M;
 
    diff_rc[r1][c1] += val;
    diff_rc[r1][c2 + 1] -= val;
    diff_rc[r2 + 1][c1] -= val;
    diff_rc[r2 + 1][c2 + 1] += val;
}
 
void add_rect_uv(int u1, int v1, int u2, int v2, long long val, int UMAX, int VMAX)
{
    if (u1 > u2 || v1 > v2)
        return;
    if (u2 < 1 || v2 < 1 || u1 > UMAX || v1 > VMAX)
        return;
 
    if (u1 < 1)
        u1 = 1;
    if (v1 < 1)
        v1 = 1;
    if (u2 > UMAX)
        u2 = UMAX;
    if (v2 > VMAX)
        v2 = VMAX;
 
    diff_uv[u1][v1] += val;
    diff_uv[u1][v2 + 1] -= val;
    diff_uv[u2 + 1][v1] -= val;
    diff_uv[u2 + 1][v2 + 1] += val;
}
 
int main()
{
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    cout.tie(NULL);
 
    cin >> C;
    cin >> N >> M >> Q;
 
    int UMAX = N + M;
    int VMAX = N + M;
    int v_shift = M;
 
    for (int q = 0; q < Q; q++)
    {
        int r, c, K, cheie;
        long long val;
        cin >> r >> c >> K >> val >> cheie;
 
        int u0 = r + c;
        int v0 = r - c + v_shift;
        add_rect_uv(u0 - K, v0 - K, u0 + K, v0 + K, val, UMAX, VMAX);
 
        add_rect_rc(r, c - K, r, c + K, val);
        add_rect_rc(r - K, c, r + K, c, val);
        add_rect_rc(r, c, r, c, val);
    }
 
    for (int i = 1; i <= N; i++)
        for (int j = 1; j <= M; j++)
        {
            long long v = diff_rc[i][j];
            if (i > 1)
                v += diff_rc[i - 1][j];
            if (j > 1)
                v += diff_rc[i][j - 1];
            if (i > 1 && j > 1)
                v -= diff_rc[i - 1][j - 1];
            diff_rc[i][j] = v;
        }
 
    for (int u = 1; u <= UMAX; u++)
        for (int v = 1; v <= VMAX; v++)
        {
            long long x = diff_uv[u][v];
            if (u > 1)
                x += diff_uv[u - 1][v];
            if (v > 1)
                x += diff_uv[u][v - 1];
            if (u > 1 && v > 1)
                x -= diff_uv[u - 1][v - 1];
            diff_uv[u][v] = x;
        }
 
    for (int i = 1; i <= N; i++)
    {
        for (int j = 1; j <= M; j++)
        {
            int u = i + j;
            int v = i - j + v_shift;
            long long ans = diff_rc[i][j] + diff_uv[u][v];
            cout << ans << ' ';
        }
        cout << "\n";
    }
 
    return 0;
}