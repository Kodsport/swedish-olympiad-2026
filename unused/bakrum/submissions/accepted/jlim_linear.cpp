#include <bits/stdc++.h>
using namespace std;

template <typename T>
array<T, 3> extended_gcd(const T &a, const T &b) {
    if (b == (T) 0) return {a, (T) 1, (T) 0};

    auto divmod = [&](const T &x, const T &y) -> pair<T, T> {
        if constexpr (requires(T z) { z.real(); z.imag(); }) {
            auto div = [&](const T &x, const T &y) -> T {
                auto numer = x * conj(y);
                auto denom = norm(y);
                auto round_div = [&](auto part) {
                    return (part >= 0) ? (part + denom / 2) / denom : (part - denom / 2) / denom;
                };
                return (T) {round_div(numer.real()), round_div(numer.imag())};
            };
            auto q = div(x, y);
            auto r = x - q * y;
            return {q, r};
        } else {
            auto q = x / y;
            auto r = x - q * y;
            return {q, r};
        }
    };

    auto [q, r] = divmod(a, b);
    auto [g, s, t] = extended_gcd(b, r);
    return {g, t, s - t * q};
}
int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int R, C;
    cin >> R >> C;

    vector<string> grid(R);
    for (auto &row : grid) cin >> row;

    vector<string> rev(R, string(C, '#'));
    for (int i = 0; i < R; i++) rev[R - 1 - i] = grid[i];

    int n = 0;
    vector<vector<int>> id(R, vector<int>(C, -1));
    for (int r = 0; r < R; r++)
        for (int c = 0; c < C; c++)
            if (rev[r][c] == '.') id[r][c] = n++;

    vector<vector<array<int, 3>>> adj_list(n);
    vector<int> dr{0, 0, 1, -1}, dc{1, -1, 0, 0};
    for (int r = 0; r < R; r++)
        for (int c = 0; c < C; c++) {
            if (id[r][c] == -1) continue;
            int v = id[r][c];
            for (int dir = 0; dir < 4; dir++) {
                int nr = r + dr[dir], nc = c + dc[dir], a = 0, b = 0;
                if (nc < 0) {
                    nc += C;
                    a--;
                } else if (nc >= C) {
                    nc -= C;
                    a++;
                }
                if (nr < 0) {
                    nr += R;
                    b--;
                } else if (nr >= R) {
                    nr -= R;
                    b++;
                }
                if (id[nr][nc] == -1) continue;
                adj_list[v].push_back({id[nr][nc], a, b});
            }
        }

    vector<int> component(n, -1);
    vector<complex<long long>> base(n);
    vector<vector<complex<long long>>> cycles;
    int components = 0;
    for (int s = 0; s < n; s++) {
        if (component[s] != -1) continue;
        component[s] = components;
        base[s] = complex<long long>(0, 0);
        cycles.emplace_back();

        queue<int> q;
        q.emplace(s);
        while (!q.empty()) {
            int v = q.front();
            q.pop();

            for (auto [u, a, b] : adj_list[v]) {
                complex<long long> step(a, b);
                if (component[u] == -1) {
                    component[u] = components;
                    base[u] = base[v] + step;
                    q.emplace(u);
                } else {
                    auto cycle = base[v] + step - base[u];
                    if (cycle != complex<long long>(0, 0)) cycles[components].emplace_back(cycle);
                }
            }
        }
        components++;
    }

    auto cross = [&](auto &a, auto &b) {
        return (conj(a) * b).imag();
    };

    auto normalize = [&](auto &z) {
        if (z.real() < 0 || (!z.real() && z.imag() < 0)) z = -z;
        return z;
    };

    struct State {
        int type;
        complex<long long> z, p, q;
        long long det;
    };

    vector<State> states(components);
    for (int c = 0; c < components; c++) {
        State s;
        s.type = 0;
        s.z = s.p = s.q = complex<long long>(0, 0);
        s.det = 0;
        if (!cycles[c].empty()) {
            auto start = cycles[c][0];
            bool indep = false;
            for (int i = 1; i < cycles[c].size(); i++)
                if (cross(start, cycles[c][i]) != 0) {
                    indep = true;
                    break;
                }

            if (!indep) {
                s.type = 1;
                auto z = cycles[c][0];
                for (int i = 1; i < cycles[c].size(); i++) z = extended_gcd(z, cycles[c][i])[0];
                s.z = normalize(z);
            } else {
                s.type = 2;

                complex<long long> p(0, 0), q(0, 0);
                auto det = 0LL;
                for (auto v : cycles[c]) {
                    if (v == complex<long long>(0, 0)) continue;
                    if (p == complex<long long>(0, 0)) {
                        p = v;
                        continue;
                    }

                    if (q == complex<long long>(0, 0)) {
                        if (cross(p, v) == 0) p = normalize(extended_gcd(p, v)[0]);
                        else {
                            q = v;
                            det = cross(p, q);
                            if (det < 0) {
                                det = -det;
                                q = -q;
                            }
                        }
                        continue;
                    }

                    det = cross(p, q);
                    if (det < 0) {
                        det = -det;
                        q = -q;
                    }

                    auto [g1, x1, y1] = extended_gcd(det, cross(p, v));
                    q = q * x1 + v * y1;
                    det = g1;
                    if (cross(p, q) < 0) q = -q;

                    auto det1 = cross(p, q);
                    if (det1 < 0) det1 = -det1;
                    if (det1 != det)
                        if (det1) q /= det1 / det;

                    auto [g2, x2, y2] = extended_gcd(det, cross(v, q));
                    p = p * x2 + v * y2;
                    det = g2;
                    if (cross(p, q) < 0) q = -q;

                    auto det2 = cross(p, q);
                    if (det2 < 0) det2 = -det2;
                    if (det2 != det)
                        if (det2) q /= det2 / det;
                }

                det = cross(p, q);
                if (det < 0) {
                    det = -det;
                    q = -q;
                }

                s.p = p;
                s.q = q;
                s.det = det;
            }
        }
        states[c] = s;
    }

    for (int _ = 0; _ < 5; _++) {
        long long sx, sy, gx, gy;
        cin >> sx >> sy >> gx >> gy;

        auto convert = [&](long long x, long long y) {
            return make_pair(id[y % R][x % C], complex<long long>(x / C, y / R));
        };

        auto [c_s, cell_s] = convert(sx, sy);
        auto [c_g, cell_g] = convert(gx, gy);
        if (component[c_s] != component[c_g]) {
            cout << "Nej\n";
            continue;
        }

        auto &s = states[component[c_s]];
        auto need = cell_g - cell_s - base[c_g] + base[c_s];
        bool reach;
        if (!s.type) reach = (need == complex<long long>(0, 0));
        else if (s.type == 1) {
            auto h = s.z;
            if (h == complex<long long>(0, 0)) reach = (need == complex<long long>(0, 0));
            else {
                if (cross(need, h) != 0) reach = false;
                else {
                    if (h.real()) {
                        if (need.real() % h.real()) reach = false;
                        else reach = (need.imag() == h.imag() * need.real() / h.real());
                    } else {
                        if (need.imag() % h.imag() != 0) reach = false;
                        else reach = (need.real() == h.real() * need.imag() / h.imag());
                    }
                }
            }
        } else {
            auto det = s.det;
            if (!det) reach = (need == complex<long long>(0, 0));
            else reach = !((cross(s.p, need) + det) % det) && !((cross(need, s.q) + det) % det);
        }

        if (reach) cout << "Ja\n";
        else cout << "Nej\n";
    }
}