#include "validator.h"
#define rep(i, lo, hi) for (int i = (lo); i < (hi); i++)


void run() {
    int n = Int(1, Arg("maxn"));
    Space();
    int p = Int(1, Arg("maxp"));
    Space();
    int h = Int(0, Arg("maxh"));
    Space();
    int f = Int(1, Arg("maxf"));
    Endl();

    vector<vector<int>> solved(n, vector<int>(p));

    auto add_sub = [&](int team, int problem, char verdict) {
        assert(!solved[team][problem]);
        if (verdict=='A') solved[team][problem]=1;
    };

    rep(i,0,h) {
        int team = Int(1, n);
        Space();
        int problem = Int(1, p);
        Space();
        char verdict = Char();
        Endl();
        team--; problem--;
        add_sub(team,problem,verdict);
    }

    rep(i,0,f) {
        int team = Int(1, n);
        Space();
        int problem = Int(1, p);
        Space();
        char verdict = Char();
        Endl();
        team--; problem--;
        add_sub(team,problem,verdict);
    }
}
