#include <bits/stdc++.h>
using namespace std;

#define rep(i, a, b) for(int i = a; i < (b); ++i)
#define all(x) begin(x), end(x)
#define sz(x) (int)(x).size()
typedef long long ll;
typedef pair<int, int> pii;
typedef pair<ll, ll> pll;
typedef vector<int> vi;

#define dbg(x) cout << #x << " = " << x << endl

int main() {
	cin.tie(0)->sync_with_stdio(0);

	ll n, S;
	cin >> n >> S;

	vector<pair<ll, ll>> portals(n);
	rep(i, 0, n) {
		cin >> portals[i].first >> portals[i].second;
	}

	// graph creation
	unordered_map<ll, vector<pair<ll, ll>>> graph;
	graph[0].emplace_back(8*portals[0].first, 8*portals[0].first);
	rep(i, 0, n) {
		ll x, t;
		tie(x, t) = portals[i];
		graph[-x].emplace_back(t, 8*x);
		graph[8*x].emplace_back(t, -x);
		if (i < n - 1) {
			ll x_nxt = portals[i + 1].first;
			graph[-x].emplace_back(x_nxt - x, -x_nxt);
			graph[8*x].emplace_back(8*(x_nxt - x), 8*x_nxt);
		}
		else {
			graph[8*x].emplace_back(S - 8*x, S);
		}
	}

	// dijkstra
	unordered_set<ll> vis;
	unordered_map<ll, ll> dist;
	priority_queue<pll, vector<pll>, greater<pll>> pq;
	pq.emplace(0, 0);
	while(!pq.empty()) {
		pair<ll, ll> cur = pq.top();
		pq.pop();
		ll d, at;
		tie(d, at) = cur;

		if (vis.count(at)) continue;
		vis.insert(at);
		dist[at] = d;		

		for(auto &x: graph[at]) {
			ll weight, v;
			tie(weight, v) = x;
			if (!vis.count(v)) {
				pq.emplace(d + weight, v);
			}
		}
	}

	cout << dist[S];

}

