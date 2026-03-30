#pragma GCC optimize("O3,unroll-loops")
#include <bits/stdc++.h>
#pragma GCC target("avx2,bmi,bmi2,lzcnt,popcnt")

using namespace std;

static const int MAXN = 200005;
static const int MAXM = 200005;

struct DSU {
    int p[MAXN];
    int sz[MAXN];

    inline void init(int n) {
        for (int i = 1; i <= n; ++i) {
            p[i] = i;
            sz[i] = 1;
        }
    }

    inline int find(int x) {
        while (p[x] != x) {
            p[x] = p[p[x]];
            x = p[x];
        }
        return x;
    }

    inline void unite(int a, int b) {
        a = find(a);
        b = find(b);
        if (a == b) return;
        if (sz[a] < sz[b]) swap(a, b);
        p[b] = a;
        sz[a] += sz[b];
    }
};

struct Edge {
    int u, v;
};

static Edge edges[MAXM + 5];
static int linkTo[MAXM + 5];
static int pid[MAXM + 5];
static int pairsArr[MAXM + 5];
static bool disabled[MAXM + 5];
static DSU dsu;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    auto start_time = chrono::steady_clock::now();

    int n, m;
    if (!(cin >> n >> m)) return 0;

    int p_cnt = 0;

    for (int i = 1; i <= m; ++i) {
        int u, v, l;
        cin >> u >> v >> l;
        edges[i] = {u, v};
        linkTo[i] = l;
    }

    // Pair IDs
    for (int i = 1; i <= m; ++i) {
        if (pid[i] == 0) {
            ++p_cnt;
            pid[i] = p_cnt;
            pid[linkTo[i]] = p_cnt;
        }
    }

    // Build list of pair IDs
    for (int i = 1; i <= p_cnt; ++i) pairsArr[i] = i;

    mt19937 rng(1337);
    shuffle(pairsArr + 1, pairsArr + p_cnt + 1, rng);

    auto check = [&]() -> bool {
        dsu.init(n);

        for (int i = 1; i <= m; ++i) {
            if (!disabled[pid[i]]) {
                dsu.unite(edges[i].u, edges[i].v);
                if (dsu.find(1) == dsu.find(3)) return false;
            }
        }

        int r1 = dsu.find(1);
        return (r1 == dsu.find(2) && r1 != dsu.find(3));
    };

    // 1) No removals needed
    if (check()) {
        cout << "Ja\n";
        return 0;
    }

    auto elapsed_ok = [&](double limit) -> bool {
        double elapsed = chrono::duration<double>(chrono::steady_clock::now() - start_time).count();
        return elapsed < limit;
    };

    int idx = 0;

    // 2) Try single pair removals
    while (elapsed_ok(0.5) && idx < p_cnt) {
        int p = pairsArr[++idx];
        disabled[p] = true;

        if (check()) {
            cout << "Ja\n";
            return 0;
        }

        disabled[p] = false;
    }

    // 3) Try random double pair removals
    while (elapsed_ok(0.95)) {
        int p1 = pairsArr[1 + (rng() % p_cnt)];
        int p2 = pairsArr[1 + (rng() % p_cnt)];

        disabled[p1] = true;
        disabled[p2] = true;

        if (check()) {
            cout << "Ja\n";
            return 0;
        }

        disabled[p1] = false;
        disabled[p2] = false;
    }

    cout << "Nej\n";
    return 0;
}