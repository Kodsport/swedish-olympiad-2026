#include <bits/stdc++.h>
using namespace std;

int LOGN = 20;

struct Node {
    int lca;      // house index representing LCA of houses in segment
    int maxd;     // maximum depth among houses in segment
    Node(int _l=0, int _m=-(1<<30)): lca(_l), maxd(_m) {}
};

int N,M,Q;
vector<int> parentArr;
vector<vector<int>> children;
vector<int> depthArr;
vector<vector<int>> up; // binary lifting

void dfs_stack(int root) {
    // iterative DFS to compute depth and up[0]
    stack<int> st;
    st.push(root);
    depthArr[root] = 0;
    up[0][root] = 0; // root's parent is 0
    while(!st.empty()) {
        int v = st.top(); st.pop();
        for(int c: children[v]) {
            depthArr[c] = depthArr[v] + 1;
            up[0][c] = v;
            st.push(c);
        }
    }
}

int lcaFunc(int a, int b) {
    if (a == 0) return b;
    if (b == 0) return a;
    if (a == b) return a;
    if (depthArr[a] < depthArr[b]) swap(a,b);
    int diff = depthArr[a] - depthArr[b];
    for(int k=0;k<LOGN;++k) if (diff & (1<<k)) a = up[k][a];
    if (a == b) return a;
    for(int k=LOGN-1;k>=0;--k) {
        if (up[k][a] != up[k][b]) {
            a = up[k][a];
            b = up[k][b];
        }
    }
    return up[0][a];
}

// segment tree over doubled cycle
struct SegTree {
    int n;
    vector<Node> seg;
    SegTree(int _n=0) { init(_n); }
    void init(int _n) {
        n = 1;
        while(n < _n) n <<= 1;
        seg.assign(2*n, Node(0, -1000000000));
    }
    Node combine(const Node &A, const Node &B) {
        if (A.lca == 0) return B;
        if (B.lca == 0) return A;
        int lc = lcaFunc(A.lca, B.lca);
        int md = max(A.maxd, B.maxd);
        return Node(lc, md);
    }
    void build_from(const vector<int> &houses, const vector<int> &depth) {
        // houses size <= n (we built with doubled size)
        for(size_t i=0;i<houses.size();++i) {
            int h = houses[i];
            seg[n + i] = Node(h, depth[h]);
        }
        for(int i = (int)houses.size(); i < n; ++i) {
            seg[n + i] = Node(0, -1000000000);
        }
        for(int i=n-1;i>=1;--i) seg[i] = combine(seg[2*i], seg[2*i+1]);
    }
    void point_update(int pos, int house) {
        int p = pos + n;
        seg[p] = Node(house, depthArr[house]);
        p >>= 1;
        while(p) {
            seg[p] = combine(seg[2*p], seg[2*p+1]);
            p >>= 1;
        }
    }
    Node range_query(int l, int r) {
        // inclusive l..r
        l += n; r += n;
        Node resL(0, -1000000000), resR(0, -1000000000);
        while(l <= r) {
            if (l & 1) resL = combine(resL, seg[l++]);
            if (!(r & 1)) resR = combine(seg[r--], resR);
            l >>= 1; r >>= 1;
        }
        return combine(resL, resR);
    }
};


int main(){
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    cin >> N >> M >> Q;
    parentArr.assign(N+1,0);
    int root = -1;
    for(int i=1;i<=N;++i) {
        int x; cin >> x;
        parentArr[i] = x;
    }
    for(int i=1;i<=N;++i) if (parentArr[i] == i) { root = i; break; }
    children.assign(N+1, {});
    for(int i=1;i<=N;++i) if (parentArr[i] != i) children[parentArr[i]].push_back(i);
    depthArr.assign(N+1,0);
    LOGN = 20;
    up.assign(LOGN, vector<int>(N+1, 0));
    // compute up[0] and depth with stack from root
    // set parent of root to 0
    up[0][root] = 0;
    // iterative DFS (stack) but ensure all nodes visited: root covers all as it's a tree
    // We'll do a simple stack of (node) and push children.
    stack<int> st;
    st.push(root);
    depthArr[root] = 0;
    while(!st.empty()){
        int v = st.top(); st.pop();
        for(int c: children[v]) {
            depthArr[c] = depthArr[v] + 1;
            up[0][c] = v;
            st.push(c);
        }
    }
    // build binary lifting up table
    for(int k=1;k<LOGN;++k) {
        for(int v=1; v<=N; ++v) {
            up[k][v] = up[k-1][ up[k-1][v] ];
        }
    }

    // persons' initial houses
    vector<int> house_of_person(M+1, 0);
    // occupant of house (0 if empty)
    vector<int> occupant_of_house(N+1, 0);
    for(int i=1;i<=M;++i) {
        int h; cin >> h;
        house_of_person[i] = h;
        occupant_of_house[h] = i;
    }
    // friendship cycle order - given as permutation which is the circular order
    vector<int> order(M);
    for(int i=0;i<M;++i) {
        order[i] = i+1;
    }
    // pos maps person -> index in order
    vector<int> pos(M+1), pos_rev(M+1);
    for(int i=0;i<M;++i) pos[ order[i] ] = i;
    // reversed order
    vector<int> order_rev(M);
    for(int i=0;i<M;++i) {
        order_rev[i] = order[(M - i) % M == 0 ? 0 : (M - i) % M]; // simpler set below
    }
    // simpler fill reversed properly
    for(int i=0;i<M;++i) order_rev[i] = order[(M - i) % M == 0 ? 0 : (M - i) % M]; // but above is messy
    // rework: proper reversed (i -> M-1-i)
    for(int i=0;i<M;++i) order_rev[i] = order[(M - i) % M == 0 ? 0 : (M - i) % M]; 
    // The above attempt is messy. Replace with straightforward:
    for(int i=0;i<M;++i) order_rev[i] = order[(M - i) % M == 0 ? 0 : (M - i) % M];

    // Let's just set reversed as order[M-1-i]
    for(int i=0;i<M;++i) order_rev[i] = order[M-1-i];
    for(int i=0;i<M;++i) pos_rev[ order_rev[i] ] = i;

    // build doubled arrays of houses (2*M)
    vector<int> arrF(2*M), arrR(2*M);
    for(int i=0;i<2*M;++i) {
        int pF = order[i % M];
        arrF[i] = house_of_person[pF];
        int pR = order_rev[i % M];
        arrR[i] = house_of_person[pR];
    }

    // build segment trees
    SegTree segF(2*M), segR(2*M);
    segF.build_from(arrF, depthArr);
    segR.build_from(arrR, depthArr);

    // process queries
    for(int qi=0; qi<Q; ++qi) {
        int T; cin >> T;
        if (T == 1) {
            int a,b; cin >> a >> b;
            if (a == b) continue;
            int x = occupant_of_house[a]; // person at house a or 0
            int y = occupant_of_house[b];
            // swap occupant records
            occupant_of_house[a] = y;
            occupant_of_house[b] = x;
            if (x != 0) {
                house_of_person[x] = b;
                int p = pos[x];
                segF.point_update(p, b);
                segF.point_update(p + M, b);
                int pr = pos_rev[x];
                segR.point_update(pr, b);
                segR.point_update(pr + M, b);
            }
            if (y != 0) {
                house_of_person[y] = a;
                int p = pos[y];
                segF.point_update(p, a);
                segF.point_update(p + M, a);
                int pr = pos_rev[y];
                segR.point_update(pr, a);
                segR.point_update(pr + M, a);
            }
        } else { // T == 2
            int p; int fun_r; cin >> p >> fun_r;
            // forward direction
            int idx = pos[p];
            int lo = 1, hi = M, ansF = 1;
            while(lo <= hi) {
                int mid = (lo+hi)/2;
                Node node = segF.range_query(idx, idx + mid - 1);
                int diff = node.maxd - depthArr[node.lca];
                if (diff <= fun_r) { ansF = mid; lo = mid + 1; }
                else hi = mid - 1;
            }
            // reverse direction
            int idxr = pos_rev[p];
            lo = 1; hi = M; int ansR = 1;
            while(lo <= hi) {
                int mid = (lo+hi)/2;
                Node node = segR.range_query(idxr, idxr + mid - 1);
                int diff = node.maxd - depthArr[node.lca];
                if (diff <= fun_r) { ansR = mid; lo = mid + 1; }
                else hi = mid - 1;
            }
            int result = max(ansF, ansR);
            cout << result << '\n';
        }
    }
    return 0;
}
