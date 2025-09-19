#include <bits/stdc++.h>
using namespace std;

#define rep(i, a, b) for(int i = a; i < (b); ++i)
#define all(x) begin(x), end(x)
#define sz(x) (int)(x).size()
typedef long long ll;
typedef pair<int, int> pii;
typedef vector<int> vi;

#define dbg(x) cout << #x << " = " << x << endl;

int main() {
	cin.tie(0)->sync_with_stdio(0);
	cin.exceptions(cin.failbit);

   	ll a, b;
	cin >> a >> b;

	assert(a == b);

	vector<bool> vis(360, 0);

	ll dir = 0;

	while(!vis[dir]) {
		vis[dir] = 1;
		dir = (dir + a) % 360;
	}

	cout << count(all(vis), 1) << '\n';
}
