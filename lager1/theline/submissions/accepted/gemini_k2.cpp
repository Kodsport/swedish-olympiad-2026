#include <iostream>
#include <vector>
#include <cmath>
#include <algorithm>
#include <cstdlib>

using namespace std;

int main() {
    // Optimize standard I/O operations for performance
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    int N, K;
    if (!(cin >> N >> K)) return 0;

    vector<long long> u(K), v(K);
    vector<long long> U, V;
    U.reserve(K);
    V.reserve(K);

    long long total_initial = 0;
    for (int i = 0; i < K; i++) {
        long long s, t;
        cin >> s >> t;
        u[i] = min(s, t);
        v[i] = max(s, t);
        U.push_back(u[i]);
        V.push_back(v[i]);
        total_initial += v[i] - u[i];
    }

    // Prepare sorted arrays of unique query endpoints
    sort(U.begin(), U.end());
    U.erase(unique(U.begin(), U.end()), U.end());

    sort(V.begin(), V.end());
    V.erase(unique(V.begin(), V.end()), V.end());

    int nU = U.size();
    int nV = V.size();

    vector<int> p1(K, 0), p3(K, 0), p2(K, 0);

    for (int k = 0; k < K; k++) {
        // p2[k] denotes the first index tracking where V element > v[k]
        p2[k] = upper_bound(V.begin(), V.end(), v[k]) - V.begin();
    }

    // Initialize boundary pointers p1 and p3 for the first state x = U[0]
    if (nU > 0) {
        long long x = U[0];
        for (int k = 0; k < K; k++) {
            long long C = v[k] - u[k] - 1 - abs(x - u[k]);
            long long target_L = v[k] - C;
            long long target_R = v[k] + C;
            p1[k] = lower_bound(V.begin(), V.end(), target_L) - V.begin();
            p3[k] = upper_bound(V.begin(), V.end(), target_R) - V.begin();
        }
    }

    long long max_savings = 0;
    vector<long long> delta_slope(nV + 1, 0);
    vector<long long> delta_const(nV + 1, 0);

    // Iterating cleanly through choices of edge's x parameter
    for (int i = 0; i < nU; i++) {
        long long x = U[i];
        
        // Reset diff arrays safely bounded to max updated length limits
        fill(delta_slope.begin(), delta_slope.end(), 0);
        fill(delta_const.begin(), delta_const.end(), 0);

        for (int k = 0; k < K; k++) {
            long long C = v[k] - u[k] - 1 - abs(x - u[k]);
            long long target_L = v[k] - C;
            long long target_R = v[k] + C;

            // Update region boundary iterators in amortized O(1) loop limits
            while (p1[k] > 0 && V[p1[k] - 1] >= target_L) p1[k]--;
            while (p1[k] < nV && V[p1[k]] < target_L) p1[k]++;

            while (p3[k] > 0 && V[p3[k] - 1] > target_R) p3[k]--;
            while (p3[k] < nV && V[p3[k]] <= target_R) p3[k]++;

            if (C >= 0) {
                int l = p1[k];
                int mid = p2[k];
                int r = p3[k];

                if (l < mid) {
                    delta_slope[l] += 1;
                    delta_slope[mid] -= 1;
                    delta_const[l] += C - v[k];
                    delta_const[mid] -= C - v[k];
                }
                if (mid < r) {
                    delta_slope[mid] -= 1;
                    delta_slope[r] += 1;
                    delta_const[mid] += C + v[k];
                    delta_const[r] -= C + v[k];
                }
            }
        }

        long long current_slope = 0;
        long long current_const = 0;
        
        // Evaluating all evaluated candidate y parameters in O(K) sequentially 
        for (int j = 0; j < nV; j++) {
            current_slope += delta_slope[j];
            current_const += delta_const[j];
            long long val = current_slope * V[j] + current_const;
            if (val > max_savings) {
                max_savings = val;
            }
        }
    }

    cout << total_initial - max_savings << "\n";
    return 0;
}
