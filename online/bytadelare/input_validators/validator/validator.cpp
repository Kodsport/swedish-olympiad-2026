#include "validator.h"

#include <bits/stdc++.h>
using namespace std;

#define fo(i, n) for(ll i=0;i<((ll)n);i++)
#define rep(i, a, b) for(int i = a; i < (b); ++i)
#define trav(i, a) for(auto& i : a)
#define all(x) begin(x), end(x)
#define sz(x) (int)(x).size()
typedef long long ll;
typedef pair<int, int> pii;
typedef vector<int> vi;

#define LSOne(S) ((S) & (-S))


void run() {

    int maxn = Arg("maxn", 1000);
    int maxx = Arg("maxx", 1);
    int a1 = Arg("a1", 1000000);
    int max_unique = Arg("max_unique", 1000);

    int n = Int(1, maxn);
    Space();
    int x = Int(0, maxx);
    Endl();
    
    vi A = SpacedInts(n, 1, 1000000);
    assert(A[0] <= a1);
    set<int> S;
    trav(y, A){
        S.insert(y);
    }
    assert(sz(S) <= max_unique);
}