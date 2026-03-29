#include <iostream>
#include <string>
#include <vector>
#include <algorithm>

using namespace std;

int main() {
    // Optimize standard I/O operations for performance
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    long long N, a, b;
    if (!(cin >> N >> a >> b)) return 0;
    string s;
    cin >> s;

    // Precompute prefix matching stats
    vector<long long> R(N + 1, 0); // Count of ')' in prefix
    vector<long long> M(N + 1, 0); // Max valid pairs in prefix
    long long open_cnt = 0, matches = 0, close_cnt = 0;
    
    for (int i = 0; i < N; ++i) {
        if (s[i] == '(') {
            open_cnt++;
        } else {
            close_cnt++;
            if (open_cnt > 0) {
                open_cnt--;
                matches++;
            }
        }
        R[i + 1] = close_cnt;
        M[i + 1] = matches;
    }

    // Precompute suffix matching stats
    vector<long long> L_prime(N + 1, 0); // Count of '(' in suffix
    vector<long long> M_prime(N + 1, 0); // Max valid pairs in suffix
    long long close_cnt_rev = 0;
    matches = 0;
    long long open_cnt_rev = 0;
    
    for (int i = N - 1; i >= 0; --i) {
        if (s[i] == ')') {
            close_cnt_rev++;
        } else {
            open_cnt_rev++;
            if (close_cnt_rev > 0) {
                close_cnt_rev--;
                matches++;
            }
        }
        L_prime[i] = open_cnt_rev;
        M_prime[i] = matches;
    }

    long long max_savings = -1;

    // Evaluate split point optimizations
    for (int p = 0; p <= N; ++p) {
        long long r = R[p];
        long long m = M[p];
        long long lp = L_prime[p];
        long long mp = M_prime[p];

        long long w_max = min(r, lp);
        vector<long long> cands = {0, w_max};
        if (r - m >= 0 && r - m <= w_max) cands.push_back(r - m);
        if (lp - mp >= 0 && lp - mp <= w_max) cands.push_back(lp - mp);

        for (long long w : cands) {
            long long cur_f = 0;
            if (a >= b) {
                cur_f = (2 * min(m, r - w) + w) * (a - b);
            } else {
                cur_f = w * (a - b);
            }
            
            long long cur_g = (2 * min(mp, lp - w) + w) * a;
            long long total = cur_f + cur_g;
            
            if (total > max_savings) {
                max_savings = total;
            }
        }
    }

    cout << (N * a - max_savings) << "\n";

    return 0;
}
