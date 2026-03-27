#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

struct Person {
    int L, R;
};

struct Event {
    int type; // 0: Vertical, 1: Diagonal (X+Y=C), 2: Anti-diagonal (X-Y=C)
    int idx;
    int val;
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

    // Schedule the second derivative boundary changes across Y coordinates
    vector<vector<Event>> events(2 * N + 5);

    auto add_event = [&](int Y_start, int Y_end, int type, int idx, int val) {
        if (Y_start <= Y_end) {
            if (Y_start <= N) events[Y_start].push_back({type, idx, val});
            if (Y_end + 1 <= N) events[Y_end + 1].push_back({type, idx, -val});
        }
    };

    for (int i = 0; i < K; ++i) {
        int L = people[i].L;
        int R = people[i].R;
        int H = R - L - 1;
        if (H <= 0) continue;

        // Register the exact 4 boundaries of the saving trapezoid in O(1) time
        add_event(1, 2 * H + 1, 0, 2 * L + 2, 1);
        add_event(1, 2 * H + 1, 0, 2 * R, 1);

        add_event(1, H + 1, 2, 2 * L + 1 + N, -1); 
        if (H > 0) add_event(H + 2, 2 * H + 1, 1, 2 * L + 2 * H + 3, -1);

        add_event(1, H + 1, 1, 2 * R + 1, -1);
        if (H > 0) add_event(H + 2, 2 * H + 1, 2, 2 * R - 2 * H - 1 + N, -1);
    }

    // Active state arrays for the current Y
    vector<int> cur_V(2 * N + 5, 0);
    vector<int> cur_D1(3 * N + 5, 0);
    vector<int> cur_D2(3 * N + 5, 0);
    
    long long max_saving = 0;

    for (int Y = 1; Y <= N; ++Y) {
        for (const auto& ev : events[Y]) {
            if (ev.type == 0) cur_V[ev.idx] += ev.val;
            else if (ev.type == 1) cur_D1[ev.idx] += ev.val;
            else cur_D2[ev.idx] += ev.val;
        }

        long long cur_slope = 0;
        long long cur_val = 0;

        // Double integration along X to calculate absolute heights
        for (int X = 1; X <= 2 * N; ++X) {
            int d2_val = cur_V[X];
            if (X + Y < cur_D1.size()) d2_val += cur_D1[X + Y];
            if (X - Y + N >= 0 && X - Y + N < cur_D2.size()) d2_val += cur_D2[X - Y + N];

            cur_slope += d2_val;
            cur_val += cur_slope;

            if ((X % 2) == (Y % 2)) {
                if (X >= Y + 2 && X <= 2 * N - Y) {
                    if (cur_val > max_saving) max_saving = cur_val;
                }
            }
        }
    }

    cout << original_total_time - max_saving << "\n";
    return 0;
}