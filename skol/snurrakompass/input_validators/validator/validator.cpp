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
	int a = Int(1, 360);
	Endl();
	int b = Int(1, 360);
	Endl();

	if (Arg("ab", 0)) assert(a == b);
	if (Arg("prime", 0)) assert(a == 13);
}

