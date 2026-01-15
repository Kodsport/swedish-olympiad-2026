#include "validator.h"

typedef long long ll;

void run() {
    
    ll maxn = Arg("maxn", 200000);
    ll maxt = Arg("maxt", 1000000000);

    ll n = Int(2, maxn);        Space();
    ll s = Int(1, 1000000000);  Endl();

    ll last = 0;

    for (int i = 0; i < n; i++) {
        ll x = Int(1, 1000000000); Space();
        ll t = Int(1, maxt); Endl();

        assert(last < x);
        last = x;
    }

    assert(last*8 < s);

    Eof();
}
