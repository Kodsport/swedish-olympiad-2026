#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

// Fenwick Tree to maintain point counts and coordinate sums
struct BIT {
    int n;
    vector<long long> count_tree;
    vector<long long> sum_tree;

    BIT(int size) {
        n = size;
        count_tree.assign(n + 1, 0);
        sum_tree.assign(n + 1, 0);
    }

    void add(int idx, long long cnt, long long val) {
        idx++; // 1-based internal indexing
        while (idx <= n) {
            count_tree[idx] += cnt;
            sum_tree[idx] += val;
            idx += idx & -idx;
        }
    }

    long long query_count(int idx) {
        idx++;
        if (idx > n) idx = n;
        if (idx <= 0) return 0;
        long long res = 0;
        while (idx > 0) {
            res += count_tree[idx];
            idx -= idx & -idx;
        }
        return res;
    }

    long long query_sum(int idx) {
        idx++;
        if (idx > n) idx = n;
        if (idx <= 0) return 0;
        long long res = 0;
        while (idx > 0) {
            res += sum_tree[idx];
            idx -= idx & -idx;
        }
        return res;
    }

    long long query_count(int l, int r) {
        if (l > r) return 0;
        return query_count(r) - query_count(l - 1);
    }

    long long query_sum(int l, int r) {
        if (l > r) return 0;
        return query_sum(r) - query_sum(l - 1);
    }
};

struct Person {
    int L, R;
};

// Evaluates the derivative (slope) of the saving function at a specific Y
long long get_slope(int X, int Y, BIT& bit1, BIT& bit2, BIT& bit3, long long N_active) {
    long long count1 = bit1.query_count(max(0, X - Y), bit1.n);
    long long count2 = bit2.query_count(0, min(bit2.n, X + Y));
    long long count3 = bit3.query_count(0, Y);
    return N_active - count1 - count2 + count3;
}

// Evaluates the exact total saving at coordinates X and Y
long long get_value(int X, int Y, BIT& bit1, BIT& bit2, BIT& bit3, long long N_active) {
    long long val = N_active * (Y - 1);
    
    long long c1 = bit1.query_count(max(0, X - Y), bit1.n);
    long long s1 = bit1.query_sum(max(0, X - Y), bit1.n);
    val -= (Y - X) * 1LL * c1 + s1;
    
    long long c2 = bit2.query_count(0, min(bit2.n, X + Y));
    long long s2 = bit2.query_sum(0, min(bit2.n, X + Y));
    val -= (Y + X) * 1LL * c2 - s2;
    
    long long c3 = bit3.query_count(0, Y);
    long long s3 = bit3.query_sum(0, Y);
    val += Y * 1LL * c3 - s3;
    
    return val;
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

    int MAX_X = 2 * N + 5;
    vector<vector<int>> add_events(MAX_X);
    vector<vector<int>> remove_events(MAX_X);

    for (int i = 0; i < K; ++i) {
        if (people[i].R - people[i].L <= 1) continue; 
        
        // Interval must strictly contain the midpoint M = X / 2
        add_events[2 * people[i].L + 2].push_back(i);
        remove_events[2 * people[i].R - 1].push_back(i);
    }

    BIT bit1(MAX_X), bit2(MAX_X), bit3(MAX_X);
    long long N_active = 0;
    long long max_saving = 0;

    // Sweep line over X = U + V
    for (int X = 1; X <= 2 * N; ++X) {
        for (int i : add_events[X]) {
            int L = people[i].L;
            int R = people[i].R;
            bit1.add(2 * L, 1, 2 * L);
            bit2.add(2 * R, 1, 2 * R);
            bit3.add(2 * R - 2 * L - 1, 1, 2 * R - 2 * L - 1);
            N_active++;
        }
        for (int i : remove_events[X]) {
            int L = people[i].L;
            int R = people[i].R;
            bit1.add(2 * L, -1, -2 * L);
            bit2.add(2 * R, -1, -2 * R);
            bit3.add(2 * R - 2 * L - 1, -1, -(2 * R - 2 * L - 1));
            N_active--;
        }

        if (N_active > 0) {
            // Binary search the derivative along Y to locate the global peak for this X
            int low = 1, high = min(X - 2, 2 * N - X);
            int best_Y = low;
            
            while (low <= high) {
                int mid = low + (high - low) / 2;
                if (get_slope(X, mid, bit1, bit2, bit3, N_active) >= 0) {
                    best_Y = mid;
                    low = mid + 1; // Search right to find where slope drops below 0
                } else {
                    high = mid - 1;
                }
            }
            
            // Check the immediate neighborhood to ensure integer parity (X and Y must match parity to yield valid U, V)
            for (int Y = best_Y - 3; Y <= best_Y + 3; ++Y) {
                if (Y >= 1 && Y <= min(X - 2, 2 * N - X) && (Y % 2 == X % 2)) {
                    max_saving = max(max_saving, get_value(X, Y, bit1, bit2, bit3, N_active));
                }
            }
        }
    }

    cout << original_total_time - max_saving << "\n";

    return 0;
}