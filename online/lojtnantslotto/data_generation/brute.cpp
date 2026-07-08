#include <bits/stdc++.h>
using namespace std;
// exact: for value classes 1..5, sort asc, prefix sums; DP over total value -> min cost.
int main(){
    int n; long long T; 
    if(!(cin>>n>>T)) return 0;
    vector<vector<long long>> g(6);
    for(int i=0;i<n;i++){int l;long long c;cin>>l>>c;g[l].push_back(c);}
    vector<vector<long long>> pre(6);
    int maxv=0;
    for(int v=1;v<=5;v++){sort(g[v].begin(),g[v].end());pre[v].assign(g[v].size()+1,0);
        for(size_t j=0;j<g[v].size();j++)pre[v][j+1]=pre[v][j]+g[v][j];
        maxv+=v*g[v].size();}
    const long long INF=1e18;
    vector<long long> dp(maxv+1,INF); dp[0]=0; int cur=0;
    for(int v=1;v<=5;v++){
        int cnt=g[v].size(); int nx=cur+v*cnt;
        vector<long long> nd(nx+1,INF);
        for(int k=0;k<=cnt;k++){long long c=pre[v][k];int add=v*k;
            for(int val=0;val<=cur;val++) if(dp[val]<INF){long long nc=dp[val]+c; if(nc<nd[val+add]) nd[val+add]=nc;}}
        dp=move(nd); cur=nx;
    }
    long long ans=0; for(int val=0;val<=cur;val++) if(dp[val]<=T) ans=max(ans,(long long)val);
    cout<<ans<<"\n";
}
