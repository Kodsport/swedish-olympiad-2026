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

namespace bcc {
    ll t_m = 0;
    vi tin, low, st;
    vi br;
    vvi bccs;
    vector<vector<p2>> adj;

    void dfs(ll u, ll p = -1) {
        tin[u] = low[u] = ++t_m;
        repe(x, adj[u]) {
            ll v = x.first, id = x.second;
            if (id == p) continue;
            if (tin[v]) {
                low[u] = min(low[u], tin[v]);
                if (tin[v] < tin[u]) st.push_back(id);
            } else {
                st.push_back(id);
                dfs(v, id);
                low[u] = min(low[u], low[v]);
                if (low[v] >= tin[u]) {
                    br[id] = low[v] > tin[u];
                    vi c;
                    ll e;
                    do {
                        e = st.back();
                        st.pop_back();
                        if (e != id || !br[id]) c.push_back(e);
                    } while (e != id);
                    if (sz(c)) bccs.push_back(c);
                }
            }
        }
    }

    void find_bcc(ll n, ll m, const vector<vector<p2>>& graph) {
        adj = graph;
        tin.assign(n, 0);
        low.assign(n, 0);
        br.assign(m, false);
        st.clear();
        bccs.clear();
        t_m = 0;
        dfs(0);
        rep(i,n) assert(tin[i]>0);
    }
};

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

    bcc::find_bcc(n, m, adj);
    vvi cycle_edges = bcc::bccs;
    vi is_bridge = bcc::br;

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

    
    auto get_cycles = [&](vi& edge_in_path) {
        vector<pair<vi,vi>> sides;
        rep(cycle, sz(cycle_edges)) {
            vi& edges = cycle_edges[cycle];
            bool any = 0;
            repe(u, edges) any |= edge_in_path[u];
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
    vi is_left(m);
    vi is_right(m);
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

        // Remove those that cut bridge on my escape path
        lside.erase(remove_if(all(lside), [&](int e) {
            return forced_alive[e] || forced_alive[other[e]];
        }), lside.end());

        rside.erase(remove_if(all(rside), [&](int e) {
            return forced_alive[e] || forced_alive[other[e]];
        }), rside.end());

        set<set<p2>> lside_cuts; // edge -> which cycles do you cut, and which side
        set<set<p2>> rside_cuts; // edge -> which cycles do you cut, and which side
        repe(e, lside) {
            set<p2> cuts;
            if (which_cycle[e] != -1) {
                cuts.emplace(which_cycle[e], is_left[e]);
            }

            int oe = other[e];
            if (which_cycle[oe] != -1) {
                cuts.emplace(which_cycle[oe], is_left[oe]);
            }
            lside_cuts.insert(cuts);
        }

        repe(e, rside) {
            set<p2> cuts;
            if (which_cycle[e] != -1) {
                cuts.emplace(which_cycle[e], is_left[e]);
            }

            int oe = other[e];
            if (which_cycle[oe] != -1) {
                cuts.emplace(which_cycle[oe], is_left[oe]);
            }
            rside_cuts.insert(cuts);
        }

        
        repe(c1, lside_cuts) {
            repe(c2, rside_cuts) {
                map<int, p2> damaged_cycles;
                for (auto [cycle, left] : c1) {
                    if (left) damaged_cycles[cycle].first = 1;
                    else damaged_cycles[cycle].second = 1;
                }
                for (auto [cycle, left] : c2) {
                    if (left) damaged_cycles[cycle].first = 1;
                    else damaged_cycles[cycle].second = 1;
                }

                bool good = 1;
                for (auto [cycle, sides] : damaged_cycles) {
                    if (sides.first && sides.second) good=0;
                }
                if (!good) continue;
                cout << "Ja\n";
                return 0;
            }
        }
    }

    cout << "Nej\n";

    return 0;
}
