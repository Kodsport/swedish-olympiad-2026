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
int timer = 0;
int tin[maxn], tout[maxn], depth[maxn];
void dfs(int u, int p, vvi& adj)
{
    depth[u] = depth[p] + 1;
    tin[u] = timer++;

    repe(e, adj[u]) if (e != p) dfs(e, u, adj);

    tout[u] = timer++;
}

bool is_ancestor(int a, int b)
{
    return tin[a] <= tin[b] && tin[b] <= tout[a];
}

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


    vi friendships(m);
    rep(i,n) friendships[i]=i;
    vi my_friend_index(m);
    rep(i, m) my_friend_index[friendships[i]] = i;
    
    MinTree tinmin(m);
    MaxTree tinmax(m);
    MaxTree depmax(m);
    rep(i, m) tinmin.update(my_friend_index[i], tin[which_house[i]]);
    rep(i, m) tinmax.update(my_friend_index[i], tin[which_house[i]]);
    rep(i, m) depmax.update(my_friend_index[i], depth[which_house[i]]);


    auto swap_houses = [&](int a, int b)
    {
        swap(who_lives[a], who_lives[b]);
        if (who_lives[a] != -1)
        {
            int person = who_lives[a];
            which_house[person] = a;
            tinmin.update(my_friend_index[person], tin[which_house[person]]);
            tinmax.update(my_friend_index[person], tin[which_house[person]]);
            depmax.update(my_friend_index[person], depth[which_house[person]]);
        }
        if (who_lives[b] != -1)
        {
            int person = who_lives[b];
            which_house[person] = b;
            tinmin.update(my_friend_index[person], tin[which_house[person]]);
            tinmax.update(my_friend_index[person], tin[which_house[person]]);
            depmax.update(my_friend_index[person], depth[which_house[person]]);
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
            int friend_ind = my_friend_index[p];

            int radius = r;
            rep(h, n)
            {
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

                        if (mn < tin[h] || mx > tout[h] || md - depth[h] > radius) {
                            hi = mid;
                        } else {
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

                        if (mn < tin[h] || mx > tout[h] || md - depth[h] > radius) {
                            hi = mid;
                        } else {
                            lo = mid;
                        }
                    }
                    best = max(best, (ll)lo);
                }

            }

            cout << best << '\n';
        }
    }

    return 0;
}
