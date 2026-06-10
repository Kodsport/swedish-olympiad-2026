#include "validator.h"
#include <unordered_map>
typedef vector<int> vi;
#define rep(i, lo, hi) for (int i = (lo); i < (hi); i++)


void run() {
    int maxn = Arg("maxn",75);
    int n = Int(5,maxn); Space();
    int T = Int(0,4); Endl();
    assert(n%2 == 1);

    int graded = Arg("graded");
    assert(graded == T);

    int x1,y1,x2,y2;
    x1 = Int(0,n-1); Space();
    y1 = Int(0,n-1); Space();
    x2 = Int(0,n-1); Space();
    y2 = Int(0,n-1); Endl();

    for (auto x: {x1,y1,x2,y2}) {
        assert(x%2 == 1);
    }

    assert(x1 != x2);
    assert(y1 != y2);

    bool corner = Arg("corner",0);
    if (corner) {
        assert((x1 == 1 and y1 == 1) or (x2 == 1 and y2 == 1));
    }
}
