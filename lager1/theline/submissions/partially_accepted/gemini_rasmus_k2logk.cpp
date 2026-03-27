#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>

using namespace std;

struct Person {
    long long L, R;
};

struct Event {
    long long pos;
    long long slope_change;

    bool operator<(const Event& other) const {
        return pos < other.pos;
    }
};

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    long long N, K;
    if (!(cin >> N >> K)) return 0;

    vector<Person> people(K);
    long long baseline_distance = 0;

    for (int i = 0; i < K; ++i) {
        long long u, v;
        cin >> u >> v;
        // FIX 1: Normalize directionality
        people[i].L = min(u, v);
        people[i].R = max(u, v);
        baseline_distance += (people[i].R - people[i].L);
    }

    vector<long long> U_cands;
    U_cands.reserve(K);
    for (int i = 0; i < K; ++i) {
        U_cands.push_back(people[i].L);
    }
    sort(U_cands.begin(), U_cands.end());
    U_cands.erase(unique(U_cands.begin(), U_cands.end()), U_cands.end());

    long long max_global_saving = 0;

    for (long long U : U_cands) {
        if (U >= N) continue; // U must be strictly less than N

        vector<Event> events;
        events.reserve(K * 3); 

        for (const auto& p : people) {
            long long C = (p.R - p.L - 1) - abs(U - p.L);
            if (C > 0) {
                events.push_back({p.R - C, 1});   
                events.push_back({p.R, -2});      
                events.push_back({p.R + C, 1});   
            }
        }

        if (events.empty()) continue;
        sort(events.begin(), events.end());

        long long current_v = events[0].pos;
        long long current_slope = 0;
        long long current_saving = 0;

        for (size_t i = 0; i < events.size(); ) {
            long long next_v = events[i].pos;
            long long next_saving = current_saving + current_slope * (next_v - current_v);

            // Evaluate valid domain: U + 1 <= V <= N
            if (next_v >= U + 1 && current_v <= N) {
                // Clamp evaluation window
                long long start_v = max(current_v, U + 1LL);
                long long end_v = min(next_v, N);
                
                if (start_v <= end_v) {
                    long long saving_at_start = current_saving + current_slope * (start_v - current_v);
                    long long saving_at_end = current_saving + current_slope * (end_v - current_v);
                    
                    max_global_saving = max(max_global_saving, saving_at_start);
                    max_global_saving = max(max_global_saving, saving_at_end);
                }
            }

            current_saving = next_saving;
            current_v = next_v;

            while (i < events.size() && events[i].pos == current_v) {
                current_slope += events[i].slope_change;
                i++;
            }
        }
    }

    // FIX 2: Output required format (Baseline - Savings)
    cout << baseline_distance - max_global_saving << "\n";

    return 0;
}