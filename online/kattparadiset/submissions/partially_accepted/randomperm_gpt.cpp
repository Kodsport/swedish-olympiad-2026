#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, k;
    cin >> n >> k;

    vector<string> out;
    for (int i = 0; i < k; ++i) {
        string color;
        int amount;
        cin >> color >> amount;
        for (int j = 0; j < amount; ++j)
            out.push_back(color);
    }

    auto check = [&]() -> bool {
        for (int i = 1; i < n; ++i) {
            if (out[i] == out[i - 1])
                return false;
        }
        return true;
    };

    mt19937 rng(69);
    int tries = (2e7 / n + 1);

    for (int i = 0; i < tries; ++i) {
        if (check()) {
            cout << "Ja\n";
            for (int j = 0; j < n; ++j)
                cout << out[j] << (j + 1 == n ? '\n' : ' ');
            return 0;
        }
        shuffle(out.begin(), out.end(), rng);
    }

    cout << "Nej\n";
    return 0;
}
