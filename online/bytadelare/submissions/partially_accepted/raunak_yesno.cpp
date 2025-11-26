#include <bits/stdc++.h>
using namespace std;

#define rep(i, a, b) for(int i = a; i < (b); ++i)
#define all(x) begin(x), end(x)
#define sz(x) (int)(x).size()
typedef long long ll;
typedef pair<int, int> pii;
typedef vector<int> vi;

#define dbg(x) cout << #x << " = " << x << endl

struct UF {
	vi e;
	UF(ll n) : e(n, -1) {}
	bool sameSet(int a, int b) { return find(a) == find(b); }
	int size(int x) { return -e[find(x)]; }
	int find(int x) { return e[x] < 0 ? x : e[x] = find(e[x]); }
	bool join(int a, int b) {
		a = find(a), b = find(b);
		if (a == b) return false;
		if (e[a] > e[b]) swap(a, b);
		e[a] += e[b]; e[b] = a;
		return true;
	}
};

int main() {
	cin.tie(0)->sync_with_stdio(0);
	cin.exceptions(cin.failbit);

	ll n, x;
	cin >> n >> x;
	vector<ll> a(n);
	rep(i, 0, n) {
		cin >> a[i];
	}
	vector<ll> b = a;
	sort(all(b));

	UF uf(n);

	vector<vector<ll>> graph(n);
	rep(i, 0, n) {
		rep(j, 0, n) {
			if (i == j) continue;
			if (max(a[i], a[j]) % min(a[i], a[j]) == 0) {
				graph[i].push_back(j);
				graph[j].push_back(i);
				uf.join(i, j);
			}
		}
	}

	map<ll, vector<ll>> comps;
	rep(i, 0, n) {
		comps[uf.find(i)].push_back(i);
	}

	bool possible = 1;
	for(auto &[rep, v]: comps) {
		map<ll, ll> freq, freq2;
		for(auto &i: v) {
			freq[a[i]]++;
			freq2[b[i]]++;
		}
		possible &= (freq == freq2);
	}

	assert(x == 0);
	cout << (possible ? "JA" : "NEJ");
		
}


