#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>

using namespace std;

struct Person {
    int L, R;
};

int main() {
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

    long long max_saving = 0;
    
    // MICRO-OPTIMIZATION: D2 uses 'int' (4 bytes) instead of 'long long' (8 bytes), 
    // halving the array's memory footprint.
    vector<int> D2(N + 5, 0);

    for (int U = 1; U <= N; ++U) {
        // Reset the linear memory block for the next sweep
        fill(D2.begin(), D2.end(), 0);

        for (int i = 0; i < K; ++i) {
            int L = people[i].L;
            int R = people[i].R;
            
            // Calculate the peak height of the saving triangle for this U
            int C = R - L - 1 - abs(U - L);
            
            if (C > 0) {
                int left_bound = R - C + 1;
                int mid_bound = R + 1;
                int right_bound = R + C + 1;
                
                if (left_bound <= N) D2[max(1, left_bound)] += 1;
                if (mid_bound <= N) D2[mid_bound] -= 2;
                if (right_bound <= N) D2[right_bound] += 1;
            }
        }

        // cur_slope stays 'int' to match D2
        int cur_slope = 0;
        long long cur_val = 0;

        for (int V = 1; V <= N; ++V) {
            cur_slope += D2[V];
            
            // Implicit promotion to long long prevents overflow on the total sum
            cur_val += cur_slope; 
            
            if (V > U && cur_val > max_saving) {
                max_saving = cur_val;
            }
        }
    }

    cout << original_total_time - max_saving << "\n";
    return 0;
}