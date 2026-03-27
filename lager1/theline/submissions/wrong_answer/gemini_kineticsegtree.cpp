#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

const long long INF = 1e18;

// Segment Tree Node to maintain moving trapezoids
struct Node {
    long long max_even;
    long long max_odd;
    long long lazy_val_even;
    long long lazy_val_odd;
    long long lazy_vel_even;
    long long lazy_vel_odd;
    
    Node() {
        max_even = -INF;
        max_odd = -INF;
        lazy_val_even = 0;
        lazy_val_odd = 0;
        lazy_vel_even = 0;
        lazy_vel_odd = 0;
    }
};

class KineticSegmentTree {
private:
    int n;
    vector<Node> tree;

    void apply_val(int node, long long val_even, long long val_odd) {
        if (tree[node].max_even != -INF) {
            tree[node].max_even += val_even;
            tree[node].lazy_val_even += val_even;
        }
        if (tree[node].max_odd != -INF) {
            tree[node].max_odd += val_odd;
            tree[node].lazy_val_odd += val_odd;
        }
    }

    void apply_vel(int node, long long vel_even, long long vel_odd) {
        tree[node].lazy_vel_even += vel_even;
        tree[node].lazy_vel_odd += vel_odd;
    }

    void push(int node) {
        // Propagate base values
        if (tree[node].lazy_val_even != 0 || tree[node].lazy_val_odd != 0) {
            apply_val(2 * node, tree[node].lazy_val_even, tree[node].lazy_val_odd);
            apply_val(2 * node + 1, tree[node].lazy_val_even, tree[node].lazy_val_odd);
            tree[node].lazy_val_even = 0;
            tree[node].lazy_val_odd = 0;
        }
        // Propagate velocities
        if (tree[node].lazy_vel_even != 0 || tree[node].lazy_vel_odd != 0) {
            apply_vel(2 * node, tree[node].lazy_vel_even, tree[node].lazy_vel_odd);
            apply_vel(2 * node + 1, tree[node].lazy_vel_even, tree[node].lazy_vel_odd);
            tree[node].lazy_vel_even = 0;
            tree[node].lazy_vel_odd = 0;
        }
    }

    void pull(int node) {
        tree[node].max_even = max(tree[2 * node].max_even, tree[2 * node + 1].max_even);
        tree[node].max_odd = max(tree[2 * node].max_odd, tree[2 * node + 1].max_odd);
    }

public:
    KineticSegmentTree(int size) {
        n = size;
        tree.resize(4 * n + 1);
    }

    // Initialize leaves to 0 based on their parity to enforce valid (x, y) coordinates
    void build(int node, int start, int end) {
        if (start == end) {
            if (start % 2 == 0) {
                tree[node].max_even = 0;
                tree[node].max_odd = -INF;
            } else {
                tree[node].max_even = -INF;
                tree[node].max_odd = 0;
            }
            return;
        }
        int mid = (start + end) / 2;
        build(2 * node, start, mid);
        build(2 * node + 1, mid + 1, end);
        pull(node);
    }

    // Add a base value to an interval
    void update_val(int node, int start, int end, int l, int r, long long val) {
        if (r < start || end < l) return;
        if (l <= start && end <= r) {
            apply_val(node, val, val);
            return;
        }
        push(node);
        int mid = (start + end) / 2;
        update_val(2 * node, start, mid, l, r, val);
        update_val(2 * node + 1, mid + 1, end, l, r, val);
        pull(node);
    }

    // Add velocity to an interval
    void update_vel(int node, int start, int end, int l, int r, long long vel) {
        if (r < start || end < l) return;
        if (l <= start && end <= r) {
            apply_vel(node, vel, vel);
            return;
        }
        push(node);
        int mid = (start + end) / 2;
        update_vel(2 * node, start, mid, l, r, vel);
        update_vel(2 * node + 1, mid + 1, end, l, r, vel);
        pull(node);
    }

    // Advance time by dx (implicitly updating values based on accumulated velocities)
    void advance_time(int node, int start, int end, long long dx) {
        if (tree[node].lazy_vel_even == 0 && tree[node].lazy_vel_odd == 0 && start != end) {
            // Optimization: if no velocity in this subtree, no need to step down
            return; 
        }
        if (start == end) {
            if (tree[node].max_even != -INF) tree[node].max_even += tree[node].lazy_vel_even * dx;
            if (tree[node].max_odd != -INF)  tree[node].max_odd += tree[node].lazy_vel_odd * dx;
            return;
        }
        push(node);
        int mid = (start + end) / 2;
        advance_time(2 * node, start, mid, dx);
        advance_time(2 * node + 1, mid + 1, end, dx);
        pull(node);
    }

    long long query_max(int parity) {
        return (parity % 2 == 0) ? tree[1].max_even : tree[1].max_odd;
    }
};

struct Event {
    int x;
    int type; // 1: start ascending, 2: peak (start descending), 3: end
    int y_center;
    int height;
    
    bool operator<(const Event& other) const {
        if (x != other.x) return x < other.x;
        return type < other.type;
    }
};

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    int N, K;
    if (!(cin >> N >> K)) return 0;

    long long original_total_distance = 0;
    vector<Event> events;

    for (int i = 0; i < K; i++) {
        long long L, R;
        cin >> L >> R;
        if (L > R) swap(L, R);
        
        original_total_distance += (R - L);
        
        long long X = L + R;
        long long Y = R - L;
        
        if (Y <= 1) continue; // No possible savings if distance is <= 1

        // Event generations corresponding to x = X - Y + 1, x = X, x = X + Y - 1
        events.push_back({(int)(X - Y + 1), 1, (int)Y, (int)(Y - 1)});
        events.push_back({(int)X, 2, (int)Y, (int)(Y - 1)});
        events.push_back({(int)(X + Y - 1), 3, (int)Y, (int)(Y - 1)});
    }

    sort(events.begin(), events.end());

    int max_y = 2 * N;
    KineticSegmentTree st(max_y);
    st.build(1, 1, max_y);

    long long max_savings = 0;
    int current_x = events.empty() ? 0 : events[0].x;

    for (const auto& ev : events) {
        if (ev.x > current_x) {
            long long dx = ev.x - current_x;
            st.advance_time(1, 1, max_y, dx);
            current_x = ev.x;
            
            // Query maximum overlapping height at current sweep line position
            max_savings = max(max_savings, st.query_max(current_x % 2));
        }

        // Interval dimensions derived from the cross-section of the pyramid
        int l = max(1, ev.y_center - ev.height);
        int r = min(max_y, ev.y_center + ev.height);

        if (ev.type == 1) {
            // Entering left triangle: velocity of the interval becomes +1
            st.update_vel(1, 1, max_y, l, r, 1);
        } else if (ev.type == 2) {
            // Peak reached: velocity changes from +1 to -1
            st.update_vel(1, 1, max_y, l, r, -2);
        } else if (ev.type == 3) {
            // Leaving right triangle: velocity returns to 0
            st.update_vel(1, 1, max_y, l, r, 1);
        }
    }

    cout << original_total_distance - max_savings << "\n";

    return 0;
}