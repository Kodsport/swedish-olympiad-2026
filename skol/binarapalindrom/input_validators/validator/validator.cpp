#include "validator.h"
#include <cstring>
#include <vector>

typedef long long ll;

void run() {
    ll maxn = Arg("maxn", (ll)1e18);
    ll maxk = Arg("maxk", (ll)59);

    ll a = Int(1, maxn);
    Space();
    ll b = Int(1, maxn);
    Space();
    ll k = Int(1, maxk);
    Endl();

    assert(a <= b);

    Eof();
}
