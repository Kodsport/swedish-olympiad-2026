#include <iostream>
#include <vector>
#include <numeric>
#include <algorithm>
#include <cstdlib>

using namespace std;

using u64 = unsigned long long;
using u128 = __uint128_t;

u64 power(u64 base, u64 exp, u64 mod) {
    u64 res = 1;
    base %= mod;
    while (exp > 0) {
        if (exp % 2 == 1) res = (u128)res * base % mod;
        base = (u128)base * base % mod;
        exp /= 2;
    }
    return res;
}

bool miller_rabin(u64 n) {
    if (n < 2) return false;
    if (n == 2 || n == 3) return true;
    if (n % 2 == 0) return false;

    u64 d = n - 1;
    int s = 0;
    while (d % 2 == 0) {
        d /= 2;
        s++;
    }

    static const u64 bases[] = {2, 3, 5, 7, 11, 13, 17, 19, 23, 29, 31, 37};
    for (u64 a : bases) {
        if (n <= a) break;
        u64 x = power(a, d, n);
        if (x == 1 || x == n - 1) continue;
        bool composite = true;
        for (int r = 1; r < s; r++) {
            x = (u128)x * x % n;
            if (x == n - 1) {
                composite = false;
                break;
            }
        }
        if (composite) return false;
    }
    return true;
}

u64 pollard_rho(u64 n) {
    if (n % 2 == 0) return 2;
    if (miller_rabin(n)) return n;
    u64 x = 2, y = 2, d = 1, c = 1;
    auto f = [&](u64 x, u64 n, u64 c) {
        return (u64)(((u128)x * x + c) % n);
    };
    while (d == 1) {
        x = f(x, n, c);
        y = f(f(y, n, c), n, c);
        d = std::gcd(x > y ? x - y : y - x, n);
        if (d == n) {
            x = rand() % (n - 2) + 2;
            y = x;
            c = rand() % (n - 1) + 1;
            d = 1;
        }
    }
    return d;
}

void factorize(u64 n, vector<u64>& factors) {
    if (n == 1) return;
    if (miller_rabin(n)) {
        factors.push_back(n);
        return;
    }
    u64 divisor = pollard_rho(n);
    factorize(divisor, factors);
    factorize(n / divisor, factors);
}

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

    vector<u64> prime_factors;
    factorize(M, prime_factors);
    sort(prime_factors.begin(), prime_factors.end());
    
    vector<pair<u64, int>> factor_counts;
    for (u64 p : prime_factors) {
        if (factor_counts.empty() || factor_counts.back().first != p) {
            factor_counts.push_back({p, 1});
        } else {
            factor_counts.back().second++;
        }
    }

    vector<long long> divisors;
    auto build_divisors = [&](auto& self, int idx, u64 current_div) -> void {
        if (idx == (int)factor_counts.size()) {
            if (current_div >= (u64)max_a) divisors.push_back((long long)current_div);
            return;
        }
        u64 pw = 1;
        for (int i = 0; i <= factor_counts[idx].second; ++i) {
            self(self, idx + 1, current_div * pw);
            if (i < factor_counts[idx].second) pw *= factor_counts[idx].first;
        }
    };
    build_divisors(build_divisors, 0, 1);
    sort(divisors.begin(), divisors.end());

    for (long long S : divisors) {
        int K = M / S;
        if (K == 1) {
            cout << N - 1 << "\n";
            return 0;
        }

        bool possible = false;
        for (int start = 1; start <= N; ++start) {
            if (P[start] > P[0] + S) break;

            int curr = start;
            bool valid = true;
            for (int step = 1; step <= K; ++step) {
                long long target = P[start] + step * S;
                int L = curr + 1, R = curr + N;
                if (R > 2 * N) R = 2 * N;
                
                int next_node = -1;
                while (L <= R) {
                    int mid = L + (R - L) / 2;
                    if (P[mid] == target) {
                        next_node = mid;
                        break;
                    }
                    if (P[mid] < target) L = mid + 1;
                    else R = mid - 1;
                }
                
                if (next_node != -1) {
                    curr = next_node;
                } else {
                    valid = false;
                    break;
                }
            }
            if (valid) {
                possible = true;
                break;
            }
        }

        if (possible) {
            cout << N - K << "\n";
            return 0;
        }
    }

    return 0;
}
