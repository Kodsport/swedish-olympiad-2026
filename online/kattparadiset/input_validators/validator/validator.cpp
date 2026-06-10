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
	int maxn = Arg("maxn", 2e5);
	int maxk = Arg("maxk", 2e5);
	int sameAmount = Arg("allsame",0);

	int n = Int(3, maxn);
	Space();
	int k = Int(2, maxk);
	Endl();

	assert(n >= k);

	if (sameAmount) assert(n % k == 0);

	set<string> colors;
	ll totbeds = 0;

	rep(i,0,k) {
		string color = Word();

		assert(sz(color) >= 1 && sz(color) <= 6);
		colors.insert(color);
		for (char c : color) {
			assert(c >= 'a' && c <= 'z');
		}

		Space();
		int cnt = Int(1, n);
		totbeds += cnt;
		Endl();

		if (sameAmount) assert(cnt == n / k);
	}


	assert(sz(colors) == k);
	assert(totbeds == n);
}
