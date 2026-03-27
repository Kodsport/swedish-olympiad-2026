#include "validator.h"

void run() {
    int n = Int(1, Arg("maxn", 500000));
    Space();
    int a = Int(1, 1e9);
    Space();
    int b = Int(Arg("minb", 1), Arg("maxb", 1000000000));
    Endl();

    string row = Line();
    int left = 0, right = 0;
    for (int i = 0; i < n; i++) {
        char c = row[i];
        assert(c=='(' || c==')');
        if (c == '(') left++;
        else right++;
    }

    if (Arg("equal", 0)) {
        assert(left == right);
    }
}
