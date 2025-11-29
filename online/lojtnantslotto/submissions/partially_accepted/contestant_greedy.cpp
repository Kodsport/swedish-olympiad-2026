#include <bits/stdc++.h>
using namespace std;

struct Castle {
    size_t gold;
    size_t soldiers;

    float worth() const {
        if (soldiers == 0) return numeric_limits<float>::max();
        return static_cast<float>(gold) / static_cast<float>(soldiers);
    }
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    size_t n, troops;
    cin >> n >> troops;

    vector<Castle> castles;
    castles.reserve(n);

    for (size_t i = 0; i < n; i++) {
        size_t gold, soldiers;
        cin >> gold >> soldiers;
        castles.push_back({gold, soldiers});
    }

    sort(castles.begin(), castles.end(),
         [](const Castle& a, const Castle& b) {
             return a.worth() < b.worth();
         });

    size_t gold = 0;

    // Iterate from highest worth to lowest
    for (auto it = castles.rbegin(); it != castles.rend(); ++it) {
        if (it->soldiers > troops) continue;
        gold += it->gold;
        troops -= it->soldiers;
    }

    cout << gold << "\n";
    return 0;
}
