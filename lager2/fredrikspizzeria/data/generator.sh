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
compile gen_kill_dfs_branches.py
compile gen_hard.py

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
tc g3-ec-sno gen_edge_cases mode=small_no
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

N=2000
M=2000
group group5 15
limits maxn=$N maxm=$M
include_group group3 group4
tc g5-1 gen_rand n=1950 seed=902
tc g5-2 gen_rand n=1950
for p in 10 15 20 22; do
    for k in 10 15 20 22; do
        tc g5-y-vertex-p$p-k$k gen_y type=edge parts=$p k=$k extra-edges=1
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

group group6 19
limits vertexcactus=1
include_group group2 group4
tc g6-1 gen_rand_vertexcactus n=199950 cycles=1
tc g6-2 gen_rand_vertexcactus n=199951 cycles=2
tc g6-3 gen_rand_vertexcactus n=199950 cycles=3
tc g6-4 gen_rand_vertexcactus n=199951 cycles=10
tc g6-5 gen_rand_vertexcactus n=199851 cycles=100
tc g6-6 gen_rand_vertexcactus n=198851 cycles=1000
tc g6-7 gen_kill_m2 k=19999
tc g6-mt1 gen_multiple_types n=150000 m=199998 structure=hub dep=cross vertex_disjoint=1
tc g6-mt2 gen_multiple_types n=180000 m=199998 structure=deep dep=mixed vertex_disjoint=1
tc g6-y1 gen_y type=vertex parts=100 k=500 extra-edges=0
tc g6-y2 gen_y type=vertex parts=50 k=1000 extra-edges=0

group group7 31
include_group group5 group6
tc g7-1 gen_rand n=199950
tc g7-2 gen_rand n=199950
tc g7-3 gen_kill_m2 k=19999
tc g7-mt1 gen_multiple_types n=150000 m=200000 structure=hub dep=cross
tc g7-mt2 gen_multiple_types n=180000 m=200000 structure=deep dep=mixed
tc g7-kh1 gen_kill_heuristic k=39999
tc g7-ec-bitset gen_edge_cases mode=kill_bitset256 cycles=60000
tc g7-hard-fn gen_hard mode=forced_no len=99998
tc g7-hard-fy gen_hard mode=forced_yes len=50000
tc g7-hard-cc gen_hard mode=conflict_chain c12=30000 c13=20000
tc g7-y1 gen_y type=edge parts=100 k=600 extra-edges=2
tc g7-y2 gen_y type=edge parts=50 k=1000 extra-edges=3

