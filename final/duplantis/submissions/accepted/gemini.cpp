#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

int main() {
    // Fast I/O
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    
    int n;
    long long v;
    if (!(cin >> n >> v)) return 0;
    
    vector<long long> a(n);
    for (int i = 0; i < n; i++) {
        cin >> a[i];
    }
    
    // b array to store maximum ending world record for each length k
    // Size n + 2 is safe since L can at most be n and we assign to L + 1 at the extreme.
    vector<long long> b(n + 2, -1);
    long long L = 0; // Current maximum valid sequence length in prefix
    
    for (int i = 0; i < n; i++) {
        // Find the maximum valid length index this element could terminate.
        // Needs max length L(i-1) + 1, and also must individually be >= v + k (i.e., a[i] - v >= k)
        long long m_j = min(L + 1, a[i] - v);
        
        // If it can be a valid end for a sequence of length >= 1
        if (m_j >= 1) {
            // Cap idx to n (though L + 1 <= n + 1 in the absolute worst edge case)
            long long idx = min(m_j, (long long)n);
            b[idx] = max(b[idx], a[i]);
        }
        
        // Greedily extend the maximum length valid prefix
        if (a[i] >= v + L + 1) {
            L++;
        }
    }
    
    // Propagate the maximum upwards since an element that is valid for length m_j 
    // is intrinsically valid as a terminus for length k <= m_j.
    for (int k = n; k >= 1; k--) {
        b[k] = max(b[k], b[k + 1]);
    }
    
    // Output:
    // r: maximum number of records
    cout << L << "\n";
    
    // b_1 ... b_r: max ending values
    if (L > 0) {
        for (int i = 1; i <= L; i++) {
            cout << b[i] << (i == L ? "" : " ");
        }
        cout << "\n";
    }
    
    return 0;
}
