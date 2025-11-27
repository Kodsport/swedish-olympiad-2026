#include <bits/stdc++.h>
using namespace std;
using pii = pair<int,int>;
const int INF = 1e9;

struct SegTree {
    int n;           // size (power of two)
    vector<int> mn, mx;
    SegTree(int sz=0) { init(sz); }
    void init(int sz) {
        n = 1;
        while (n < sz) n <<= 1;
        mn.assign(2*n, INF);
        mx.assign(2*n, -INF);
    }
    void build(const vector<int>& vals) {
        int sz = (int)vals.size();
        init(sz);
        for (int i = 0; i < sz; ++i) {
            mn[n+i] = vals[i];
            mx[n+i] = vals[i];
        }
        for (int i = n-1; i >= 1; --i) {
            mn[i] = min(mn[2*i], mn[2*i+1]);
            mx[i] = max(mx[2*i], mx[2*i+1]);
        }
    }
    // point update index i -> value v
    void update(int i, int v) {
        int p = n + i;
        mn[p] = mx[p] = v;
        p >>= 1;
        while (p) {
            mn[p] = min(mn[2*p], mn[2*p+1]);
            mx[p] = max(mx[2*p], mx[2*p+1]);
            p >>= 1;
        }
    }
    // query on [l, r] inclusive, 0-based
    pair<int,int> query(int l, int r) {
        if (l > r) return {INF, -INF};
        l += n; r += n;
        int curMin = INF, curMax = -INF;
        while (l <= r) {
            if (l & 1) { curMin = min(curMin, mn[l]); curMax = max(curMax, mx[l]); ++l; }
            if (!(r & 1)) { curMin = min(curMin, mn[r]); curMax = max(curMax, mx[r]); --r; }
            l >>= 1; r >>= 1;
        }
        return {curMin, curMax};
    }
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    int N, M, Q;
    if (!(cin >> N >> M >> Q)) return 0;
    vector<int> r_slide(N+1);
    for (int i = 1; i <= N; ++i) cin >> r_slide[i]; // vi behöver inte r_slide för linje-subtasken
    vector<int> pos(M);         // pos[person_index] = house (1..N)
    vector<int> houseToPerson(N+1, -1); // which person lives in house, -1 if empty
    for (int i = 0; i < M; ++i) {
        int h; cin >> h;
        pos[i] = h;
        houseToPerson[h] = i;
    }

    // vi arbetar med persons 0..M-1 i cykelordning
    // segtree byggs på dubbla arrayen (längd 2M) för att enkelt hantera wrap
    vector<int> vals(2*M);
    for (int i = 0; i < M; ++i) {
        vals[i] = pos[i];
        vals[i+M] = pos[i];
    }
    SegTree st;
    st.build(vals);

    for (int qi = 0; qi < Q; ++qi) {
        int T; cin >> T;
        if (T == 1) {
            int a, b; cin >> a >> b; // swap inhabitants of houses a,b
            int pa = houseToPerson[a];
            int pb = houseToPerson[b];
            // swap occupants in houseToPerson
            houseToPerson[a] = pb;
            houseToPerson[b] = pa;
            // update persons' pos and update segtree (if person exists)
            if (pa != -1) {
                pos[pa] = b;
                st.update(pa, pos[pa]);
                st.update(pa + M, pos[pa]);
            }
            if (pb != -1) {
                pos[pb] = a;
                st.update(pb, pos[pb]);
                st.update(pb + M, pos[pb]);
            }
        } else if (T == 2) {
            int p; int r; cin >> p >> r;
            // Vi antar p är person-id i [1..M] enligt problemets intention (typo i statement).
            int pidx = p - 1;

            auto feasible_forward = [&](int startIdx, int len) -> bool {
                // frågar segment [startIdx, startIdx+len-1] i duplicerad array (0..2M-1)
                int l = startIdx;
                int rpos = startIdx + len - 1;
                auto pr = st.query(l, rpos);
                int mn = pr.first;
                int mx = pr.second;
                return (mx - mn) <= r;
            };

            auto feasible_backward = [&](int endIdx, int len) -> bool {
                // segment length len som slutar i endIdx (i.e. p är sista), motsvarar start s = endIdx - (len-1) mod M
                int s = endIdx - (len - 1);
                if (s < 0) s += M;
                // om s <= endIdx -> [s, endIdx] i första block, annars -> [s, endIdx+M]
                if (s <= endIdx) {
                    auto pr = st.query(s, endIdx);
                    return (pr.second - pr.first) <= r;
                } else {
                    auto pr = st.query(s, endIdx + M);
                    return (pr.second - pr.first) <= r;
                }
            };

            // Binärsök maximal längd i forward-riktning (segment börjar i pidx)
            int lo = 1, hi = M; // min 1 (värden själv), max M
            while (lo < hi) {
                int mid = (lo + hi + 1) >> 1;
                if (feasible_forward(pidx, mid)) lo = mid; else hi = mid - 1;
            }
            int best = lo;

            // Binärsök maximal längd i backward-riktning (segment slutar i pidx)
            lo = 1; hi = M;
            while (lo < hi) {
                int mid = (lo + hi + 1) >> 1;
                if (feasible_backward(pidx, mid)) lo = mid; else hi = mid - 1;
            }
            best = max(best, lo);

            cout << best << '\n';
        }
    }
    return 0;
}
