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

    rep(i,m) {
        rep(j,m) {
            UF uf(n);
            // remove i and j
            rep(k,m) {
                if (i==k || other[i]==k || j==k || other[j]==k) continue;
                uf.merge(edges[k].first, edges[k].second);
            }
            if (uf.find(0)==uf.find(1) && uf.find(0)!=uf.find(2)) {
                cout << "Ja\n";
                return 0;
            }
        }
    }
    cout << "Nej\n";

    return 0;
}
