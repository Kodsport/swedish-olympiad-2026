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

	ll ans = 0;

	rep(k, 0, 360) {
		rep(x, 0, 361) {
			ll s = k - x * a;
			if (s % b == 0) {
				ll y = s / b;	
				if (y >= 0) {
					ans++;
					break;
				}
			}
		}
	}

	cout << ans << '\n';
}
