#include <bits/stdc++.h>
#define ll long long 
using namespace std;
pair<int,vector<int>> getBest(pair<int,vector<int>> a, pair<int,vector<int>> b){
    if(a.first < b.first) return b;
    if(a.first > b.first) return a;
    int suma = 0;
    for(auto num : a.second) suma += num;
    int sumb = 0;
    for(auto num : b.second) sumb += num;
    return suma > sumb ? b : a;
}
int main(){
    vector<vector<int>> valueArr(5);
    int n , k;
    cin >> n >> k;
    for(int i = 0; i< n; i++){
        int l,g;
        cin >> l >> g;
        valueArr[l-1].push_back(g);
    }
    for(auto& arr: valueArr){
        sort(arr.begin(),arr.end());
    }
    vector<pair<int,vector<int>>> dp(k+1,{-1,{}});
    dp[0] = {0,{-1,-1,-1,-1,-1}};
    for(int i = 0; i< k; i++){
        if(dp[i].first == -1) continue;
        for(int j = 0; j< 5; j++){
            if(valueArr[j].empty() || dp[i].second[j] +1 >= valueArr[j].size()) continue;
            if(i + valueArr[j][dp[i].second[j] +1] >= dp.size()) continue;
            vector<int> newPoses = dp[i].second;
            newPoses[j]++;
            dp[i + valueArr[j][dp[i].second[j] +1]] = getBest(dp[i + valueArr[j][dp[i].second[j] +1]],{dp[i].first + j+1, newPoses});
        }
    }
    int maxans = 0;
    for(int i = k; i>= 0; i--){
        maxans = max(maxans,dp[i].first);
    }
    cout << maxans << endl;
    return 0;
}
