#include <bits/stdc++.h>
using namespace std;

#define rep(i, a, b) for(int i = a; i < (b); ++i)
#define all(x) begin(x), end(x)
#define sz(x) (int)(x).size()
typedef long long ll;
typedef pair<int, int> pii;
typedef vector<int> vi;

#define dbg(x) cout << #x << " = " << x << endl

int main() {
	cin.tie(0)->sync_with_stdio(0);
	cin.exceptions(cin.failbit);

	ll l, r, k;
	cin >> l >> r >> k;

	vector<ll> palindromes;
	rep(B, 0, (1<<17)) {
		vector<ll> b;
		rep(i, 0, 17) {
			if (B & (1LL<<i)) b.push_back(1);
			else b.push_back(0);
		}
		while(!b.empty() && b.back() == 0) b.pop_back();
		reverse(all(b));
		vector<ll> c = b;
		ll len = sz(b);
		for(int i = len - 1; i >= 0; i--) {
			b.push_back(b[i]);
			if (i != len - 1) c.push_back(b[i]);
		}
		ll one = 0, two = 0;
		rep(i, 0, sz(b)) {
			if (b[i] == 1) one += (1LL<<i);
		}
		rep(i, 0, sz(c)) {
			if (c[i] == 1) two += (1LL<<i);
		}
		if (count(all(b), 1) == k) palindromes.push_back(one);
		if (count(all(c), 1) == k) palindromes.push_back(two);
	}

	sort(all(palindromes));

	ll ans = 0;
	rep(i, 0, sz(palindromes)) {
		if (l <= palindromes[i] && palindromes[i] <= r) ans++;
	}

	cout << ans;
}

