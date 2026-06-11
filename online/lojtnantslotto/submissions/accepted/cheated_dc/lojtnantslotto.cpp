#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

const long long INF = 1e18;
const int MAX_VAL = 1000005;
long long dp[MAX_VAL];

vector<long long> old_dp, new_dp, cost;

void solve(int L, int R, int optL, int optR) 
{
    if (L > R) return;

    int mid = (L + R) / 2;
    int best_k = -1;
    new_dp[mid] = INF;

    int min_k = max(optL, mid - (int)cost.size() + 1);
    int max_k = min(optR, mid);

    for (int k = min_k; k <= max_k; k++) 
    {
        long long val = old_dp[k] + cost[mid - k];
        if (val < new_dp[mid]) 
        {
            new_dp[mid] = val;
            best_k = k;
        }
    }

    solve(L, mid - 1, optL, best_k);
    solve(mid + 1, R, best_k, optR);
}

int main() 
{
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, t;
    cin >> n >> t;
    
    vector<int> items[6];
    


    for (int k = 0; k < n; k++)
    {
        int gold;
        int guards;
        cin >> gold >> guards;
        if(guards <= t)
            items[gold].push_back(guards);
    }

    for (int v = 1; v <= 5; v++) 
    {
        sort(items[v].begin(), items[v].end());
    }

    fill(dp, dp + MAX_VAL, INF);
    dp[0] = 0;
    int max_current_val = 0;

    for (int v = 1; v <= 5; v++) 
    {
        if (items[v].empty()) 
            continue;


        cost = { 0 };
        for (int w : items[v]) 
            cost.push_back(cost.back() + w);

        int added_val = items[v].size() * v;
        int limit = min(MAX_VAL - 1, max_current_val + added_val);

        for (int r = 0; r < v; r++) 
        {
            old_dp.clear();
            for (int j = r; j <= max_current_val; j += v) 
            {
                old_dp.push_back(dp[j]);
            }

            if (old_dp.empty()) 
                continue;


            int new_size = (limit - r) / v + 1;
            new_dp.assign(new_size, INF);


            solve(0, new_size - 1, 0, old_dp.size() - 1);


            for (int i = 0; i < new_size; i++) 
            {
                int real_idx = r + i * v;
                if (new_dp[i] < dp[real_idx]) 
                {
                    dp[real_idx] = new_dp[i];
                }
            }
        }
        max_current_val = limit;
    }


    int ans = 0;
    for (int v = max_current_val; v >= 0; v--) 
    {
        if (dp[v] <= t) 
        {
            ans = v;
            break;
        }
    }

    cout << ans << endl;


    return 0;
}