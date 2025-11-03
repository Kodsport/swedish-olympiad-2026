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

	set<string> names;
	int totbeds = 0;

	rep(i,0,k) {
		string line = Line();

		int space_ind = line.find(' ');
		assert(space_ind != string::npos);
		string name = line.substr(0, space_ind);
		assert(sz(name) >= 1 && sz(name) <= 6);
		names.insert(name);
		for (char c : name) {
			assert(c >= 'a' && c <= 'z');
		}

		int cnt = Int(1, n, line.substr(space_ind + 1));
		totbeds += cnt;

		if (sameAmount) assert(cnt == n / k);
	}


	assert(sz(names) == k);
	assert(totbeds == n);
}
