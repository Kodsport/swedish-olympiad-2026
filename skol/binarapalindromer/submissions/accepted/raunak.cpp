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

	vector<vector<ll>> C(61, vector<ll>(61, 0));
	rep(i, 0, 61) {
		C[i][0] = C[i][i] = 1;
		rep(j, 1, i) {
			C[i][j] = C[i-1][j-1] + C[i-1][j];
		}
	}

	auto calc = [&](ll x) -> ll { 
		vector<ll> b;
		rep(i, 0, 63) {
			if (x & (1ULL<<i)) b.push_back(1);
			else b.push_back(0);
		}

		while(!b.empty() && b.back() == 0) b.pop_back();

		if (b.empty()) b.push_back(0);

		reverse(all(b));

		ll ans = 0;

		// shorter palindromes
		rep(len, 1, sz(b)) {
			if (len < k) continue;
			if (len == 1) {
				if (k == 1) ans++;
			}
			else {
				int half = len / 2;
				if (k % 2 == 1) {
					if (len % 2 == 0) continue;
					int need = (k - 1) / 2;
					if (need >= 1 && need <= half) ans += C[half - 1][need - 1];
				}
				else {
					int need = k / 2;
					if (need >= 1 && need <= half) ans += C[half - 1][need - 1];
				}
			}
		}

		// itself a palindrome?
		vector<ll> tmp = b;
		reverse(all(tmp));
		if (b == tmp && count(all(b), 1) == k) ans++;

		// same size palindromes
		if (sz(b) < k) return ans;

		ll halfb = sz(b) / 2;
		if (sz(b) % 2 == 0) {
			if (k % 2 == 0) {
				ll half = k / 2;
				ll cnt = 1;
				rep(i, 1, sz(b) / 2) {
					if (b[i] == 1) {
						ll rem = halfb - i - 1;
						ll need = half - cnt;
						if (need >= 0 && need <= rem) ans += C[rem][need];
						cnt++;
					}
				}
			}
		}
		else {
			ll half = k / 2;
			if (k % 2 == 1) half = (k - 1) / 2;

			ll cnt = 1;
			rep(i, 1, sz(b) / 2) {
				if (b[i] == 1) {
					ll rem = halfb - i - 1;
					ll need = half - cnt;
					if (need >= 0 && need <= rem) ans += C[rem][need];
					cnt++;
				}
			}
		}

		// counting 5 (101) for calc(6) case
		ll onehalf = accumulate(b.begin(), b.begin() + halfb, 0LL);

		if (sz(b) % 2 == 1) {
			// mid is 0, so must check
			if (b[halfb] == 1) {
				if (2 * onehalf == k) ans++; 
			}

			ll pal = 0;
			rep(i, 0, halfb) pal = (pal << 1) | b[i];
			pal = (pal << 1) | b[halfb];
			for(int i = halfb - 1; i >= 0; i--) pal = (pal << 1) | b[i];
			if (pal < x && (2 * onehalf + b[halfb]) == k) ans++;
	    	}
	    	else {
			ll pal = 0;
			rep(i, 0, halfb) pal = (pal << 1) | b[i];
			for(int i = halfb - 1; i >= 0; i--) pal = (pal << 1) | b[i];
			if (pal < x && 2 * onehalf == k) ans++;
	    	}
		return ans;
	};

	cout << calc(r) - calc(l - 1);

}


