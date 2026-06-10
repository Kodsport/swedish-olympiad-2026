#include "validator.h"

#define sz(x) (int)(x).size()

void run() {

    int maxn = Arg("maxn", 1000);
    int maxx = Arg("maxx", 1);
    int a1 = Arg("a1", 1000000);
    int max_unique = Arg("max_unique", 1000);

    int n = Int(1, maxn);
    Space();
    int x = Int(0, maxx);
    Endl();
    
    vector<int> A = SpacedInts(n, 1, 1000000);
    assert(A[0] <= a1);
    set<int> S;
    for (auto y : A){
        S.insert(y);
    }
    assert(sz(S) <= max_unique);
}