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

    // Sort by position ascending
    bool operator<(const Event& other) const {
        return pos < other.pos;
    }
};

int main() {
    // Optimize standard I/O operations for performance
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    int N, K;
    if (!(cin >> N >> K)) return 0;

    vector<Person> people(K);
    for (int i = 0; i < K; ++i) {
        cin >> people[i].L >> people[i].R;
    }

    // 1. Extract and deduplicate all valid U candidates
    vector<long long> U_cands;
    U_cands.reserve(K);
    for (int i = 0; i < K; ++i) {
        U_cands.push_back(people[i].L);
    }
    sort(U_cands.begin(), U_cands.end());
    U_cands.erase(unique(U_cands.begin(), U_cands.end()), U_cands.end());

    long long max_global_saving = 0;

    // 2. Fix U and sweep V using discrete derivatives
    for (long long U : U_cands) {
        vector<Event> events;
        events.reserve(K * 3); // Pre-allocate maximum possible events

        // Calculate saving potential and generate slope-change events
        for (const auto& p : people) {
            long long C = (p.R - p.L - 1) - abs(U - p.L);
            if (C > 0) {
                events.push_back({p.R - C, 1});   // Slope increases
                events.push_back({p.R, -2});      // Peak hit, slope drops
                events.push_back({p.R + C, 1});   // Slope normalizes to 0
            }
        }

        if (events.empty()) continue;

        sort(events.begin(), events.end());

        long long current_v = events[0].pos;
        long long current_slope = 0;
        long long current_saving = 0;

        // 3. Evaluate the exact piece-wise function
        for (size_t i = 0; i < events.size(); ) {
            long long next_v = events[i].pos;
            
            // Calculate saving at the next event point
            long long next_saving = current_saving + current_slope * (next_v - current_v);

            // Check boundaries to handle cases where the peak is cut off by U < V
            if (next_v >= U + 1) {
                // If the segment crosses U + 1, evaluate exactly at U + 1
                long long eval_v = max(current_v, U + 1LL);
                long long saving_at_eval = current_saving + current_slope * (eval_v - current_v);
                max_global_saving = max(max_global_saving, saving_at_eval);
                
                // Evaluate at the end of the current segment
                max_global_saving = max(max_global_saving, next_saving);
            }

            current_saving = next_saving;
            current_v = next_v;

            // Process all events stacked at this exact V coordinate
            while (i < events.size() && events[i].pos == current_v) {
                current_slope += events[i].slope_change;
                i++;
            }
        }
    }

    // Calculate baseline distance to output the final minimum time if required
    // long long baseline = 0;
    // for(const auto& p : people) baseline += (p.R - p.L);
    // cout << baseline - max_global_saving << "\n";
    
    // Output the maximum distance saved
    cout << max_global_saving << "\n";

    return 0;
}