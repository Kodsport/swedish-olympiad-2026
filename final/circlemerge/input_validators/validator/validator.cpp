#include "validator.h"

void run() {
    

    long long n = Int(1, Arg("maxn", 200000));
    Endl();

    vector<long long> a = SpacedInts(n, (long long) 1, (long long) Arg("maxa", (long long) 5000000000000LL));

    Eof();
}
