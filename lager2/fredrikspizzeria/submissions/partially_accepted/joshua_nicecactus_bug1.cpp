// in this subtask, after removing all bridges, every component is a cycle (potentially one node)
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

const int maxn=2e5+10;
const int maxm=2e5+10;
int vis[maxn], up[maxn], depth[maxn];
int is_bridge[maxm];
void dfs(int u, int p, int dep, vector<vector<p2>>& adj) {
    up[u] = depth[u] = dep;
    vis[u] = 1;
    for (auto [e, i] : adj[u]) {
        if (e==p) continue;
        if (vis[e]) { // up edge
            up[u] = min(up[u], depth[e]);
        }
        else { // tree edge
            dfs(e,u,dep+1,adj);
            up[u] = min(up[u], up[e]);
            if (up[e] < depth[e]) {
                // covered, not bridge
            }
            else {
                // bridge
                is_bridge[i] = 1;
            }
        }
    }
}

bool get_path(int u, int p, int t, vi& path, vector<vector<p2>>& adj) {
    if (u==t) {
        return true;
    }
    for (auto [e, i] : adj[u]) {
        if (e==p) continue;
        path.push_back(i);
        if (get_path(e, u, t, path, adj)) return true;
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
    vector<tuple<int,int,int>> edges(m);
    rep(i,m) {
        int a,b;
        cin >> a >> b >> other[i];
        other[i]--;
        a--; b--;
        edges[i] = {a,b,i};
        adj[a].emplace_back(b, i);
        adj[b].emplace_back(a, i);
    }

    memset(vis, 0, sizeof(vis));
    memset(is_bridge, 0, sizeof(is_bridge));

    dfs(0,0,0,adj);

    UF uf(n);
    vector<vector<p2>> spanningtree_adj(n);
    for (auto [u,v,i] : edges) {
        if (uf.find(u) != uf.find(v)) {
            uf.merge(u,v);
            spanningtree_adj[u].emplace_back(v,i);
            spanningtree_adj[v].emplace_back(u,i);
        }
    }
    vi path1;
    vi path2;
    get_path(0,0,1,path1,spanningtree_adj);
    get_path(0,0,2,path2,spanningtree_adj);

    // We now have: all bridges, any path 0->1 and any path 0->2

    vi edge_in_p1(m), edge_in_p2(m);
    repe(u, path1) edge_in_p1[u] = 1;
    repe(u, path2) edge_in_p2[u] = 1;

    vvi cycle_edges, cycle_nodes;
    vi vis(n);
    rep(i,n) {
        if (vis[i]) continue;

        int prev = -1;
        vi edges, nodes;
        int u = i;
        while (!vis[u]) {
            vis[u]=1;
            nodes.push_back(u);
            int num_e = 0;
            int new_e = u;
            for (auto [e, eind] : adj[u]) {
                if (e==prev) continue;
                if (is_bridge[eind]) continue;
                edges.push_back(eind);
                new_e = e;
            }
            assert(num_e <= 1);
            u = new_e;
        }

        if (sz(cycle_edges)) { // dont get messed up by isolated nodes
            cycle_edges.push_back(edges);
            cycle_nodes.push_back(nodes);
        }
    }

    auto get_cycles = [&](vi& edge_in_path) {
        vector<pair<vi,vi>> sides;
        rep(cycle, sz(cycle_edges)) {
            vi& nodes = cycle_nodes[cycle];
            vi& edges = cycle_edges[cycle];
            bool any = 0;
            repe(u, nodes) any |= edge_in_path[u];
            if (!any) continue;
            vi lside, rside;

            repe(u, edges) {
                if (edge_in_path[u]) lside.push_back(u);
                else rside.push_back(u);
            }

            sides.emplace_back(lside, rside);
        }
        return sides;
    };

    
    vi forced_alive(m);
    rep(i,m) if (edge_in_p1[i] && is_bridge[i]) forced_alive[i] = 1;

    // try cutting a bridge 0 -> 2
    rep(i,m) {
        if (!edge_in_p2[i]) continue;
        if (!is_bridge[i]) continue;
        if (forced_alive[i] || forced_alive[other[i]]) continue;
        // cut i and win!
        cout << "Ja\n";
        return 0;
    }

    // we have to cut two sides of a cycle
    vector<pair<vi, vi>> cycles_p1 = get_cycles(edge_in_p1);
    vector<pair<vi, vi>> cycles_p2 = get_cycles(edge_in_p2);

    vi which_cycle(m, -1);
    vi is_left(m, -1);
    vi is_right(m, -1);
    rep(cycle, sz(cycles_p1)) {
        vi edges = cycles_p1[cycle].first;
        repe(e, edges) which_cycle[e] = cycle, is_left[e] = 1;
        edges = cycles_p1[cycle].second;
        repe(e, edges) which_cycle[e] = cycle, is_right[e] = 1;
    }

    rep(cycle, sz(cycles_p2)) {
        vi lside = cycles_p2[cycle].first;
        vi rside = cycles_p2[cycle].second;
        assert(sz(lside) && sz(rside));

        lside.erase(remove_if(all(lside), [&](int e) {
            return forced_alive[e] || forced_alive[other[e]];
        }));

        rside.erase(remove_if(all(rside), [&](int e) {
            return forced_alive[e] || forced_alive[other[e]];
        }));

        if (lside.empty() || rside.empty()) continue; // no valid cut exists in this rat cycle
        repe(e, lside) {
            int oe = other[e];
            if (which_cycle[e] == -1) {
                // We can cut e without consequence
                cout << "Ja\n";
                return 0;
            }
        }
        repe(e, rside) {
            int oe = other[e];
            if (which_cycle[e] == -1) {
                // We can cut e without consequence
                cout << "Ja\n";
                return 0;
            }
        }
        set<int> lcycles, rcycles;
        repe(e, lside) {
            int oe = other[e];
            lcycles.insert(which_cycle[oe]);
        }

        repe(e, rside) {
            int oe = other[e];
            rcycles.insert(which_cycle[oe]);
        }
        assert(sz(lcycles) && sz(rcycles));

        if (sz(lcycles) > 1 || sz(rcycles) > 1) { // cut one from each cycle
            cout << "Ja\n";
            return 0;
        }
        assert(sz(lcycles)==1 && sz(rcycles)==1);

        // all are in the same component
        int myleft[2] = {0,0}; // left,right
        int myright[2] = {0,0}; // left,right

        repe(e, lside) {
            int oe = other[e];
            myleft[0] |= is_left[oe];
            myleft[1] |= is_right[oe];
        }

        repe(e, rside) {
            int oe = other[e];
            myright[0] |= is_left[oe];
            myright[1] |= is_right[oe];
        }

        if (myleft[0] && myright[0]) { // cut two on the same side of path cycle
            cout << "Ja\n";
            return 0;
        }
        if (myleft[1] && myright[1]) { // cut two on the same side of path cycle
            cout << "Ja\n";
            return 0;
        }
    }

    cout << "Nej\n";

    return 0;
}
