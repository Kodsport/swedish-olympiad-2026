#include "validator.h"

const int MAXN = 300;

void run() {
    int N = Int(2, MAXN);
    Space();
    int M = Int(2, N);
    Space();
    int T = Int(0, 3);
    Endl();

    assert(T == int(Arg("T")));
    if(T == 1)assert(M == 2);
    if(T == 2)assert(M == N);

    vector<int> weights = SpacedInts(N, 1, M);
    for(int c1 = 1; c1 < N; c1++){
        assert(weights[c1] >= weights[c1-1]);
    }

}
