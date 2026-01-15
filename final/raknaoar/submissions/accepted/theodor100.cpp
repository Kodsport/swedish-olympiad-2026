#include <bits/stdc++.h>
using namespace std;

#define ll long long
#define INF ((ll)(1e9+7))
#define fo(i, n) for(ll i=0;i<((ll)n);i++)
#define all(x) x.begin(), x.end()
#define rall(x) x.rbegin(), x.rend()

typedef pair<int, int> pii;
typedef pair<ll, ll> pl;
typedef vector<int> vi;
typedef vector<ll> vl;
typedef vector<pl> vpl;

int main() {
    cin.tie(0)->sync_with_stdio(0);

    ll n, q, k, val;
    cin >> n >> q;
    priority_queue<pl> pq;
    vpl v;
    vl am(n, 0);
    fo(i, n){
        cin >> k;
        am[i] = k;
        pq.push({k, i});
        fo(j, k){
            cin >> val;
            v.push_back({val, i});
        }
    }
    sort(all(v));
    vpl que;
    fo(i, q){
        cin >> val;
        que.push_back({val, i});
    }
    sort(all(que));
    ll index, ind, pos = 0;
    vl ans(q);
    fo(i, q){
        tie(val, index) = que[i];
        while(pos<v.size()){
            if(val<=v[pos].first) break;
            am[v[pos++].second]--;
        }
        while(1){
            tie(val, ind) = pq.top();
            pq.pop();
            val = am[ind];
            pq.push({val, ind});
            if(pq.top().second == ind) break;
        }
        ans[index]=pq.top().second;
    }
    fo(i, q){
        cout << ans[i]+1 << "\n";
    }

    return 0;
}
