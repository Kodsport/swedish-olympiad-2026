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

const int maxn = 2e5 + 10;
int timer = 0, intimer = 0;
int tin[maxn], tout[maxn], depth[maxn], intime[maxn];
vi path;
void dfs(int u, int p, vvi& adj)
{
    path.push_back(u);
    intime[u] = intimer++;
    depth[u] = depth[p] + 1;
    tin[u] = timer++;

    repe(e, adj[u]) if (e != p) dfs(e, u, adj);

    tout[u] = timer++;
}

bool is_ancestor(int a, int b)
{
    return tin[a] <= tin[b] && tin[b] <= tout[a];
}


template<class T>
struct RMQ {
    vector<vector<T>> jmp;
    RMQ(const vector<T>& V) : jmp(1, V) {
        for (int pw = 1, k = 1; pw * 2 <= sz(V); pw *= 2, ++k) {
            jmp.emplace_back(sz(V) - pw * 2 + 1);
            rep(j, sz(jmp[k]))
                jmp[k][j] = min(jmp[k - 1][j], jmp[k - 1][j + pw]);
        }
    }
    T query(int a, int b) {
        assert(a < b); // or return inf if a == b
        int dep = 31 - countl_zero((unsigned int)(b - a));
        return min(jmp[dep][a], jmp[dep][b - (1 << dep)]);
    }
};

struct LCA {
    int T = 0;
    vi time, path;
    vector<int> ret;
    RMQ<int> rmq;

    LCA(const vector<vi>& C, int root) : time(sz(C)), rmq((dfs(C, root, -1), ret)) {}
    void dfs(const vector<vi>& C, int v, int par) {
        time[v] = T++;
        for (int y : C[v]) if (y != par) {
            path.push_back(v), ret.push_back(time[v]);
            dfs(C, y, v);
        }
    }

    int lca(int a, int b) {
        if (a == -1) return b;
        if (b == -1) return a;
        if (a == b) return a;
        tie(a, b) = minmax(time[a], time[b]);
        return path[rmq.query(a, b)];
    }
    //dist(a,b){return depth[a] + depth[b] - 2*depth[lca(a,b)];}
};
LCA* g_glca = 0;

const int smallinf = 1e9;
struct MinTree
{
    int n;
    vi tree;
    MinTree(int n) : n(n), tree(n * 4, smallinf) {}
    void update(int x, int l, int r, int i, int v)
    {
        if (l == r) return void(tree[x] = v);
        int mid = (l + r) / 2;
        if (i <= mid) update(x * 2, l, mid, i, v);
        else update(x * 2 + 1, mid + 1, r, i, v);
        tree[x] = min(tree[x * 2], tree[x * 2 + 1]);
    }
    void update(int i, int v) { update(1, 0, n - 1, i, v); }
    int query(int x, int l, int r, int ql, int qr)
    {
        if (l > qr || r < ql) return smallinf;
        if (l >= ql && r <= qr) return tree[x];
        int mid = (l + r) / 2;
        return min(query(x * 2, l, mid, ql, qr), query(x * 2 + 1, mid + 1, r, ql, qr));
    }
    int query(int ql, int qr) { return query(1, 0, n - 1, ql, qr); }
};

struct MaxTree
{
    MinTree tree;
    MaxTree(int n) : tree(n) {}
    void update(int i, int v) {
        tree.update(i, -v);
    }
    int query(int ql, int qr) {
        return -tree.query(ql, qr);
    }
};

struct LCAtree
{
    int n;
    vi tree;
    LCAtree(int n) : n(n), tree(n * 4) {}
    void update(int x, int l, int r, int i, int v)
    {
        if (l == r) return void(tree[x] = v);
        int mid = (l + r) / 2;
        if (i <= mid) update(x * 2, l, mid, i, v);
        else update(x * 2 + 1, mid + 1, r, i, v);
        tree[x] = g_glca->lca(tree[x * 2], tree[x * 2 + 1]);
    }
    void update(int i, int v) { update(1, 0, n - 1, i, v); }
    int query(int x, int l, int r, int ql, int qr)
    {
        if (l > qr || r < ql) return -1;
        if (l >= ql && r <= qr) return tree[x];
        int mid = (l + r) / 2;
        return g_glca->lca(query(x * 2, l, mid, ql, qr), query(x * 2 + 1, mid + 1, r, ql, qr));
    }
    int query(int ql, int qr) { return query(1, 0, n - 1, ql, qr); }
};

int main()
{
    cin.tie(0)->sync_with_stdio(0);

    int n, m, q;
    cin >> n >> m >> q;

    vi par(n);
    rep(i, n) cin >> par[i], par[i]--;
    int root = -1;
    rep(i, n) if (par[i] == i) root = i;
    assert(root != -1);

    vvi adj(n);
    rep(i, n) if (i != root)
    {
        adj[i].push_back(par[i]);
        adj[par[i]].push_back(i);
    }
    depth[root] = -1;
    dfs(root, root, adj);
    g_glca = new LCA(adj, root);

    vi who_lives(n, -1);
    vi which_house(m);
    rep(i, m)
    {
        int h;
        cin >> h;
        h--;
        which_house[i] = h;
        who_lives[h] = i;
    }

    MinTree tinmin(m);
    MaxTree tinmax(m);
    MaxTree depmax(m);
    LCAtree lcatree(m);
    rep(i, m) tinmin.update(i, tin[which_house[i]]);
    rep(i, m) tinmax.update(i, tin[which_house[i]]);
    rep(i, m) depmax.update(i, depth[which_house[i]]);
    rep(i, m) lcatree.update(i, which_house[i]);


    auto swap_houses = [&](int a, int b)
    {
        swap(who_lives[a], who_lives[b]);
        if (who_lives[a] != -1)
        {
            int person = who_lives[a];
            which_house[person] = a;
            tinmin.update(person, tin[which_house[person]]);
            tinmax.update(person, tin[which_house[person]]);
            depmax.update(person, depth[which_house[person]]);
            lcatree.update(person, which_house[person]);
        }
        if (who_lives[b] != -1)
        {
            int person = who_lives[b];
            which_house[person] = b;
            tinmin.update(person, tin[which_house[person]]);
            tinmax.update(person, tin[which_house[person]]);
            depmax.update(person, depth[which_house[person]]);
            lcatree.update(person, which_house[person]);
        }
    };

    auto mod = [&](int ind)
    {
        ind %= m;
        if (ind < 0) ind += m;
        return ind;
    };

    auto range_min = [&](int l, int r, MinTree& T) {
        if (l <= r) return T.query(l, r);
        return min(T.query(l, m - 1), T.query(0, r));
    };
    auto range_max = [&](int l, int r, MaxTree& T) {
        if (l <= r) return T.query(l, r);
        return max(T.query(l, m - 1), T.query(0, r));
    };
    auto range_lca = [&](int l, int r, LCAtree& T) {
        if (l <= r) return T.query(l, r);
        return g_glca->lca(T.query(l, m - 1), T.query(0, r));
    };

    while (q--)
    {
        int t;
        cin >> t;

        if (t == 1)
        {
            int a, b;
            cin >> a >> b;
            a--; b--;
            swap_houses(a, b);
        }
        else
        {
            int p, r;
            cin >> p >> r;
            ll best = 0;
            p--;
            int friend_ind = p;

            int radius = r;
            {
                int start = friend_ind;
                int lo = 0;
                int hi = m + 1;
                while (lo + 1 < hi) {
                    int mid = (lo + hi) / 2;
                    int ridx = mod(start + mid - 1);
                    int mn = range_min(start, ridx, tinmin);
                    int mx = range_max(start, ridx, tinmax);
                    int md = range_max(start, ridx, depmax);
                    int h = range_lca(start, ridx, lcatree);
                    if (mn < tin[h] || mx > tout[h] || md - depth[h] > radius) {
                        hi = mid;
                    }
                    else {
                        lo = mid;
                    }
                }
                best = max(best, (ll)lo);
            }

            {
                int start = friend_ind;
                int lo = 0;
                int hi = m + 1;
                while (lo + 1 < hi) {
                    int mid = (lo + hi) / 2;
                    int lidx = mod(start - mid + 1);
                    int ridx = start;
                    int mn = range_min(lidx, ridx, tinmin);
                    int mx = range_max(lidx, ridx, tinmax);
                    int md = range_max(lidx, ridx, depmax);
                    int h = range_lca(lidx, ridx, lcatree);
                    if (mn < tin[h] || mx > tout[h] || md - depth[h] > radius) {
                        hi = mid;
                    }
                    else {
                        lo = mid;
                    }
                }
                best = max(best, (ll)lo);
            }


            cout << best << '\n';
        }
    }

    return 0;
}
