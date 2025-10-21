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

	int ratio = n/k;

	set<string> names;
	int totbeds = 0;

	rep(i,0,k) {
		string line = Line();
		string name;
		int cnt;

		rep(j,0,sz(line)){
			if (line[j] == ' ') {
				name = line.substr(0,j);
				cnt = stoi(line.substr(j+1,sz(line)-j-1));
				break;
			}
		}

		assert(sz(name) <= 6);

		names.insert(name);
		totbeds += cnt;

		if (sameAmount) assert(ratio == cnt);
	}


	assert(sz(names) == k);
	assert(totbeds == n);

	Eof();

}

