#include <bits/stdc++.h>
using namespace std;

const string fn("polen");

ifstream in(fn + ".in");
ofstream out(fn + ".out");

#define cin in
#define cout out 

int dist(int r,int limit)
{
    return max(r-1,limit-r);
}

signed main()
{
    int C;
    cin>>C;
    assert(C==3);
    int N,M,Q;
    cin>>N>>M>>Q;
    assert(N>=1 and M>=1 and Q>=1);
    assert(N<=1e3 and M<=1e3 and Q<=1e6);

    function<void(int,int,int,int,int)> verif = [&](int r,int c,int K,int val, int cheie)
    {
        assert(r>=1 and r<=N);
        assert(c>=1 and c<=M);
        assert(1<=K and K<=1e3);
        assert(cheie>=0 and cheie<=15);
        assert(-1e9<=val and val<=1e9);
    };

    for(int i=1;i<=Q;i++)
    {
        int r1,c1,K1,val1,cheie1;
        cin>>r1>>c1>>K1>>val1>>cheie1;
        verif(r1,c1,K1,val1,cheie1);
    }
    return 0;
}