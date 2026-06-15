#include <bits/stdc++.h>
using namespace std;
using ll = long long;
using ld = long double;
using pii = pair<int, int>;
#define pb push_back
#define ff first
#define ss second
#define arr5 array<int, 5>
const int A = 1e6;

int main(){
    ios_base::sync_with_stdio(0);
    cin.tie(0);
    cout.tie(0);

    int n, T; cin>>n>>T;
    vector<int> l(n + 1), g(n + 1);
    vector<pair<long double, int>> F;
    for (int i = 1; i <= n; i++){
        cin>>l[i]>>g[i];
        F.pb({1.0 * l[i] / g[i], i});
    }
    
    sort(F.begin(), F.end());
    reverse(F.begin(), F.end());
    
    vector<int> f;
    for (auto [x, y]: F) f.pb(y);
    
    vector<int> st[6];
    for (int i: f) st[l[i]].pb(g[i]);
    for (int i = 1; i <= 5; i++) reverse(st[i].begin(), st[i].end());
    
    vector<arr5> all;
    for (int a = 0; a <= 5; a++){
        for (int b = 0; b <= 5; b++){
            for (int c = 0; c <= 5; c++){
                for (int d = 0; d <= 5; d++){
                    for (int e = 0; e <= 5; e++){
                        all.pb({a, b, c, d, e});
                    }
                }
            }
        }
    }
    
    int sg = 0, sl = 0, out = 0;
    vector<int> s[6];
    for (int i: f){
        if ((sg + g[i]) <= T){
            sg += g[i];
            sl += l[i];
            st[l[i]].pop_back();
            s[l[i]].pb(g[i]);
        }
    }
    
    vector<vector<int>> C(all.size(), vector<int>(6));
    vector<int> L(all.size()), Gs(all.size()), Gt(all.size());
    vector<bool> I(all.size()), II(all.size());
    vector<int> mx(5 * A + 1);
    for (int i = 0; i < all.size(); i++){
        for (int j = 0; j < 5; j++){
            C[i][all[i][j]]++;
        }
        for (int j = 1; j <= 5; j++){
            L[i] += j * C[i][j];
            if (s[j].size() < C[i][j]){
                I[i] = 1;
            }
            else {
                for (int p = (int) s[j].size() - 1; p >= (int) s[j].size() - C[i][j]; p--){
                    Gs[i] += s[j][p];
                }
            }
            if (st[j].size() < C[i][j]){
                II[i] = 1;
            }
            else {
                for (int p = (int) st[j].size() - 1; p >= (int) st[j].size() - C[i][j]; p--){
                    Gt[i] += st[j][p];
                }
            }
        }
        if (!II[i]) mx[Gt[i]] = max(mx[Gt[i]], L[i]);
    }
    
    for (int i = 1; i <= 5 * A; i++) mx[i] = max(mx[i], mx[i - 1]);

    for (int p1 = 0; p1 < all.size(); p1++){
        if (I[p1]) continue;
        out = max(out, sl - L[p1] + mx[min(5 * A, T + Gs[p1] - sg)]);
    }
    
    cout<<out<<"\n";
}