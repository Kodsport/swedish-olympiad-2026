#include "validator.h"

using namespace std;

#define rep(i, a, b) for(int i = a; i < (b); ++i)
#define trav(i, a) for(auto& i : a)
#define all(x) begin(x), end(x)
#define sz(x) (int)(x).size()
typedef long long ll;
typedef pair<int, int> pii;
typedef vector<int> vi;

struct UF {
    vi par;
    UF(int n) : par(n) {
        rep(i,0,n) par[i]=i;
    }
    int find(int x) {return par[x]==x?x:par[x]=find(par[x]);}
    bool merge(int a, int b) {
        a = find(a); b = find(b);
        if (a==b) return false;
        par[b]=a;
        return true;
    }
};

void run() {
    int n = Int(1, Arg("maxn", int(1e5)));
    Space();
    int m = Int(3, Arg("maxm", int(1e5)));
    Space();
    int q = Int(1, Arg("maxq", int(1e5)));
    Endl();
    assert(m <= n);

    vector<int> par = SpacedInts(n, 1, n);
    int num_fixpoints = 0;
    for (int i = 0; i < n; i++) {
        par[i]--;
        num_fixpoints += par[i]==i;
    }

    if (Arg("is_line", 0)) {
        assert(par[0]==0);
        for (int i = 1; i < n; i++) assert(par[i]==i-1);
    }

    assert(num_fixpoints == 1);
    UF uf(n);
    rep(i,0,n) {
        if (i==par[i]) continue;
        assert(uf.merge(i, par[i]));
    }

    vector<int> which_house = SpacedInts(m, 1, n);
    vector<int> taken(n);
    for (auto h : which_house) {
        h--;
        assert(!taken[h]);
        taken[h]=1;
    }

    int mint = Arg("mint", 1);
    int maxr = Arg("maxr", n-1);
    while (q--) {
        int t = Int(mint, 2);
        Space();
        if (t==1) {
            int a = Int(1,n);
            Space();
            int b = Int(1,n);
            assert(a!=b);
        }
        else {
            Int(1,m);
            Space();
            Int(1, maxr);
        }
        Endl();
    }
}

