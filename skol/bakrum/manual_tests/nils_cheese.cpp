#include <bits/stdc++.h>
using namespace std;

int main(int argc, char** argv) {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int dx[4] = {0, 0, 1, -1};
    int dy[4] = {1, -1, 0, 0};

    int n, m;
    cin >> n >> m;

    int q = 5;

    // Read D from command-line argument
    if (argc < 2) {
        cerr << "Missing D argument\n";
        return 1;
    }
    int D;
    try {
        D = stoi(argv[1]);
    } catch (...) {
        cerr << "Invalid D argument\n";
        return 1;
    }

    vector<string> grid;
    grid.reserve(n);

    // Insert each row at the **front**, as Python insert(0, ...) does
    for (int r = 0; r < n; r++) {
        string s;
        cin >> s;
        grid.insert(grid.begin(), s);
    }

    int N = n * D;
    int M = m * D;

    vector<vector<int>> comp(N, vector<int>(M, -1));
    int c = 0;

    // Compute connected components
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < M; j++) {
            if (comp[i][j] == -1 && grid[i % n][j % m] != '#') {

                queue<pair<int,int>> Q;
                Q.emplace(i, j);

                while (!Q.empty()) {
                    auto [i2, j2] = Q.front();
                    Q.pop();

                    if (comp[i2][j2] != -1) continue;
                    comp[i2][j2] = c;

                    for (int k = 0; k < 4; k++) {
                        int i3 = (i2 + dx[k] + N) % N;
                        int j3 = (j2 + dy[k] + M) % M;

                        if (comp[i3][j3] == -1 &&
                            grid[i3 % n][j3 % m] != '#') {
                            Q.emplace(i3, j3);
                        }
                    }
                }

                c++;
            }
        }
    }

    // Process 5 queries
    for (int _ = 0; _ < q; _++) {
        long long j1, i1, j2, i2;
        cin >> j1 >> i1 >> j2 >> i2;

        i1 = i1 % N;
        j1 = j1 % M;
        i2 = i2 % N;
        j2 = j2 % M;

        //assert(comp[i1][j1] != -1 && comp[i2][j2] != -1);

        if (comp[i1][j1] == comp[i2][j2])
            cout << "Ja\n";
        else
            cout << "Nej\n";
    }

    return 0;
}
