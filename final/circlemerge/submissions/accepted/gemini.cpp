#include <iostream>
#include <vector>
#include <numeric>
#include <algorithm>

using namespace std;

int main() {
    // Fast I/O
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    int N;
    if (!(cin >> N)) return 0;

    vector<long long> A(N);
    long long M = 0;
    long long max_a = 0;
    for (int i = 0; i < N; ++i) {
        cin >> A[i];
        M += A[i];
        if (A[i] > max_a) {
            max_a = A[i];
        }
    }

    vector<long long> P(2 * N + 1, 0);
    for (int i = 0; i < 2 * N; ++i) {
        P[i + 1] = P[i] + A[i % N];
    }

    vector<long long> divisors;
    for (long long i = 1; i * i <= M; ++i) {
        if (M % i == 0) {
            if (i >= max_a) divisors.push_back(i);
            if (i * i != M && (M / i) >= max_a) {
                divisors.push_back(M / i);
            }
        }
    }
    sort(divisors.begin(), divisors.end());

    vector<int> next_idx(N, -1);
    vector<int> visited(N, 0);

    for (long long S : divisors) {
        int j = 1;
        for (int i = 0; i < N; ++i) {
            if (j <= i) j = i + 1;
            while (j < 2 * N && P[j] - P[i] < S) {
                j++;
            }
            if (j < 2 * N && P[j] - P[i] == S) {
                next_idx[i] = (j < N) ? j : (j - N);
            } else {
                next_idx[i] = -1;
            }
        }

        bool possible = false;
        fill(visited.begin(), visited.end(), 0);
        for (int i = 0; i < N; ++i) {
            if (visited[i] == 0) {
                int curr = i;
                while (curr != -1 && visited[curr] == 0) {
                    visited[curr] = i + 1;
                    curr = next_idx[curr];
                }
                if (curr != -1 && visited[curr] == i + 1) {
                    possible = true;
                    break;
                }
            }
        }


        if (possible) {
            cout << N - (M / S) << "\n";
            return 0;
        }
    }

    return 0;
}
