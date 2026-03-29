#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    int n;
    long long s;
    if (!(cin >> n >> s)) return 0;

    long long timeO = 0;
    long long timeN = 1e18; // representing infinity
    long long prev_x = 0;

    for (int i = 0; i < n; ++i) {
        long long x, t;
        cin >> x >> t;

        long long distN = x - prev_x;
        long long distO = 8 * distN;

        long long walkO = timeO + distO;
        long long walkN = timeN + distN;

        long long new_timeO = min(walkO, walkN + t);
        long long new_timeN = min(walkN, new_timeO + t);

        timeO = new_timeO;
        timeN = new_timeN;
        prev_x = x;
    }

    long long ans = timeO + (s - 8 * prev_x);
    cout << ans << "\n";

    return 0;
}
