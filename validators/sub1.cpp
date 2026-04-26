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
    assert(C==1);
    int N,M,Q;
    cin>>N>>M>>Q;
    assert(N>=1 and M>=1 and Q>=1);
    assert(N<=200 and M<=200 and Q<=200);

    function<void(int,int,int,int,int)> verif = [&](int r,int c,int K,int val, int cheie)
    {
        assert(r>=1 and r<=N);
        assert(c>=1 and c<=M);
        assert(1<=K and K<=min(dist(r,N),dist(c,M)));
        assert(0<=cheie and cheie<=15);
        assert(-1e9<=val and val<=1e9);
    };

    for(int i=1;i<=Q;i+=2)
    {
        int r1,c1,K1,val1,cheie1;
        int r2,c2,K2,val2,cheie2;
        cin>>r1>>c1>>K1>>val1>>cheie1;
        cin>>r2>>c2>>K2>>val2>>cheie2;
        verif(r1,c1,K1,val1,cheie1);
        verif(r2,c2,K2,val2,cheie2);
        if(cheie1==1)
            assert(cheie2==2);
        if(cheie1==2)
            assert(cheie2==1);
        if(cheie1==4)
            assert(cheie2==8);
        if(cheie1==8)
            assert(cheie2==4);
            
    }
    return 0;
}