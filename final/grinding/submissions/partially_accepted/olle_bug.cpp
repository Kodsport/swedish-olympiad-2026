using namespace std;
#include <bits/stdc++.h>

#define int long long
#define rep(i,a,b) for (int i = a; i < b; i++)


int n, b;

signed main() {
    cin >> n >> b;
    int ans = 1e9;
    int attacks = 0, strength = 1;

    int x = 0, gain = 0;
    int max_gain = 0;

    rep(i,0,n) {
        if (strength >= b) {
            ans = min(ans, attacks);
        }
        else {
            if (gain) ans = min(ans, attacks+(b - strength + gain - 1) / gain);
        }

        int m; cin >> m;
        vector<int> health(m), xp(m);
        rep(i,0,m) cin >> health[i] >> xp[i];

        if (strength < health[0]) {
            int here = (health[0] - strength + gain - 1) / gain;
            attacks += here;
            strength += gain * here;
        }

        x = 0;
        gain = 0;
        bool attacked = false;

        while (x < m && strength + gain >= health[x]) {
            gain += xp[x];
            x++;
        }


        if (strength + gain >= b) {
            ans = min(ans, attacks + 1);
            break;
        }

        while (x < m) {
            // strength + gain * k >= arr[x]
            
            // attack until win
            if (strength >= b) ans = min(ans, attacks);
            else ans = min(ans, attacks + (b-strength+gain-1)/gain);

            int here = (health[x] - strength + gain - 1) / gain;
            if (here > 0) {
                attacks += (here-1);
                strength += gain * (here-1);
            } 
            gain += xp[x];
            x++;

            while (x < m && strength + gain >= health[x]) {
                gain += xp[x];
                x++;
            }

            attacks++;
            strength += gain;
            if (strength >= b) ans = min(ans, attacks);
            else ans = min(ans, attacks + (b-strength+gain-1)/gain);
        }
        
        if (strength >= b) ans = min(ans, attacks);
        else ans = min(ans, attacks + (b-strength+gain-1)/gain);

        strength += gain;
        attacks++;
    }
    cout << ans << "\n";
}