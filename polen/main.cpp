/*
autor:   Ardelean Raul
timp:    O(N*M + Q)
idee:    smenul lui mars pe linie si pe coloana
*/
#include <fstream>
#include <algorithm>
 
using namespace std;
 
ifstream in("polen.in");
ofstream out("polen.out");
 
#define cin in
#define cout out
 
const int MAX_N = 1005, MAX_Q = 1000000 + 5;
 
int C;
int N, M, Q;
long long sp_vert[MAX_N][MAX_N], sp_diag[MAX_N][MAX_N], ans[MAX_N][MAX_N], row_mirror[MAX_N];
 
struct Query
{
    int r, c, K, mask;
    long long val;
};
 
Query queries[MAX_Q];
 
void add_vert(int r1, int r2, int c, long long val)
{
    if (r1 > r2)
        return;
    sp_vert[r1][c] += val;
    sp_vert[r2 + 1][c] -= val;
}
 
void add_diag(int r1, int c1, int r2, int c2, long long val)
{
    if (r1 > r2)
        return;
    sp_diag[r1][c1] += val;
    sp_diag[r2 + 1][c2 + 1] -= val;
}
 
void clear_arrays()
{
    for (int i = 0; i <= N + 1; i++)
        for (int j = 0; j <= M + 1; j++)
            sp_vert[i][j] = sp_diag[i][j] = 0;
}
 
void clear_ans()
{
    for (int i = 0; i <= N + 1; i++)
        for (int j = 0; j <= M + 1; j++)
            ans[i][j] = 0;
}
 
void build_smen()
{
    // Smen pe verticala
    for (int j = 1; j <= M + 1; j++)
        for (int i = 1; i <= N; i++)
            sp_vert[i][j] += sp_vert[i - 1][j];
 
    // Smen pe diagonala
    for (int i = 1; i <= N; i++)
        for (int j = 1; j <= M + 1; j++)
            sp_diag[i][j] += sp_diag[i - 1][j - 1];
}
 
void apply_ne(int r, int c, int K, long long val)
{
    int top = r - K, bottom = r;
    int row_start = max(top, 1), row_end = min(bottom, N);
    if (row_start > row_end)
        return;
 
    int i_max = top + (M - c); // ultima linie unde muchia dreapta e in matrice
    int diag_end = min(row_end, i_max);
    if (row_start <= diag_end)
    {
        add_vert(row_start, diag_end, c, val);
        int col_start = c + (row_start - top) + 1, col_end = c + (diag_end - top) + 1;
        add_diag(row_start, col_start, diag_end, col_end, -val);
    }
 
    int rect_start = max(row_start, i_max + 1);
    if (rect_start <= row_end)
    {
        add_vert(rect_start, row_end, c, val);
        add_vert(rect_start, row_end, M + 1, -val);
    }
}
 
void apply_sw(int r, int c, int K, long long val)
{
    int top = r, bottom = r + K;
    int row_start = max(top, 1), row_end = min(bottom, N);
    if (row_start > row_end)
        return;
 
    int i_min = bottom - c + 1; // prima linie unde muchia stanga intra in matrice
 
    int rect_end = min(row_end, i_min - 1);
    if (row_start <= rect_end)
    {
        add_vert(row_start, rect_end, 1, val);
        add_vert(row_start, rect_end, c + 1, -val);
    }
 
    int diag_start = max(row_start, i_min);
    if (diag_start <= row_end)
    {
        int col_start = diag_start + (c - bottom), col_end = row_end + (c - bottom);
        add_diag(diag_start, col_start, row_end, col_end, val);
        add_vert(diag_start, row_end, c + 1, -val);
    }
}
 
int main()
{
    cin >> C;
    cin >> N >> M >> Q;
 
    bool need_direct = false, need_mirror = false;
 
    for (int i = 0; i < Q; i++)
    {
        Query &aux = queries[i];
        cin >> aux.r >> aux.c >> aux.K >> aux.val >> aux.mask;
  
        if (aux.mask & 3)
            need_direct = true;
        if (aux.mask & 12)
            need_mirror = true;
    }
 
    clear_ans();
    if (need_direct)
    {
        clear_arrays();
        for (int i = 0; i < Q; i++)
        {
            Query &qu = queries[i];
            if (qu.mask & 1)
                apply_ne(qu.r, qu.c, qu.K, qu.val);
            if (qu.mask & 2)
                apply_sw(qu.r, qu.c, qu.K, qu.val);
        }
 
        build_smen();
 
        for (int i = 1; i <= N; i++)
        {
            long long curr_val = 0;
            for (int j = 1; j <= M; j++)
            {
                curr_val += sp_vert[i][j];
                curr_val += sp_diag[i][j];
                ans[i][j] = curr_val;
            }
        }
    }
 
    if (need_mirror)
    {
        clear_arrays();
        for (int i = 0; i < Q; i++)
        {
            Query &qu = queries[i];
            int mc = M + 1 - qu.c;
            if (qu.mask & 4)
                apply_ne(qu.r, mc, qu.K, qu.val); // NW
            if (qu.mask & 8)
                apply_sw(qu.r, mc, qu.K, qu.val); // SE
        }
 
        build_smen();
 
        for (int i = 1; i <= N; i++)
        {
            long long curr_val = 0;
            for (int j = 1; j <= M; j++)
            {
                curr_val += sp_vert[i][j];
                curr_val += sp_diag[i][j];
                row_mirror[M + 1 - j] = curr_val;
            }
            for (int j = 1; j <= M; j++)
                ans[i][j] += row_mirror[j];
        }
    }
 
    for (int i = 1; i <= N; i++, cout << endl)
        for (int j = 1; j <= M; j++)
            cout << ans[i][j] << ' ';
 
    return 0;
}