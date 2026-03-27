#include <iostream>
#include <vector>
#include <cmath>

using namespace std;

int main() {
    // Optimize standard I/O operations for performance
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    long long N;
    int K;
    if (!(cin >> N >> K)) return 0;

    vector<long long> s(K);
    vector<long long> t(K);
    
    long long initial_total_time = 0;

    for (int i = 0; i < K; ++i) {
        cin >> s[i] >> t[i];
        // Ensure s[i] <= t[i] for simpler distance logic
        if (s[i] > t[i]) {
            swap(s[i], t[i]);
        }
        initial_total_time += (t[i] - s[i]);
    }

    long long max_total_saved = 0;

    // Evaluate candidate edges formed by each person's exact start and end points
    for (int i = 0; i < K; ++i) {
        long long U = s[i];
        long long V = t[i];
        
        // Skip invalid/redundant edge evaluations
        if (U == V) continue;

        long long current_saved = 0;

        for (int j = 0; j < K; ++j) {
            // Distance if person j uses the new edge (U, V)
            long long dist_with_edge = abs(s[j] - U) + 1 + abs(V - t[j]);
            long long original_dist = t[j] - s[j];
            
            long long saved = original_dist - dist_with_edge;
            
            // Only add if the new route is strictly shorter
            if (saved > 0) {
                current_saved += saved;
            }
        }

        if (current_saved > max_total_saved) {
            max_total_saved = current_saved;
        }
    }

    // The answer is the initial total time minus the maximum possible time we can save
    long long minimum_total_time = initial_total_time - max_total_saved;
    
    cout << minimum_total_time << "\n";

    return 0;
}