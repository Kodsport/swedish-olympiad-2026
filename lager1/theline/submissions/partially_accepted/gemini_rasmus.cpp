#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

struct Person {
    int L, R;
};

// Evaluates the maximum saving for a fixed edge length Y = V - U
long long evaluate(int Y, int N, const vector<Person>& people, vector<long long>& D2) {
    // Y must be strictly less than N for a valid edge U < V within bounds 1..N
    if (Y <= 0 || Y >= N) return 0;

    // Reset the second difference array
    fill(D2.begin(), D2.end(), 0);

    for (const auto& p : people) {
        int L = p.L;
        int R = p.R;
        long long Hi = R - L - 1;
        if (Hi <= 0) continue;

        // Ki is the fixed height cap for this specific Y
        long long Bi = min((long long)Y - 1, 2LL * (R - L) - Y - 1);
        long long Ki = min(Hi, Bi);
        if (Ki <= 0) continue;

        // Register the 4 slope changes of the trapezoid in O(1)
        D2[2 * L + 2] += 1;
        D2[2 * L + Ki + 2] -= 1;
        D2[2 * R - Ki] -= 1;
        D2[2 * R] += 1;
    }

    long long max_saving = 0;
    long long current_slope = 0;
    long long current_val = 0;

    // Double integration to extract the exact geometric heights
    for (int X = 1; X <= 2 * N; ++X) {
        current_slope += D2[X];
        current_val += current_slope;

        // X and Y must share parity to map back to valid integers U and V
        // X must be within [Y + 2, 2N - Y] to ensure U >= 1 and V <= N
        if ((X % 2) == (Y % 2)) {
            if (X >= Y + 2 && X <= 2 * N - Y) {
                if (current_val > max_saving) {
                    max_saving = current_val;
                }
            }
        }
    }

    return max_saving;
}

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

    // Shared allocation to prevent memory overhead during search iterations
    vector<long long> D2(2 * N + 5, 0);

    long long global_max_saving = 0;
    int best_Y = 1;

    // 1. Coarse Grid Search to isolate the global peak region
    int step = max(1, N / 50);
    for (int Y = 1; Y <= N; Y += step) {
        long long val = evaluate(Y, N, people, D2);
        if (val > global_max_saving) {
            global_max_saving = val;
            best_Y = Y;
        }
    }

    // 2. Ternary Search within the isolated neighborhood
    int ts_left = max(1, best_Y - step);
    int ts_right = min(N, best_Y + step);

    while (ts_right - ts_left > 2) {
        int m1 = ts_left + (ts_right - ts_left) / 3;
        int m2 = ts_right - (ts_right - ts_left) / 3;
        
        long long v1 = evaluate(m1, N, people, D2);
        long long v2 = evaluate(m2, N, people, D2);
        
        if (v1 < v2) {
            ts_left = m1;
        } else if (v1 > v2) {
            ts_right = m2;
        } else {
            ts_left = m1;
            ts_right = m2;
        }
    }

    // Evaluate the final narrowed bounds strictly
    for (int Y = ts_left; Y <= ts_right; ++Y) {
        global_max_saving = max(global_max_saving, evaluate(Y, N, people, D2));
    }

    cout << original_total_time - global_max_saving << "\n";

    return 0;
}