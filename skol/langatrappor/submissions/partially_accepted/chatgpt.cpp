#include <bits/stdc++.h>
using namespace std;
using int64 = long long;
const int64 INF64 = (int64)4e18;

struct Node {
    int x;
    int y;
    int64 d;
};
struct Cmp {
    bool operator()(Node const& a, Node const& b) const {
        return a.d > b.d;
    }
};

int main(){
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    int64 R; int C;
    if(!(cin>>R>>C)) return 0;
    vector<int64> f(C);
    for(int i=0;i<C;i++) cin>>f[i];
    int sx, sy, gx, gy;
    cin>>sx>>sy;
    cin>>gx>>gy;
    if (sy > gy) {
        swap(sx, gx);
        swap(sy, gy);
    }


    // We will store distances in a map per column: since rows visited are sparse, use unordered_map.
    vector< unordered_map<int,int64> > dist(C);
    priority_queue<Node, vector<Node>, Cmp> pq;

    auto pushState = [&](int x, int y, int64 nd){
        if(y<0 || y>=R) return;
        auto it = dist[x].find(y);
        if(it==dist[x].end() || nd < it->second){
            dist[x][y]=nd;
            pq.push({x,y,nd});
        }
    };

    // initial
    pushState(sx, sy, f[sx]-sy); // start includes start-cell height

    while(!pq.empty()){
        Node cur = pq.top(); pq.pop();
        // stale?
        auto itcur = dist[cur.x].find(cur.y);
        if(itcur==dist[cur.x].end()) continue;
        if(itcur->second != cur.d) continue;

        if(cur.x==gx && cur.y==gy){
            cout<<cur.d<<"\n";
            return 0;
        }

        int x = cur.x;
        int y = cur.y;
        int64 baseD = cur.d;

        // 1) vertical-only moves: climb k >= 1 within same column
        // cost of climbing k steps: k*(f_x - y) - k*(k+1)/2
        // we could consider the best k to reach *any* y' in this column, but we only need to push a few promising k.
        // Here we push just the immediate next row (k=1) and also compute continuous optimum for combining with horizontal moves below.
        // push k=1 (step up one)
        if(y+1 < R){
            int k=1;
            int yp = y+k;
            int64 add = (int64)k*(f[x]-y) - (int64)k*(k+1)/2;
            int64 nd = baseD + add;
            // we do not add the destination cell twice here: climbing includes the costs of new cells,
            // but note that start included f[x]-y already, so climbing adds cells above.
            // when we step into a neighbor column below we will also add neighbor's cell value.
            pushState(x, yp, nd);
        }

        // 2) horizontal moves to adjacent columns (left, right).
        for(int dir=-1; dir<=1; dir+=2){
            int nx = x+dir;
            if(nx<0 || nx>=C) continue;

            // We want k in [0, R-1-y] minimizing:
            // newD(k) = baseD + climbCost(k) + (f[nx] - (y+k))
            // climbCost(k) = k*(f[x]-y) - k*(k+1)/2
            // => newD(k) = baseD + k*(f[x]-y) - k*(k+1)/2 + f[nx] - y - k
            //         = baseD + f[nx] - y + k*(f[x]-y - 1) - k*(k+1)/2
            // treat as convex quadratic in k. We'll compute derivative and find minimizer.

            int64 A = -1; // coefficient of k^2 is -1/2 from -(k*(k+1)/2) -> -(1/2)k^2 - (1/2)k
            // We'll compute continuous derivative:
            // newD(k) = const + alpha*k + (-1/2)k^2 + (-1/2)k
            // derivative w.r.t k: alpha - k - 1/2
            // where alpha = (f[x]-y - 1)
            long double alpha = (long double)(f[x] - y - 1);
            // derivative zero => alpha - k - 0.5 = 0 => k = alpha - 0.5
            long double k_cont = alpha - 0.5L;
            // candidate integer k around that
            vector<int64> candK;
            for(int delta=-2; delta<=2; ++delta){
                int64 kc = (int64)floor(k_cont) + delta;
                if(kc < 0) continue;
                if(y + kc >= R) continue;
                candK.push_back(kc);
            }
            // always also try k=0 and k=1 (small)
            candK.push_back(0);
            if(y+1 < R) candK.push_back(1);

            sort(candK.begin(), candK.end());
            candK.erase(unique(candK.begin(), candK.end()), candK.end());
            for(int64 k : candK){
                int64 yp = y + k;
                if(yp < 0 || yp >= R) continue;
                // climbCost
                int64 climb = k * (f[x]-y) - (k*(k+1))/2;
                // horizontal step to neighbor adds neighbor's cell height at row yp
                int64 horiz = f[nx] - yp;
                int64 nd = baseD + climb + horiz;
                pushState(nx, yp, nd);
            }
        }
    }

    // If queue exhausted and goal not reached (shouldn't happen if grid connected), print dist if any
    if(dist[gx].count(gy)) cout<<dist[gx][gy]<<"\n";
    else cout<<-1<<"\n";
    return 0;
}
