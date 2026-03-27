#include <bits/stdc++.h>
using namespace std;

using ll = long long;
using vi = vector<ll>;
using vvi = vector<vi>;
using p2 = pair<ll, ll>;
const ll inf = 1e18;

#define rep(i,n) for (ll i = 0; i < (n); i++)
#define repp(i,a,n) for (ll i = (a); i < (n); i++)
#define repe(i, arr) for (auto& i : arr)
#define all(x) begin(x),end(x)
#define sz(x) ((ll)(x).size())

struct UF {
    vi par;
    UF(int n) : par(n) {
        iota(all(par), 0);
    }
    int find(int x) {return par[x]==x?x:par[x]=find(par[x]);}
    void merge(int a, int b) {
        a=find(a); b=find(b);
        par[b]=a;
    }
};

bool get_path(int u, int p, vi& path, vector<vector<p2>>& adj) {
    if (u==1) {
        return true;
    }
    for (auto [e, i] : adj[u]) {
        if (e==p) continue;
        path.push_back(i);
        if (get_path(e, u, path, adj)) return true;
        path.pop_back();
    }
    return false;
}

int main() {
    cin.tie(0)->sync_with_stdio(0);

    int n, m;
    cin >> n >> m;

    vector<vector<p2>> adj(n);
    vi other(m);
    vector<p2> edges(m);
    rep(i,m) {
        int a,b;
        cin >> a >> b >> other[i];
        other[i]--;
        a--; b--;
        edges[i] = {a,b};
        adj[a].emplace_back(b, i);
        adj[b].emplace_back(a, i);
    }

    vi vis(n);
    vi path;
    get_path(0, 0, path, adj);

    UF uf(n);
    repe(eind, path) {
        uf.merge(edges[eind].first, edges[eind].second);
        int oind = other[eind];
        uf.merge(edges[oind].first, edges[oind].second);
    }
    assert(uf.find(0)==uf.find(1));
    cout << (uf.find(0)==uf.find(2) ? "Nej" : "Ja") << '\n';

    return 0;
}
