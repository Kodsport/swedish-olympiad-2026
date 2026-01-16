using namespace std;
#include <bits/stdc++.h>

#define int long long
#define rep(i,a,b) for (int i = a; i < b; i++)


int n, b;

signed main() {
    cin >> n >> b;
    int ans = 1e9;
    int attacks = 0, strength = 1;

    int max_gain = 1;

    rep(i,0,n) {        
        if (strength >= b) {
            ans = min(ans, attacks);
            break;
        }

        int m; cin >> m;
        vector<int> health(m), xp(m);
        rep(i,0,m) cin >> health[i] >> xp[i];

        ans = min(ans, attacks + (b - strength + max_gain - 1) / max_gain);

        if (strength < health[0]) {
            int here = (health[0] - strength + max_gain - 1) / max_gain;
            strength += max_gain * here;
            attacks += here;
        }

        if (strength >= b) break;

        int x = 0, gain_here = 0;
        bool skip = false;
        while (x < m) {
            while (x < m && health[x] <= strength + gain_here) {
                gain_here += xp[x];
                x++;
                max_gain = max(max_gain, gain_here);
            }

            if (x < m) {
                ans = min(ans, attacks + (b - strength + max_gain - 1) / max_gain);
                int here = (health[x] - strength + max_gain - 1) / max_gain;
                attacks += here - 1;
                strength += max_gain * (here - 1);

                while (x < m && strength + gain_here >= health[x]) {
                    gain_here += xp[x];
                    x++;
                    max_gain = max(max_gain, gain_here);
                }

                if (x < m) {
                    attacks++;
                    strength += max_gain;
                }

                if (strength >= b) {
                    ans = min(ans, attacks);
                    break;
                }
            }
        }

        attacks++;
        strength += gain_here;
        max_gain = max(max_gain, gain_here);
        ans = min(ans, attacks + (b - strength + max_gain - 1) / max_gain);
    }
    cout << ans << "\n";
}