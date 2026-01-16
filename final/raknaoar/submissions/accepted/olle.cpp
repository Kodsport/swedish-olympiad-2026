using namespace std;
#include <bits/stdc++.h>

#define rep(i,a,b) for(int i = a; i < b; i++)

int n, q;

int main() {
    cin >> n >> q;
    
    priority_queue<pair<int,int>> pq, best;

    vector<int> cur(n);

    rep(i,0,n) {
        int c; cin >> cur[i];
        best.push({cur[i], i});
        
        rep(j,0,cur[i]) {
            int x; cin >> x;
            pq.push({-x, i});
        }
    }

    vector<pair<int,int>> queries(q);
    rep(i,0,q) {
        cin >> queries[i].first;
        queries[i].second = i;
    }

    sort(queries.begin(), queries.end());
    vector<int> ans(q, 0);

    rep(Q,0,q) {
        while ((!pq.empty()) && -pq.top().first < queries[Q].first) {
            auto [x, i] = pq.top(); pq.pop();
            cur[i]--;
        }

        while ((!best.empty()) && best.top().first != cur[best.top().second]) {
            auto [val, i] = best.top(); best.pop();
            best.push({cur[i], i});
        }

        ans[queries[Q].second] = best.top().second;
    }

    rep(i,0,q) cout << ans[i]+1 << '\n';
}