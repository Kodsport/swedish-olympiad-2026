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
	int ab = Arg("ab", 0);
    
	int a = Int(1, 360);
	Space();
	int b = Int(1, 360);

	if (ab == 1) assert(a == b);

	Endl();

	Eof();
}

