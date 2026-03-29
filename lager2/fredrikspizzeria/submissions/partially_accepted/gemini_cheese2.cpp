#pragma GCC optimize("O3,unroll-loops")
#include <bits/allocator.h>
#pragma GCC target("avx2,bmi,bmi2,lzcnt,popcnt")

#include <iostream>
#include <vector>
#include <numeric>
#include <random>
#include <chrono>
#include <algorithm>

using namespace std;

const int MAXN = 200005;

struct Edge {
    int to;
    int pid; // Pair ID
};

vector<Edge> adj[MAXN];
int vis[MAXN];
int timer_vis = 0;
int q[MAXN];
bool disabled[MAXN];

int main() {
    // Insane Speed I/O
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    
    auto start_time = chrono::steady_clock::now();

    int n, m;
    if (!(cin >> n >> m)) return 0;
    
    vector<pair<int, int>> edges(m + 1);
    vector<int> link(m + 1);
    vector<int> pid(m + 1, 0);
    int p_cnt = 0;
    
    for (int i = 1; i <= m; i++) {
        int u, v, l;
        cin >> u >> v >> l;
        edges[i] = {u, v};
        link[i] = l;
    }
    
    // Group linked edges into single Pair IDs
    for (int i = 1; i <= m; i++) {
        if (pid[i] == 0) {
            p_cnt++;
            pid[i] = p_cnt;
            pid[link[i]] = p_cnt;
        }
    }
    
    for (int i = 1; i <= m; i++) {
        adj[edges[i].first].push_back({edges[i].second, pid[i]});
        adj[edges[i].second].push_back({edges[i].first, pid[i]});
    }
    
    // Hyper-optimized BFS
    auto check = [&]() -> bool {
        timer_vis++;
        int head = 0, tail = 0;
        q[tail++] = 1;
        vis[1] = timer_vis;
        
        bool found2 = false;
        
        while(head < tail) {
            int u = q[head++];
            
            if (u == 2) found2 = true;
            
            // EARLY EXIT: If we reach the rat, this configuration is immediately garbage.
            if (u == 3) return false; 
            
            for (auto& edge : adj[u]) {
                if (disabled[edge.pid]) continue;
                if (vis[edge.to] != timer_vis) {
                    vis[edge.to] = timer_vis;
                    q[tail++] = edge.to;
                }
            }
        }
        return found2;
    };
    
    // 1. Check if we don't even need to close any doors
    if (check()) {
        cout << "Ja\n";
        return 0;
    }
    
    mt19937 rng(1337);
    vector<int> pairs(p_cnt);
    iota(pairs.begin(), pairs.end(), 1);
    shuffle(pairs.begin(), pairs.end(), rng);
    
    int idx = 0;
    
    // 2. Randomly sample removing 1 pair of edges (2 edges)
    while (true) {
        auto current_time = chrono::steady_clock::now();
        double elapsed = chrono::duration<double>(current_time - start_time).count();
        if (elapsed > 0.5) break; 
        
        if (idx >= p_cnt) break; // Exhausted all single pairs
        
        int p = pairs[idx++];
        disabled[p] = true;
        
        if (check()) {
            cout << "Ja\n";
            return 0;
        }
        
        disabled[p] = false;
    }
    
    // 3. If time somehow permits, brute-force random double pairs (4 edges cut)
    while (true) {
        auto current_time = chrono::steady_clock::now();
        double elapsed = chrono::duration<double>(current_time - start_time).count();
        if (elapsed > 0.95) break;
        
        int p1 = pairs[rng() % p_cnt];
        int p2 = pairs[rng() % p_cnt];
        
        disabled[p1] = true;
        disabled[p2] = true;
        
        if (check()) {
            cout << "Ja\n";
            return 0;
        }
        
        disabled[p1] = false;
        disabled[p2] = false;
    }
    
    cout << "Nej\n";
    return 0;
}