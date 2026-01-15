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

    int maxx = 1000000000;
    int maxn = Arg("maxn", 300000);
    int sub1 = Arg("sub1", 0);
    int sub2 = Arg("sub2", 0);

    int n = Int(1, maxn);
    Space();
    int v = Int(0, maxx);
    Endl();
    
    vi A = SpacedInts(n, 0, maxx);

    vi A2 = A;
    sort(all(A2));
    if(sub1 == 1){
        assert(v < A[0]);
        for(int c1 = 1; c1 < n; c1++){
            assert(A[c1] > A[c1-1]);
        }
    }
    if(sub2 == 1){
        for(int c1 = 1; c1 < n; c1++){
            assert(A[c1] < A[c1-1]);
        }
    }

}