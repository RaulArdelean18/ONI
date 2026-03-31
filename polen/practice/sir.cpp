#include <bits/stdc++.h>
using namespace std;

#define ll long long
#define int ll
const string fn("sir");

ifstream in(fn + ".in");
ofstream out(fn + ".out");

#define cin in
#define cout out

unordered_map<int, int> m;

signed main()
{
    int n, x, sum = 0;

    cin >> n;
    for (int i = 1; i <= n; i++)
    {
        cin >> x;
        if (i != 1)
        {
            if (m[x] == 0)
                cout << "-1\n";
            else
                cout << m[x] << '\n';
        }
        sum += x;
        m[sum] = i;
    }
}