Let M = tot monsters.
- bfs_slow.cpp: O(BNT)
- bfs_precomp_dumb.cpp: O(BT+BN^2)
- bfs_precomp_smart.cpp: O(BT+BN)
- greedy_slow.cpp: O(BT). Also runs faster if few runs are needed
- greedy_slow_opt.cpp: O(BT). greedy_slow, but does all prev in batch
- greedy_slow_opt2.cpp: O(T^2log(T)). dies from single large dungeon where we visit every breakpoint
- joshua_ugly.cpp: O(Tlog(T))
- joshua.cpp: O(Tlog(T))
