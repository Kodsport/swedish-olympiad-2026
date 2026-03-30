#!/usr/bin/env bash

PPATH=$(realpath ..)
. ../../../testdata_tools/gen.sh

ulimit -s unlimited

use_solution codex.cpp

compile gen_tree.py
compile gen_rand.py
compile gen_rand_vertexcactus.py
compile gen_rand_edgecactus.py
compile gen_kill_m2.py
compile gen_y.py
compile gen_multiple_types.py
compile gen_stack_overflow.py
compile gen_edge_cases.py
compile gen_kill_heuristic.py
compile gen_kill_heuristic2.py
compile gen_kill_heuristic_fixed.py
compile gen_kill_dfs_branches.py
compile gen_hard.py
compile gen_kill_same_branch.py
compile gen_big_cycles.py
compile gen_kill_cheese.py
compile gen_kill_cheese2.py
compile gen_kill_cheese3.py
compile gen_kill_cheese4.py

samplegroup
sample 1
sample 2

MAXN=200000
MAXM=200000

N=99
SQRTN=8
group group1 5
limits maxn=100 maxm=100 tree=1
tc g1-1 gen_tree n=$N mode=random
tc g1-2 gen_tree n=$N mode=random
tc g1-3 gen_tree n=$N mode=prufer
tc g1-4 gen_tree n=$N mode=prufer
tc g1-5 gen_tree n=$N mode=star extra-nodes=$SQRTN
tc g1-6 gen_tree n=$N mode=path extra-nodes=$SQRTN
tc g1-7 gen_tree n=$N mode=broomstick extra-nodes=$SQRTN
tc g1-9 gen_tree n=$N mode=dumbbell extra-nodes=$SQRTN
tc g1-10 gen_tree n=$N mode=dumbbell extra-nodes=$SQRTN
tc g1-mt1 gen_multiple_types special=line n=99 dep=cross
tc g1-gg-path gen_multiple_types n=9 m=8 structure=path dep=path12
tc g1-gg-branch gen_multiple_types n=17 m=16 structure=random dep=random
tc g1-gg-chain gen_multiple_types n=37 m=36 structure=path dep=random
tc g1-gg-wide gen_multiple_types n=61 m=60 structure=hub dep=random
tc g1-gg-deep gen_multiple_types n=99 m=98 structure=deep dep=random
tc g1-gg-rand gen_multiple_types n=73 m=72 structure=random dep=mixed

N=$((MAXN-1))
SQRTN=300
group group2 10
limits tree=1
include_group group1
tc g2-1 gen_tree n=$N mode=random
tc g2-2 gen_tree n=$N mode=random
tc g2-3 gen_tree n=$N mode=prufer
tc g2-4 gen_tree n=$N mode=prufer
tc g2-5 gen_tree n=$N mode=star extra-nodes=$SQRTN
tc g2-6 gen_tree n=$N mode=path extra-nodes=$SQRTN
tc g2-7 gen_tree n=$N mode=broomstick extra-nodes=$SQRTN
tc g2-9 gen_tree n=$N mode=dumbbell extra-nodes=$SQRTN
tc g2-10 gen_tree n=$N mode=dumbbell extra-nodes=$SQRTN
tc g2-mt1 gen_multiple_types special=line n=199999 dep=cross
tc g2-so1 gen_stack_overflow n=199999
tc g2-gg-path gen_multiple_types n=199999 m=199998 structure=path dep=random
tc g2-gg-pair gen_multiple_types special=line n=199999 dep=random
tc g2-gg-deep gen_multiple_types n=199999 m=199998 structure=deep dep=random
tc g2-gg-rand gen_multiple_types n=199999 m=199998 structure=random dep=random
tc g2-gg-hub gen_multiple_types n=199999 m=199998 structure=hub dep=path12
tc g2-gg-mid1 gen_multiple_types n=123457 m=123456 structure=random dep=mixed
tc g2-gg-mid2 gen_multiple_types n=150001 m=150000 structure=deep dep=path13

N=100
M=100
group group3 12
limits maxn=$N maxm=$M
include_group sample group1
tc g3-1 gen_rand n=90 seed=827
tc g3-2 gen_rand n=90 seed=500
tc g3-3 gen_rand n=90
tc g3-4 gen_rand_vertexcactus n=90 cycles=1
tc g3-5 gen_rand_vertexcactus n=91 cycles=2
tc g3-6 gen_rand_vertexcactus n=90 cycles=3
tc g3-7 gen_rand_vertexcactus n=81 cycles=10
tc g3-8 gen_rand_vertexcactus n=47 cycles=40
tc g3-9 gen_rand_edgecactus  n=90 cycles=1
tc g3-10 gen_rand_edgecactus n=91 cycles=2
tc g3-11 gen_rand_edgecactus n=90 cycles=3
tc g3-12 gen_rand_edgecactus n=81 cycles=10
tc g3-13 gen_rand_edgecactus n=47 cycles=40
tc_manual ../manual_testcases/bug1_killer.in
tc g3-y1-edge gen_y type=edge parts=1 k=3 extra-edges=5
tc g3-y2-edge gen_y type=edge parts=2 k=3 extra-edges=5
tc g3-y3-edge gen_y type=edge parts=3 k=5 extra-edges=5
tc g3-y4-edge gen_y type=edge parts=5 k=4 extra-edges=5
for i in {1..10}; do
    tc g3-y-edge$i gen_y type=edge parts=2 k=5 extra-edges=1
done
tc g3-y1-vertex gen_y type=vertex parts=1 k=3 extra-edges=5
tc g3-y2-vertex gen_y type=vertex parts=2 k=3 extra-edges=5
tc g3-y3-vertex gen_y type=vertex parts=3 k=5 extra-edges=5
tc g3-y4-vertex gen_y type=vertex parts=4 k=4 extra-edges=5
for i in {1..10}; do
    tc g3-y-vertex$i gen_y type=vertex parts=2 k=5 extra-edges=1
done
tc g3-ec-syes gen_edge_cases mode=small_yes
tc g3-ec-chain gen_edge_cases mode=chain_cycles
tc g3-ec-nice gen_edge_cases mode=kill_nicecactus
tc g3-ec-ccc gen_edge_cases mode=complex_cycle_chain cycles=5
tc g3-ec-scp gen_edge_cases mode=same_cycle_pair
tc g3-ec-cpp gen_edge_cases mode=cross_path_pair
tc g3-kdb gen_kill_dfs_branches
tc g3-mt1 gen_multiple_types n=90 m=100 structure=hub dep=cross
tc g3-mt2 gen_multiple_types n=80 m=98 structure=deep dep=mixed
tc g3-hard-fn gen_hard mode=forced_no len=10
tc g3-hard-fy gen_hard mode=forced_yes len=10
tc g3-gg-tree gen_multiple_types n=91 m=90 structure=random dep=random
tc g3-gg-tri gen_multiple_types n=68 m=100 structure=hub cycle_style=small dep=random
tc g3-gg-mixed gen_multiple_types n=72 m=100 structure=random cycle_style=mixed dep=path12
tc g3-gg-lgc gen_multiple_types n=80 m=100 structure=deep cycle_style=large dep=path13
tc g3-gg-srand gen_multiple_types n=40 m=55 structure=random cycle_style=mixed dep=random
tc g3-gg-shub gen_multiple_types n=75 m=100 structure=hub cycle_style=small dep=mixed
tc g3-bc-1cy gen_big_cycles mode=one_cycle_yes n=40
tc g3-bc-1cn gen_big_cycles mode=one_cycle_no n=40
tc g3-kc gen_kill_cheese n=98

N=2000
M=2000
group group4 8
limits maxn=$N maxm=$M vertexcactus=1
include_group group1
tc 1
tc g4-1 gen_rand_vertexcactus n=1950 cycles=1
tc g4-2 gen_rand_vertexcactus n=1951 cycles=2
tc g4-3 gen_rand_vertexcactus n=1950 cycles=3
tc g4-4 gen_rand_vertexcactus n=1951 cycles=10
tc g4-5 gen_rand_vertexcactus n=1851 cycles=100
for p in 10 15 20 22; do
    for k in 10 15 20 22; do
        tc g4-y-vertex-p$p-k$k gen_y type=vertex parts=$p k=$k extra-edges=1
    done
done
for i in {1..10}; do
    tc g4-y-vertex-small$i gen_y type=vertex parts=2 k=5 extra-edges=1
done
tc g4-mt1 gen_multiple_types n=1800 m=2000 structure=hub dep=cross vertex_disjoint=1
tc g4-mt2 gen_multiple_types n=1500 m=1998 structure=deep dep=mixed vertex_disjoint=1
tc g4-kc gen_kill_cheese n=2000
tc g4-killcheese2 gen_kill_cheese2 n=499
tc g4-killcheese3 gen_kill_cheese3
tc g4-killcheese4 gen_kill_cheese4
tc g4-kh3 gen_kill_heuristic_fixed p=498 mode=vertex


N=2000
M=2000
group group5 15
limits maxn=$N maxm=$M
include_group group3 group4
tc g5-1 gen_rand n=1950 seed=902
tc g5-2 gen_rand n=1950
for p in 10 15 20 22; do
    for k in 10 15 20 22; do
        tc g5-y-edge-p$p-k$k gen_y type=edge parts=$p k=$k extra-edges=1
    done
done
tc g5-mt1 gen_multiple_types n=1500 m=2000 structure=hub dep=cross
tc g5-mt2 gen_multiple_types n=1800 m=2000 structure=deep dep=mixed
tc g5-ec-bitset gen_edge_cases mode=kill_bitset256 cycles=300
tc g5-ec-vcs gen_edge_cases mode=vertex_cactus_stress n=1200
tc g5-hard-cc gen_hard mode=conflict_chain c12=200 c13=150
tc g5-hard-fn gen_hard mode=forced_no len=500
tc g5-hard-fy gen_hard mode=forced_yes len=500
tc g5-hard-lvc gen_hard mode=large_vertex_cactus n=1800 cycles=100
tc g5-gg-tree gen_multiple_types n=2000 m=1999 structure=path dep=random
tc g5-gg-bal gen_multiple_types n=1335 m=2000 structure=random cycle_style=mixed dep=random
tc g5-gg-hub gen_multiple_types n=1335 m=2000 structure=hub cycle_style=small dep=path12
tc g5-gg-deep gen_multiple_types n=1335 m=2000 structure=deep cycle_style=large dep=path13
tc g5-gg-mid gen_multiple_types n=1500 m=1900 structure=random cycle_style=mixed dep=random
tc g5-gg-sparse gen_multiple_types n=1800 m=1900 structure=deep cycle_style=small dep=mixed
tc g5-gg-dense gen_multiple_types n=1335 m=2000 structure=hub cycle_style=small dep=random
tc g5-bc-1cy gen_big_cycles mode=one_cycle_yes n=900
tc g5-bc-1cn gen_big_cycles mode=one_cycle_no n=900
tc g5-bc-3cy gen_big_cycles mode=three_cycles_yes n=600
tc g5-bc-3cnx gen_big_cycles mode=three_cycles_no_cross n=600
tc g5-bc-3cnd gen_big_cycles mode=three_cycles_no_direct n=600
tc g5-bc-3cns gen_big_cycles mode=three_cycles_no_shared n=600
tc g5-kc gen_kill_cheese n=2000
tc g4-killcheese2
tc g4-killcheese3
tc g4-killcheese4
tc g4-kh3

group group6 19
limits vertexcactus=1
include_group group2 group4
tc g6-1 gen_rand_vertexcactus n=199950 cycles=1
tc g6-2 gen_rand_vertexcactus n=199951 cycles=2
tc g6-3 gen_rand_vertexcactus n=199950 cycles=3
tc g6-4 gen_rand_vertexcactus n=199951 cycles=10
tc g6-5 gen_rand_vertexcactus n=199851 cycles=100
tc g6-6 gen_rand_vertexcactus n=198851 cycles=1000
tc g6-kill-m2 gen_kill_m2 k=19999
tc g6-mt1 gen_multiple_types n=150000 m=199998 structure=hub dep=cross vertex_disjoint=1
tc g6-mt2 gen_multiple_types n=180000 m=199998 structure=deep dep=mixed vertex_disjoint=1
tc g6-y1 gen_y type=vertex parts=100 k=500 extra-edges=0
tc g6-y2 gen_y type=vertex parts=50 k=1000 extra-edges=0
tc g6-gg-rand gen_multiple_types n=150001 m=200000 structure=random cycle_style=mixed vertex_disjoint=1 dep=random
tc g6-gg-deep gen_multiple_types n=160000 m=200000 structure=deep cycle_style=large vertex_disjoint=1 dep=random
tc g6-gg-path gen_multiple_types n=160000 m=200000 structure=path cycle_style=small vertex_disjoint=1 dep=random
tc g6-gg-near gen_multiple_types n=199500 m=200000 structure=random cycle_style=small vertex_disjoint=1 dep=path12
tc g6-gg-mid gen_multiple_types n=120001 m=160000 structure=deep cycle_style=mixed vertex_disjoint=1 dep=path13
tc g6-gg-hub gen_multiple_types n=150001 m=200000 structure=hub cycle_style=small vertex_disjoint=1 dep=mixed
tc g6-kc gen_kill_cheese n=200000

group group7 31
include_group group5 group6
tc g7-1 gen_rand n=199950
tc g7-2 gen_rand n=199950
tc g7-mt1 gen_multiple_types n=150000 m=200000 structure=hub dep=cross
tc g7-mt2 gen_multiple_types n=180000 m=200000 structure=deep dep=mixed
tc g7-kh1 gen_kill_heuristic k=39999
tc g7-ksb gen_kill_same_branch
tc g7-ec-bitset gen_edge_cases mode=kill_bitset256 cycles=60000
tc g7-hard-fn gen_hard mode=forced_no len=99998
tc g7-hard-fy gen_hard mode=forced_yes len=50000
tc g7-hard-cc gen_hard mode=conflict_chain c12=30000 c13=20000
tc g7-y1 gen_y type=edge parts=100 k=600 extra-edges=2
tc g7-y2 gen_y type=edge parts=50 k=1000 extra-edges=3
tc g7-gg-ops1 gen_multiple_types special=ops_cap common_left=25000 common_right=25000
tc g7-gg-ops2 gen_multiple_types special=ops_cap common_left=40000 common_right=40000
tc g7-gg-rand gen_multiple_types n=150000 m=200000 structure=random cycle_style=mixed dep=random
tc g7-gg-hub gen_multiple_types n=133335 m=200000 structure=hub cycle_style=small dep=random
tc g7-gg-deep gen_multiple_types n=140000 m=200000 structure=deep cycle_style=large dep=path13
tc g7-gg-path gen_multiple_types n=170000 m=200000 structure=path cycle_style=small dep=random
tc g7-gg-mid gen_multiple_types n=150000 m=200000 structure=random cycle_style=large dep=path12
tc g7-gg-shared gen_multiple_types n=133335 m=200000 structure=hub cycle_style=small dep=mixed
tc g7-gg-deepvd gen_multiple_types seed=26 n=150001 m=200000 structure=deep cycle_style=small vertex_disjoint=1 dep=random
tc g7-bc-1cy gen_big_cycles mode=one_cycle_yes n=99000
tc g7-bc-1cn gen_big_cycles mode=one_cycle_no n=99000
tc g7-bc-3cy gen_big_cycles mode=three_cycles_yes n=60000
tc g7-bc-3cnx gen_big_cycles mode=three_cycles_no_cross n=60000
tc g7-bc-3cnd gen_big_cycles mode=three_cycles_no_direct n=60000
tc g7-bc-3cns gen_big_cycles mode=three_cycles_no_shared n=60000
tc g7-kc gen_kill_cheese n=200000

