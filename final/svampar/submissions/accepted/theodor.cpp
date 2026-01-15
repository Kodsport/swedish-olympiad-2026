#include <bits/stdc++.h>
using namespace std;
    
    
#define INF ((ll)(1e9+7))
#define fo(i, n) for(ll i=0;i<((ll)n);i++)
#define deb(x) cout << #x << " = " << (x) << endl
#define deb2(x, y) cout << #x << " = " << (x) << ", " << #y << " = " << (y) << endl
#define pb push_back
typedef long long ll;
typedef pair<ll, ll> pl;
typedef vector<ll> vl;
    
    
int main(){
    cin.tie(0)->sync_with_stdio(0);
    // cout << fixed << setprecision(20);
    
    int n;
    cin >> n;
    vl v(n);
    fo(i, n){
        cin >> v[i];
    }
    vl seen(256, -1);
    int dup;
    fo(i, n){
        if(seen[v[i]] == -1){
            seen[v[i]] = i;
        }else{
            dup = i;
            break;
        }
    }
    cout << "9\n";
    
    vl out(n, 0);
    // v[seen[v[dup]]] ^=128;
    if(v[dup]<128) v[seen[v[dup]]]--;
    fo(i, 9){
        vector<string> res(n, "p");
        vl next = out;
        
        if(i == 0) res[dup] = "+ "+to_string(dup+1);
        else if(i<8) res[dup] = "< "+to_string(dup+1);

        if(i){
            fo(j, n){
                if(j==dup) continue;
                if(v[j]&(1<<(i-1)))res[j] = "| "+ to_string(dup+1);
            }
        } 
        if(i == 8){
            if(v[dup]>=128){
                res[dup] = "| "+to_string(seen[v[dup]]+1);
                res[seen[v[dup]]] = "| "+to_string(dup+1);
            }else if(v[dup]>=1){
                res[dup] = "+ "+to_string(seen[v[dup]]+1);
                res[seen[v[dup]]] = "+ "+to_string(seen[v[dup]]+1);
            }else{
                res[dup] = "^ "+to_string(dup+1);
                res[seen[v[dup]]] = "^ "+to_string(seen[v[dup]]+1);

            }
        }



        fo(j, n){
            cout << res[j] << "\n";
            if(res[j][0] == '+')next[j] = out[stoi(res[j].substr(2, res[j].size()-2))-1]+1;
            if(res[j][0] == '<')next[j] = out[stoi(res[j].substr(2, res[j].size()-2))-1]*2;
            if(res[j][0] == '|')next[j] |= out[stoi(res[j].substr(2, res[j].size()-2))-1];
            if(res[j][0] == '^')next[j] ^= out[stoi(res[j].substr(2, res[j].size()-2))-1];
        }
        // deb("XXXXXX");
        // swap(out, next);
        // // if(i == 1)break;

        // fo(i, n) deb2(i, out[i]);
    }


    return 0;
}