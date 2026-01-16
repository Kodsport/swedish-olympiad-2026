#include "validator.h"
#define rep(i, lo, hi) for (int i = (lo); i < (hi); i++)

const int MAX_N = 2e5;
const int MAX_P = 15;
const int MAX_H = 2e5;
const int MAX_F = 2e5;

void run() {
    int n = Int(1, Arg("maxn", MAX_N));
    Space();
    int p = Int(1, Arg("maxp", MAX_P));
    Space();
    int h = Int(0, Arg("maxh", MAX_H));
    Space();
    int f = Int(1, Arg("maxf", MAX_F));
    Endl();

    vector<vector<int>> solved(n, vector<int>(p));
    vector<vector<int>> num_submissions(n, vector<int>(p));
    bool once_per_problem = Arg("once_per_problem", false);

    auto add_sub = [&](int team, int problem, string verdict) {
        assert(!solved[team][problem]);
        num_submissions[team][problem]++;
        if (once_per_problem) assert(num_submissions[team][problem] == 1);
        if (verdict=="AC") solved[team][problem]=1;
    };

    rep(i,0,h) {
        int team = Int(1, n);
        Space();
        int problem = Int(1, p);
        Space();
        string verdict = _token();
        Endl();
        assert(verdict == "AC" || verdict == "WA");
        team--; problem--;
        add_sub(team,problem,verdict);
    }

    rep(i,0,f) {
        int team = Int(1, n);
        Space();
        int problem = Int(1, p);
        Space();
        string verdict = _token();
        Endl();
        assert(verdict == "AC" || verdict == "WA");
        team--; problem--;
        add_sub(team,problem,verdict);
    }
}
