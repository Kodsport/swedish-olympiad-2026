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

struct State {
    int a,b,parb,sizea;
};

struct UF {
    vector<State> states;
    vector<int> par, size;
    UF(int n) : par(n), size(n,1) {
        iota(all(par), 0);
    }
    int find(int x) {return par[x]==x?x:find(par[x]);}
    void merge(int a, int b) {
        a=find(a); b=find(b);
        if (size[a]<size[b]) swap(a,b);
        states.push_back({a,b,par[b],size[a]});
        if (a!=b) {
            par[b]=a;
            size[a]+=size[b];
        }
    }
    void undo() {
        auto [a,b,parb,sizea] = states.back();
        states.pop_back();
        par[b] = parb;
        size[a] = sizea;
    }
};

// invariant: we have added (l,r)
void dc(int l, int r, vi& candidates, vi& canonical, vi& other, vector<p2>& edges, UF& uf) {
    if (l==r) {
        if (uf.find(0)==uf.find(1) && uf.find(0)!=uf.find(2)) {
            cout << "Ja\n";
            exit(0);
        }
        return;
    }

    int mid = (l+r)/2;

    // left half
    repp(i,l,mid+1) {
        int e = canonical[i];
        int eother = other[e];
        uf.merge(edges[e].first, edges[e].second);
        uf.merge(edges[eother].first, edges[eother].second);
    }

    dc(mid+1, r, candidates, canonical, other, edges, uf);

    repp(i,l,mid+1) {
        uf.undo();
        uf.undo();
    }

    // right half
    repp(i,mid+1, r+1) {
        int e = canonical[i];
        int eother = other[e];
        uf.merge(edges[e].first, edges[e].second);
        uf.merge(edges[eother].first, edges[eother].second);
    }

    dc(l, mid, candidates, canonical, other, edges, uf);

    repp(i,mid+1, r+1) {
        uf.undo();
        uf.undo();
    }
}

int main() {
    cin.tie(0)->sync_with_stdio(0);

    int n, m;
    cin >> n >> m;

    vi other(m);
    vector<p2> edges(m);
    rep(i,m) {
        int a,b;
        cin >> a >> b >> other[i];
        other[i]--;
        a--; b--;
        edges[i] = {a,b};
    }

    // For each edge pair, mark one as canonical
    vi canonical;
    vi del(m);
    rep(i,m) {
        if (del[i]) continue;
        del[i]=1;
        del[other[i]] = 1;
        canonical.push_back(i);
    }
    assert(sz(canonical)==m/2);


    rep(i,m/2) {

        // Check if a single bridge suffices
        {
            UF uf(n);
            rep(j,m/2) {
                if (i==j) continue;
                int e = canonical[j];
                int eother = other[e];
                uf.merge(edges[e].first, edges[e].second);
                uf.merge(edges[eother].first, edges[eother].second);
            }
            if (uf.find(0)==uf.find(1) && uf.find(0)!=uf.find(2)) {
                cout << "Ja\n";
                return 0;
            }
        }
    
        vi candidates;
        rep(j,m/2) {
            if (i==j) continue;
            candidates.push_back(canonical[j]);
        }

        UF uf(n);
        dc(0, sz(candidates)-1, candidates, canonical, other, edges, uf);
    }
    cout << "Nej\n";

    return 0;
}
