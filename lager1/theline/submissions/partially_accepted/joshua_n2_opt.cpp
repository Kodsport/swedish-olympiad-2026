#include <bits/stdc++.h>
using namespace std;

using ll = long long;
using vi = vector<ll>;
using vvi = vector<vi>;
using p2 = pair<ll, ll>;
const ll inf = 1e18;

#define rep(i,n) for (ll i = 0; i < (n); i++)
#define repp(i,a,n) for (ll i = (a); i < (n); i++)
#define repe(i, arr) for (auto& i : arr)
#define all(x) begin(x),end(x)
#define sz(x) ((ll)(x).size())

struct Event {
    Event(ll x, ll delta) : x(x), delta(delta) {}
    ll x;
    ll delta;
};

int main() {
    cin.tie(0)->sync_with_stdio(0);

    int n,k;
    cin >> n >> k;
    vector<p2> people(k);
    for (auto& [a,b] : people) {
        cin >> a >> b;
        a--; b--;
        if (a>b) swap(a,b);
    }

    ll ans = 0;
    rep(i,n) {
        vector<Event> events;
        for (auto [a,b] : people) {
            ll y_f1 = abs(b-a)-1-abs(a-i);
            ll apex_f1 = b;
            // abs(x-apex_f1)+y_f1
            p2 f1_inter = {apex_f1-y_f1, min<ll>(n-1, apex_f1+y_f1)};
            
            // fn 1: (1+abs(a-i)) + abs(b-j)

            ll y_f2 = abs(b-a)-1-abs(b-i);
            ll apex_f2 = a;
            // abs(x-apex_f1)+y_f1
            p2 f2_inter = {apex_f2-y_f2, min<ll>(n-1, apex_f2+y_f2)};

            // No effect, skip
            if (y_f1 <= 0 && y_f2 <= 0) continue;

            // Only a single nonnegative one
            if (y_f1 <= 0 || y_f2 <= 0) {
                if (y_f2 > 0) {
                    swap(y_f1, y_f2);
                    swap(apex_f1, apex_f2);
                    swap(f1_inter, f2_inter);
                }
                events.emplace_back(f1_inter.first, +1);
                events.emplace_back(apex_f1, -2);
                events.emplace_back(f1_inter.second, +1);
                continue;
            }
            
            if (f1_inter.first>f2_inter.first) {
                swap(y_f1, y_f2);
                swap(apex_f1, apex_f2);
                swap(f1_inter, f2_inter);
            }

            // Dominated
            if (f2_inter.second<=f1_inter.second) {
                events.emplace_back(f1_inter.first, +1);
                events.emplace_back(apex_f1, -2);
                events.emplace_back(f1_inter.second, +1);
                continue;
            }

            // Dominated
            if (f1_inter.first == f2_inter.first) {
                if (f1_inter.second<f2_inter.second) {
                    swap(y_f1, y_f2);
                    swap(apex_f1, apex_f2);
                    swap(f1_inter, f2_inter);
                }
                events.emplace_back(f1_inter.first, +1);
                events.emplace_back(apex_f1, -2);
                events.emplace_back(f1_inter.second, +1);
                continue;
            }

            // Separate
            if (f2_inter.first >= f1_inter.second) {
                events.emplace_back(f1_inter.first, +1);
                events.emplace_back(apex_f1, -2);
                events.emplace_back(f1_inter.second, +1);

                events.emplace_back(f2_inter.first, +1);
                events.emplace_back(apex_f2, -2);
                events.emplace_back(f2_inter.second, +1);
                continue;
            }

            ll numerator = apex_f1+apex_f2+y_f1-y_f2;
            events.emplace_back(f1_inter.first, +1);
            events.emplace_back(apex_f1, -2);
            if (numerator%2==1) {
                events.emplace_back(numerator/2, +1);
                events.emplace_back((numerator+1)/2, +1);
            }
            else {
                events.emplace_back(numerator, +2);
            }

            events.emplace_back(apex_f2, -2);
            events.emplace_back(f2_inter.second, +1);
        }
        sort(all(events), [](Event a, Event b) {
            return a.x < b.x;
        });

        if (events.empty()) continue;

        ll y = 0;
        ll dy = 0;
        int k = 0;
        repp(j,events[0].x, n) {
            if (j>=0) {
                ans = max(ans, y);
            }
            while (k<sz(events) && events[k].x <= j) {
                dy += events[k].delta;
                k++;
            }
            y += dy;
        }
    }

    ans = -ans;
    for (auto [a,b] : people) ans += abs(a-b);
    cout << ans << '\n';

    return 0;
}
