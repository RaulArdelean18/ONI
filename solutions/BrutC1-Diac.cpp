/*
autor:   Paul Diac
timp:    O(QK^2)
idee:    brut
*/
#include <cstdio>
#include <cassert>
using namespace std;
#define NMax 1011
 
long C, N, M, Q;
long r,c, K, val, cheie;
//         0:NE 1:SV 2:NV 3:SE
long dl[] = {-1,  +1,  -1,  +1};
long dc[] = {+1,  -1,  -1,  +1};
 
long long g[NMax][NMax];
int main() {
    freopen("polen.in", "rt", stdin);
    freopen("polen.out", "wt", stdout);
 
    scanf("%ld %ld %ld %ld", &C, &N, &M, &Q);
    assert(1 <= C); assert(C <= 3);
    assert(1 <= N); assert(N <= 1000);
    assert(1 <= M); assert(M <= 1000);
    assert(1 <= Q); assert(Q <= 1000000);
 
    if (!((C == 1) && (N <= 200) && (M <= 200) && (Q <= 200))) {
        // nu e task 1, dau return
        return 0;
    }
 
    while (Q--) {
        scanf("%ld %ld %ld %ld %ld", &r,&c, &K, &val, &cheie);
        assert(1 <= r); assert(r <= N);
        assert(1 <= c); assert(c <= M);
        assert(1 <= K); assert(K <= 1000);
        assert(0 <= cheie); assert(cheie <= 15);
        assert(-1000000000 <= val); assert(val <= +1000000000);
 
        long lin, col, v, i, j;
        for (v = 0; v < 4; v++) if (cheie & (1 << v)) { // vantul
            for (i = 0; i <= K; i++) {
                lin = r + dl[v] * i;
                if (1 <= lin && lin <= N) {
                    for (j = K-i; j >= 0; j--) {
                        col = c + dc[v] * j;
                        if (1 <= col && col <= M) {
                            g[lin][col] += val;
                        }
                    }
                }
            }
        }
    }
 
    for (long i = 1; i <= N; i++) {
        for (long j = 1; j <= M; j++) {
            printf("%lld ", g[i][j]);
        }
        printf("\n");
    }
    return 0;
}