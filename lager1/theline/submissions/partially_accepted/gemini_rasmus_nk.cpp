#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

struct Person {
    int L, R;
};

int main() {
    // Optimize standard I/O operations for competitive programming
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    int N, K;
    if (!(cin >> N >> K)) return 0;

    vector<Person> people(K);
    long long original_total_time = 0;

    for (int i = 0; i < K; ++i) {
        int s, t;
        cin >> s >> t;
        people[i].L = min(s, t);
        people[i].R = max(s, t);
        original_total_time += (people[i].R - people[i].L);
    }

    // P[S] stores the indices of people whose path strictly contains the midpoint S/2
    vector<vector<int>> P(2 * N + 5);
    for (int i = 0; i < K; ++i) {
        if (people[i].R - people[i].L <= 1) continue;
        for (int S = 2 * people[i].L + 1; S <= 2 * people[i].R - 1; ++S) {
            P[S].push_back(i);
        }
    }

    long long max_saving = 0;

    // Sweep across all possible mid-point sums S = U + V
    for (int S = 3; S <= 2 * N - 1; ++S) {
        if (P[S].empty()) continue;

        // Y = V - U must fit within the physical grid 1 <= U < V <= N
        int max_valid_Y = min(S - 2, 2 * N - S);
        if (max_valid_Y < 1) continue;

        vector<pair<int, int>> events;
        
        // Generate the 4 derivative boundaries for the trapezoid
        for (int i : P[S]) {
            int A = S - 2 * people[i].L;
            int B = 2 * people[i].R - S;
            int C = min(A, B) - 1;

            events.push_back({1, 1});
            events.push_back({C + 1, -1});
            events.push_back({max(A, B), -1});
            events.push_back({A + B - 1, 1});
        }

        // Insert dummy events to guarantee the sweep evaluates the strict boundaries safely
        events.push_back({max_valid_Y, 0});
        if (max_valid_Y > 1) events.push_back({max_valid_Y - 1, 0});
        events.push_back({1, 0});
        events.push_back({2, 0});

        sort(events.begin(), events.end());

        long long cur_val = 0;
        long long cur_slope = 0;
        int prev_Y = events[0].first;

        for (size_t j = 0; j < events.size(); ) {
            int Y = events[j].first;
            
            // Advance value to the current Y coordinate
            cur_val += cur_slope * (Y - prev_Y);
            long long prev_slope = cur_slope;
            
            // Process all intersecting slope changes at exactly this Y
            while (j < events.size() && events[j].first == Y) {
                cur_slope += events[j].second;
                j++;
            }

            // Y and S must share parity to map back to integer coordinates U and V
            auto check = [&](int test_Y, long long val) {
                if (test_Y >= 1 && test_Y <= max_valid_Y && (test_Y % 2) == (S % 2)) {
                    if (val > max_saving) max_saving = val;
                }
            };

            // Check current peak and strictly adjacent integers to avoid skipping a parity requirement
            check(Y, cur_val);
            if (Y - 1 >= prev_Y) check(Y - 1, cur_val - prev_slope);
            check(Y + 1, cur_val + cur_slope);

            prev_Y = Y;
        }
    }

    cout << original_total_time - max_saving << "\n";

    return 0;
}