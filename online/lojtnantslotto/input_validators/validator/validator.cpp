#include "validator.h"

using namespace std;

#define rep(i, a, b) for(int i = a; i < (b); ++i)
#define trav(i, a) for(auto& i : a)
#define all(x) begin(x), end(x)
#define sz(x) (int)(x).size()
typedef long long ll;
typedef pair<int, int> pii;
typedef vector<int> vi;


void run() {
    int n = Int(1, Arg("maxn", int(2e5)));
    Space();
    int t = Int(1, Arg("maxt", int(1e6)));
    Endl();

    int maxl = Arg("maxl", 5);
    for (int i = 0; i < n; i++) {
        Int(1, maxl);
        Space();
        Int(1, int(1e6));
        Endl();
    }
}

