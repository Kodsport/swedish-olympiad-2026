#include <bits/stdc++.h>
using namespace std;

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    long long a, b;
    cin >> n >> a >> b;
    string s;
    cin >> s;

    long long best = LLONG_MAX;

    for (int c = 0; c < n; c++) {
        // Rotated string: s[c..n-1] (unshifted, cost a) + s[0..c-1] (shifted, cost a-b)
        // Standard left-to-right stack matching, tracking U vs V
        // stack: 0 = U, 1 = V
        vector<int> stack;
        int balance = 0;
        int rd_u = 0, rd_v = 0;

        for (int i = c; i < n; i++) {
            if (s[i] == '(') {
                stack.push_back(0);
                balance++;
            } else {
                if (balance > 0) {
                    stack.pop_back();
                    balance--;
                } else {
                    rd_u++;
                }
            }
        }

        for (int i = 0; i < c; i++) {
            if (s[i] == '(') {
                stack.push_back(1);
                balance++;
            } else {
                if (balance > 0) {
                    stack.pop_back();
                    balance--;
                } else {
                    rd_v++;
                }
            }
        }

        int e_u = 0, e_v = 0;
        for (int x : stack) {
            if (x == 0) e_u++;
            else e_v++;
        }

        int v_open = 0;
        for (int i = 0; i < c; i++)
            if (s[i] == '(') v_open++;

        int R = rd_u + rd_v + e_u + e_v;
        int S = min(e_u, v_open - e_v);
        long long cost = (long long)b * c + (long long)a * R - (long long)b * S;
        best = min(best, cost);
    }

    cout << best << "\n";
}
