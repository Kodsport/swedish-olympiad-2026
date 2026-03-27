#include <iostream>
#include <vector>
#include <cmath>
#include <algorithm>

using namespace std;

// Represents a change in slope for our piecewise continuous savings function
struct Event {
    unsigned int pos;
    int delta_slope;
};

const int MAXK=15000;
Event events[MAXK*3+5];
Event events_tmp[MAXK*3+5];
int counts[256];

// An ultra-fast Radix Sort to sort the events by their position
void radix_sort(int n) {
    for (int shift = 0; shift < 32; shift += 8) {
        for (int i = 0; i < 256; i++) counts[i] = 0;
        for (int i = 0; i < n; i++) counts[(events[i].pos >> shift) & 255]++;
        for (int i = 1; i < 256; i++) counts[i] += counts[i - 1];
        for (int i = n - 1; i >= 0; i--) {
            events_tmp[--counts[(events[i].pos >> shift) & 255]] = events[i];
        }
        for (int i = 0; i < n; i++) events[i] = events_tmp[i];
    }
}

int main() {
    // Fast I/O
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    int N, K;
    if (!(cin >> N >> K)) return 0;

    long long initial_sum = 0;
    vector<pair<int, int>> paths;
    
    for (int i = 0; i < K; i++) {
        int s, t;
        cin >> s >> t;
        if (s > t) swap(s, t); // Normalize left-to-right travels
        initial_sum += t - s;
        if (t - s > 1) {
            // A shortcut can only help if the initial distance is strictly > 1
            paths.push_back({s, t});
        }
    }

    int num_paths = paths.size();
    if (num_paths == 0) {
        cout << initial_sum << "\n";
        return 0;
    }

    // Extrapolating the possible optimal left coordinates (u's) 
    vector<int> unique_u;
    for (int i = 0; i < num_paths; i++) {
        unique_u.push_back(paths[i].first);
    }
    sort(unique_u.begin(), unique_u.end());
    unique_u.erase(unique(unique_u.begin(), unique_u.end()), unique_u.end());

    long long max_savings = 0;

    for (int j = 0; j < unique_u.size(); j++) {
        int u = unique_u[j];
        int num_events = 0;

        for (int i = 0; i < num_paths; i++) {
            int s_i = paths[i].first;
            int t_i = paths[i].second;
            int C_i = t_i - s_i - 1 - abs(u - s_i);

            // If C_i is strictly positive, an intersecting range presents a benefit
            if (C_i > 0) {
                events[num_events++] = {(unsigned int)(t_i - C_i), 1};
                events[num_events++] = {(unsigned int)t_i, -2};
                events[num_events++] = {(unsigned int)(t_i + C_i), 1};
            }
        }

        if (num_events == 0) continue;

        radix_sort(num_events);

        long long cur_val = 0;
        long long cur_slope = 0;
        unsigned int last_pos = events[0].pos;

        // Sweep-line execution over sorted boundary events
        for (int i = 0; i < num_events; i++) {
            unsigned int pos = events[i].pos;
            cur_val += cur_slope * (long long)(pos - last_pos);
            last_pos = pos;
            cur_slope += events[i].delta_slope;

            if (cur_val > max_savings) {
                max_savings = cur_val;
            }
        }
    }

    // Minimum sum of traveling time strictly applies our mapped maximal savings
    cout << initial_sum - max_savings << "\n";

    return 0;
}
