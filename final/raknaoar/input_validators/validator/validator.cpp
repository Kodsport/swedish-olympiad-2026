#include "validator.h"
#include <cstring>
#include <vector>

typedef long long ll;

void run() {
    int maxn = Arg("maxn", int(1e5));
    int maxval = Arg("maxval", int(1e9));
    int maxm = Arg("maxm", int(3e5));
    int maxq = Arg("maxq", int(1e5));

    int n = Int(1, maxn);
    Space();
    int q = Int(1, maxq);
    Endl();

    int ksum = 0;

    for (int i = 0; i < n; i++){
        int k = Int(1, maxm);
        Space();
        for (int j = 0; j < k-1; j++){ 
            Int(1, maxval);
            Space();
        }
        if(k) Int(1, maxval);
        Endl();
        ksum += k;
    }

    assert(ksum <= maxm);

    for (int i = 0; i < q; i++) {
        Int(1, maxval);
        Endl();
    }
}
