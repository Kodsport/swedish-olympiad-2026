#include <bits/stdc++.h>
using namespace std;

int main(){
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int N, Q;
    cin >> N >> Q;

    // For each country, store sorted island sizes (descending)
    vector<vector<int>> islands(N);
    for(int i = 0; i < N; i++){
        int c;
        cin >> c;
        islands[i].resize(c);
        for(int j = 0; j < c; j++){
            cin >> islands[i][j];
        }
        sort(islands[i].rbegin(), islands[i].rend());
    }

    // Merge all islands into one list: (size, country_index)
    // sorted by size descending
    vector<pair<int,int>> allIslands;
    for(int i = 0; i < N; i++){
        for(int s : islands[i]){
            allIslands.push_back({s, i});
        }
    }
    sort(allIslands.begin(), allIslands.end(), [](const pair<int,int>& a, const pair<int,int>& b){
        if(a.first != b.first) return a.first > b.first;
        return a.second > b.second; // higher index first for tie-breaking
    });

    // Read queries with original indices
    vector<pair<int,int>> queries(Q);
    for(int i = 0; i < Q; i++){
        cin >> queries[i].first; // A value
        queries[i].second = i;  // original index
    }

    // Sort queries by A descending
    vector<int> qOrder(Q);
    iota(qOrder.begin(), qOrder.end(), 0);
    sort(qOrder.begin(), qOrder.end(), [&](int a, int b){
        return queries[a].first > queries[b].first;
    });

    // Process: sweep from largest A to smallest
    // cnt[country] = number of islands with size >= current threshold
    vector<int> cnt(N, 0);
    int bestCountry = N - 1; // highest index by default (all zeros, tie -> highest index)
    int bestCount = 0;
    int ptr = 0; // pointer into allIslands

    vector<int> ans(Q);

    for(int qi = 0; qi < Q; qi++){
        int idx = qOrder[qi];
        int A = queries[idx].first;

        // Add all islands with size >= A
        while(ptr < (int)allIslands.size() && allIslands[ptr].first >= A){
            int country = allIslands[ptr].second;
            cnt[country]++;
            // Update best
            if(cnt[country] > bestCount || (cnt[country] == bestCount && country >= bestCountry)){
                bestCount = cnt[country];
                bestCountry = country;
            }
            ptr++;
        }

        ans[idx] = bestCountry + 1; // 1-indexed
    }

    for(int i = 0; i < Q; i++){
        cout << ans[i] << "\n";
    }

    return 0;
}
