#include <fstream>
using namespace std;

#define ll long long
#define int ll
const string fn("aplusb");

ifstream in(fn + ".in");
ofstream out(fn + ".out");

#define cin in
#define cout out
signed main()
{
    ll a, b;
    cin >> a >> b;
    cout << a + b;
    return 0;
}