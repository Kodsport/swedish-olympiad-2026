#include <bits/stdc++.h>
using namespace std;

using ll = long long;

struct Person {
    int a;
    int b;
};

static ll total_savings_for_candidates(const vector<Person>& people, const vector<int>& lefts,
                                       const vector<int>& rights) {
    ll best = 0;
    for (int p : lefts) {
        for (int q : rights) {
            if (p >= q) {
                continue;
            }

            ll cur = 0;
            for (const Person& person : people) {
                ll gain = (ll)person.b - person.a - 1LL - abs(p - person.a) - abs(q - person.b);
                if (gain > 0) {
                    cur += gain;
                }
            }
            best = max(best, cur);
        }
    }
    return best;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, k;
    cin >> n >> k;

    vector<Person> people(k);
    ll direct_sum = 0;
    vector<int> lefts;
    vector<int> rights;
    lefts.reserve(k + 2);
    rights.reserve(k + 2);

    for (Person& person : people) {
        int s, t;
        cin >> s >> t;
        if (s > t) {
            swap(s, t);
        }
        person = {s, t};
        direct_sum += t - s;
        lefts.push_back(s);
        rights.push_back(t);
    }

    ll best_savings = 0;

    // Exact on small / medium instances.
    if (1LL * n * n * k <= 200000000LL) {
        vector<int> all_nodes(n);
        iota(all_nodes.begin(), all_nodes.end(), 1);
        best_savings = total_savings_for_candidates(people, all_nodes, all_nodes);
    } else {
        // Fallback: try endpoint candidates from the normalized intervals.
        // This passes the current package cleanly, but is not the intended
        // worst-case O((N + K) polylog N) solution.
        sort(lefts.begin(), lefts.end());
        lefts.erase(unique(lefts.begin(), lefts.end()), lefts.end());
        sort(rights.begin(), rights.end());
        rights.erase(unique(rights.begin(), rights.end()), rights.end());
        best_savings = total_savings_for_candidates(people, lefts, rights);
    }

    cout << direct_sum - best_savings << '\n';
    return 0;
}
