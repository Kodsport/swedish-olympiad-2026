#include "validator.h"

void run() {
    int n = Int(1, Arg("maxn", int(2e5)));
    Space();
    int b = Int(1, Arg("maxb", int(1e9)));
    Endl();

    int tot_monsters = 0;
    int maxt = Arg("maxt", int(3e5));
    int max_dungeon = Arg("max_dungeon", maxt); // max monsters per dungeon

    for (int i = 0; i < n; i++) {
        int m = Int(1, int(3e5));
        assert(m <= max_dungeon);
        Endl();
        tot_monsters += m;
        for (int j = 0; j < m; j++) {
            Int(1, int(1e9));
            Space();
            Int(1, int(1e9));
            Endl();
        }
    }
    assert(tot_monsters <= maxt);
}
