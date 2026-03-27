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
    int no_short_cycles = Arg("no_short_cycles", 0);
    int one_cycle = Arg("one_cycle", 0);
    int identity = Arg("identity", 0);

    int n = Int(2, maxn);
    Endl();
    
    vi P = SpacedInts(n, 1, n);
    rep(c1,0,n){
        P[c1]--;
    }

    set<int> S;
    trav(y, P){
        S.insert(y);
    }
    assert(sz(S) == n);

    if(no_short_cycles == 1){
        rep(c1,0,n){
            assert(P[P[c1]] != c1);
        }
    }

    if(one_cycle == 1){
        rep(c1,0,n){
            assert(P[c1] == (c1+1)%n);
        }
    }

    if(identity == 1){
        rep(c1,0,n){
            assert(P[c1] == c1);
        }
    }

}