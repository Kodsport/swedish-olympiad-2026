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
	vector<pair<ll, ll>> b(n);
	vector<ll> pos(n);
	rep(i, 0, n) {
		cin >> a[i];
		b[i].first = a[i];
		pos[i] = i;
		b[i].second = i;
	}

	sort(all(b));
	
	vector<ll> home(n);
	rep(i, 0, n){
		home[b[i].second] = i;	
	}

	UF uf(n);

	vector<ll> deg(n);

	vector<vector<ll>> graph(n);
	rep(i, 0, n) {
		rep(j, 0, n) {
			if (i == j) continue;
			if (max(a[i], a[j]) % min(a[i], a[j]) == 0 && !uf.sameSet(i, j)) {
				uf.join(i, j);
				graph[i].push_back(j);
				graph[j].push_back(i);
				deg[i]++;
				deg[j]++;
			}
		}
	}

	map<ll, vector<ll>> comps;
	rep(i, 0, n) {
		comps[uf.find(i)].push_back(i);
	}

	bool possible = 1;
	for(auto &[_, v]: comps) {
		map<ll, ll> freq, freq2;
		for(auto &i: v) {
			freq[a[i]]++;
			freq2[b[i].first]++;
		}
		possible &= (freq == freq2);
	}

	cout << (possible ? "JA" : "NEJ") << '\n';
	if (!possible || x == 0) return 0;

	vector<ll> who(n);
	rep(i, 0, n) who[pos[i]] = i;

	vector<pair<ll, ll>> swp;

	vector<ll> leaves;
	rep(i, 0, n) {
		if (deg[i] == 1) leaves.push_back(i);
	}

	vector<bool> blocked(n);

	vector<ll> st;

	auto dfs = [&](auto &self, ll at, ll p, ll goal, vector<ll>& path) -> void {
		st.push_back(at);
		if (at == goal) {
			path = st;
		}
		for(auto &v: graph[at]) {
			if (!blocked[v] && v != p) {
				self(self, v, at, goal, path);
			}
		}
		st.pop_back();
	};

	while(!leaves.empty()) {
		vector<ll> nxt;
		vector<bool> in_nxt(n);

		for(auto &cur: leaves) {
			if (blocked[cur]) continue;
			if (pos[cur] != home[cur]) {
				vector<ll> path;
				dfs(dfs, cur, -1, who[home[cur]], path);

				reverse(all(path));
				rep(i, 0, sz(path) - 1) {
					swp.emplace_back(pos[path[i]], pos[path[i + 1]]);
					swap(who[pos[path[i]]], who[pos[path[i + 1]]]);
					swap(pos[path[i]], pos[path[i + 1]]);
				}
			}

			blocked[cur] = 1;
			for(auto &v: graph[cur]) {
				if (!blocked[v]) {
					deg[v]--;
					if (deg[v] == 1 && !in_nxt[v]) {
						nxt.push_back(v);
						in_nxt[v] = 1;
					}
				}
			}
		}
		swap(leaves, nxt);
	}
	
	cout << sz(swp) << '\n';
	for(auto &[one, two]: swp) cout << one + 1 << ' ' << two + 1 << '\n';
		
}


