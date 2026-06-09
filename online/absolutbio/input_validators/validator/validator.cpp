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
	ll N = Int(1, Arg("maxn",1e9)); Space();
	ll Q = Int(1, Arg("maxq",2e5)); Endl();


	set<int> seen;

	int onlyadd = Arg("onlyadd",0);

	rep(i,0,Q) {
		int type = Int(1,2); Space();

		if (onlyadd) assert(type == 1);

		if (type == 1) {
			int x = Int(1,N); Endl();

			seen.insert(x);
		}
		else {
			int l = Int(1,N); Space();
			int r = Int(1,N); Endl();

			assert(l != r);
			assert(seen.count(l) == 1);

			if (seen.count(r) == 0) {
				seen.insert(r);
				seen.erase(l);
			}
		}
	}
}

