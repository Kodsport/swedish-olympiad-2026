#include <bits/stdc++.h>
using namespace std;
int main(){
    int n, t, l, g;
    cin >> n >> t;
    vector<vector<int>> costs(6), dp(5*n+1, vector<int>(6, 0));
    for(int i = 0; i < n; i++){ cin >> l >> g; costs[l].push_back(g); }
    for(int i = 1; i <= 5; i++) sort(costs[i].begin(), costs[i].end());
    for(int i = 0; i <= 5*n; i++) dp[i][0] = 1e7;
    dp[0] = {0, 0, 0, 0, 0, 0};
    for(int i = 1; i <= 5*n; i++){ for(int j = 1; j <= 5; j++){
        if(i - j < 0 || dp[i-j][j] >= costs[j].size()) continue;
        if(dp[i-j][0] + costs[j][dp[i-j][j]] < dp[i][0]){ dp[i][0] = dp[i-j][0] + costs[j][dp[i-j][j]];
            for(int k = 1; k <= 5; k++) dp[i][k] = dp[i-j][k] + (j==k);  } } }
    for(int i = 5*n; i >= 0; i--){ if(dp[i][0] <= t){ cout << i; return 0; }}
}

