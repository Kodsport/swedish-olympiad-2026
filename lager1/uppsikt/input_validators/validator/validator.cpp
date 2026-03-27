#include "validator.h"

void run() {
    int n = Int(1, Arg("maxn", 300000));
    Endl();

    vector<int> p = SpacedInts(n, 1, n);

    vector<int> seen(n + 1, 0);
    for (int x : p) {
        assert(x >= 1 and x <= n and !seen[x]);
        seen[x] = 1;
    }
}
