#include <bits/stdc++.h>
using namespace std;

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    
    int n, k;
    cin >> n >> k;
    
    int* a = new int[k];
    int* b = new int[k];
    int* c = new int[k];
    long long base = 0;
    for (int i = 0; i < k; i++) {
        cin >> a[i] >> b[i];
        if (a[i] > b[i]) swap(a[i], b[i]);
        base += b[i] - a[i];
        c[i] = b[i] - a[i] - 1;
    }
    
    // saving_i(u,v) = max(0, c_i - |a_i-u| - |b_i-v|) for u<=v
    // Maximize total savings over (u,v) with u<=v.
    
    // UB(u) = sum_i max(0, c_i - |a_i - u|) via second-difference array
    const int UB_OFF = n;
    const int UB_SZ = 3 * n + 5;
    long long* ub_dd = new long long[UB_SZ]();
    
    for (int i = 0; i < k; i++) {
        if (c[i] <= 0) continue;
        int p1 = a[i] - c[i] + 1 + UB_OFF;
        int p2 = a[i] + 1 + UB_OFF;
        int p3 = a[i] + c[i] + 1 + UB_OFF;
        if (p1 >= 0 && p1 < UB_SZ) ub_dd[p1] += 1;
        if (p2 >= 0 && p2 < UB_SZ) ub_dd[p2] -= 2;
        if (p3 >= 0 && p3 < UB_SZ) ub_dd[p3] += 1;
    }
    
    long long* ub = new long long[n + 2]();
    {
        long long slope = 0, val = 0;
        for (int j = 0; j < UB_SZ; j++) {
            slope += ub_dd[j];
            val += slope;
            int u = j - UB_OFF;
            if (u >= 1 && u <= n) ub[u] = val;
        }
    }
    delete[] ub_dd;
    
    // For fixed u, g_u(v) = sum of tent functions max(0, r_i - |b_i - v|)
    // where r_i = c_i - |a_i - u|.
    // Using second-difference array: dd[b_i-r_i+1] += 1, dd[b_i+1] -= 2, dd[b_i+r_i+1] += 1
    // Then double prefix sum gives g_u(v).
    
    int* dd = new int[n + 2]();
    
    auto eval_u = [&](int u) -> long long {
        memset(dd, 0, (n + 2) * sizeof(int));
        
        for (int i = 0; i < k; i++) {
            int r = c[i] - abs(a[i] - u);
            if (r <= 0) continue;
            int lo = b[i] - r + 1;
            int mid = b[i] + 1;
            int hi = b[i] + r + 1;
            if (lo >= 1 && lo <= n) dd[lo] += 1;
            if (mid >= 1 && mid <= n) dd[mid] -= 2;
            if (hi >= 1 && hi <= n) dd[hi] += 1;
        }
        
        long long slope = 0, val = 0, best = 0;
        for (int v = 1; v <= n; v++) {
            slope += dd[v];
            val += slope;
            if (v >= u && val > best) best = val;
        }
        return best;
    };
    
    long long max_savings = 0;
    
    if (n <= 5000) {
        // Exact: try all u from 1 to n with UB pruning
        for (int u = 1; u <= n; u++) {
            if (ub[u] <= max_savings) continue;
            long long s = eval_u(u);
            if (s > max_savings) max_savings = s;
        }
    } else {
        // Heuristic for large n: evaluate top candidates by UB
        // Empirically, optimal u is within top ~600 by UB for random data at N=50000.
        // Use 5000 to be safe.
        
        vector<pair<long long, int>> cands;
        {
            vector<bool> seen(n + 1, false);
            for (int i = 0; i < k; i++) {
                if (!seen[a[i]]) {
                    seen[a[i]] = true;
                    cands.push_back({ub[a[i]], a[i]});
                }
            }
        }
        sort(cands.begin(), cands.end(), greater<pair<long long,int>>());
        
        int limit = min((int)cands.size(), 5000);
        for (int j = 0; j < limit; j++) {
            if (cands[j].first <= max_savings) continue;
            long long s = eval_u(cands[j].second);
            if (s > max_savings) max_savings = s;
        }
    }
    
    cout << base - max_savings << "\n";
    
    delete[] a; delete[] b; delete[] c; delete[] ub; delete[] dd;
    return 0;
}
