// @EXPECTED_GRADES@ AC AC TLE AC AC TLE
#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int N, K;
    cin >> N >> K;

    vector<string> names(K);
    vector<int> cnt(K);
    for (int i = 0; i < K; ++i)
        cin >> names[i] >> cnt[i];

    int maxc = 0;
    for (int c : cnt) if (c > maxc) maxc = c;

    // Correct feasibility for circular case
    if (maxc > (N) / 2) {
        cout << "Nej\n";
        return 0;
    }

    vector<int> ord(N);
    int last = -1;

    int first = -1;

    for (int i = 0; i < N; ++i) {
        int best = -1, bestVal = -1;

        // Simple O(K) linear scan, no heap
        for (int j = 0; j < K; ++j) {
            int c = cnt[j];
            if (!c || j == last) continue;
            if (c > bestVal or (j == first and c == bestVal)) { bestVal = c; best = j; }
        }

        if (best == -1) { cout << "Nej\n"; return 0; }


        if (first == -1) first = best;

        ord[i] = best;
        --cnt[best];
        last = best;
    }



    cout << "Ja\n";
    for (int i = 0; i < N; ++i) {
        if (i) cout << ' ';
        cout << names[ord[i]];
    }
    cout << '\n';
}
