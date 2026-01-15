#include <iostream>
#include <map>
#include <vector>
#include <string>
#include <algorithm>
#include <cmath>
#include <queue>
#include <set>
#include <iomanip>
#include <unordered_map>
#include <stack>

#define all(x)  (x).begin(), (x).end()
#define rall(x) (x).rbegin(), (x).rend()

typedef long long ll;

ll linf = 1e15+1;

using namespace std;

inline void scoobydoobydoo(){
    ios::sync_with_stdio(false);
    ios_base::sync_with_stdio(false);
    cin.tie(NULL); cout.tie(NULL);
}

pair<int, int> BIT[500001];
int sum[500001];

void update(int x){
    sum[x]++;
    int start = x;
    while (x < 5e5){
        if (BIT[x].first < sum[start] || (BIT[x].first == sum[start] && start > BIT[x].second))BIT[x] = {sum[start], start};
        x += x&(~x+1);
    }
}

int query(int x){
    pair<int, int> maxi = {0, 0};
    while (x){
        if (BIT[x].first > maxi.first || (BIT[x].first == maxi.first && maxi.second < BIT[x].second))maxi = BIT[x];
        x -= x&(~x+1);
    }
    return maxi.second;
}


int main(){ 
    //freopen("a.in", "r", stdin);
    int n, m, q; cin >> n >> q;
    vector<pair<int, int>> v;
    for (int i = 0; i < n; i++){
        int k; cin >> k;
        for (int j = 0; j < k; j++){
            int a; cin >> a;
            v.push_back({a, i+1});
        }
    }
    sort(rall(v));

    vector<pair<int, int> > queries;

    for (int i = 0; i < q; i++){
        int k; cin >> k;
        queries.push_back({k, i});
    }
    

    sort(rall(queries));

    vector<pair<int, int> > ans;

    int i = 0;
    for (auto p : queries){
        // cout << p.first << " " << p.second << endl;
        while (i < v.size() && v[i].first >= p.first){
            update(v[i].second);
            i++;
            // cout << i << " ";
        }
        // cout << endl;
        ans.push_back({p.second, query(200000)});
    }

    sort(all(ans));

    for (auto x : ans){
        cout << x.second << endl;
    }


    return 0;
}