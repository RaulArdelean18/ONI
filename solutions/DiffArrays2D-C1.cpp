/// Ardelean Raul, cerinta 1, sume partiale => O(Q + N*M)
/// se formeaza o matrice
#include <fstream>
#include <algorithm>
 
using namespace std;
 
ifstream in("polen.in");
ofstream out("polen.out");
 
#define cin in
#define cout out
 
const int MAX_N = 1005;
 
long long diff[MAX_N + 2][MAX_N + 2];
int C, N, M, Q;
 
void add_rect(int r1, int c1, int r2, int c2, long long val)
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
 
    diff[r1][c1] += val;
    diff[r1][c2 + 1] -= val;
    diff[r2 + 1][c1] -= val;
    diff[r2 + 1][c2 + 1] += val;
}
 
int main()
{
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    cout.tie(NULL);
 
    cin >> C;
    cin >> N >> M >> Q;
 
    for (int i = 0; i + 1 < Q; i += 2)
    {
        int r1, c1, K1, ch1;
        int r2, c2, K2, ch2;
        long long val1, val2;
 
        cin >> r1 >> c1 >> K1 >> val1 >> ch1;
        cin >> r2 >> c2 >> K2 >> val2 >> ch2;
 
        int rmin = min(r1, r2);
        int rmax = max(r1, r2);
        int cmin = min(c1, c2);
        int cmax = max(c1, c2);
 
        add_rect(rmin, cmin, rmax, cmax, val1);
    }
 
    for (int i = 1; i <= N; i++, cout << '\n')
    {
        for (int j = 1; j <= M; j++)
        {
            long long val = diff[i][j];
            if (i > 1)
                val += diff[i - 1][j];
            if (j > 1)
                val += diff[i][j - 1];
            if (i > 1 && j > 1)
                val -= diff[i - 1][j - 1];
            diff[i][j] = val;
            cout << val << ' ';
        }
    }
 
    return 0;
}