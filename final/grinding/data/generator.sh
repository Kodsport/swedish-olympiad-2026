#!/usr/bin/env bash

PPATH=$(realpath ..)

. ../../../testdata_tools/gen.sh

ulimit -s unlimited

use_solution joshua.cpp opt

compile gen_rand.py
compile gen_many.py
compile gen_one.py
compile gen_tailheavy.py
compile gen_expensive.py

samplegroup
limits maxn=10 maxb=1000 maxt=10
sample 1
sample 2
sample 3

MAXN=200000
MAXB=1000_000_000
MAXT=300000

N=100
B=100
T=100
group group1 10
limits maxn=$N maxb=$B maxt=$T
for n in 1 5 10; do
    for t in 20 50 100; do
        for b in 10 50 100; do
            tc g1-rand$i-n${n}-t${t}-b${b} gen_rand n=$n b=$b t=$t monster_distribution=random stat_mode=random
        done
    done
done

for distribution in random even frontloaded backloaded; do
    for stat_mode in random strong weak ratio; do
        tc g1-dist-${distribution}-stat-${stat_mode} gen_rand n=$N b=$B t=$T monster_distribution=$distribution stat_mode=$stat_mode
    done
done
tc g1-spiky-1 gen_rand n=$N b=$B t=$T monster_distribution=spiky spikes=1 stat_mode=random
tc g1-spiky-2 gen_rand n=$N b=$B t=$T monster_distribution=spiky spikes=2 stat_mode=random
tc g1-spiky-3 gen_rand n=$N b=$B t=$T monster_distribution=spiky spikes=3 stat_mode=random
tc g1-many1 gen_many n=10 b=$B num_weird=0
tc g1-many2 gen_many n=50 b=$B num_weird=0
tc g1-many3 gen_many n=$N b=$B num_weird=0
tc g1-many4 gen_many n=$N b=$B num_weird=5
tc g1-many5 gen_many n=$N b=$B num_weird=10
tc g1-many6 gen_many n=$N b=$B num_weird=50
tc g1-one1 gen_one b=$B t=$T mul=1 add=0
tc g1-one2 gen_one b=$B t=$T mul=2 add=5
tc g1-tailheavy1 gen_tailheavy n=$N b=$B t=$T
tc g1-expensive-good-1 gen_expensive n=10 b=97
tc g1-expensive-good-2 gen_expensive n=4 b=97
tc g1-expensive-good-3 gen_expensive n=5 b=96
tc g1-expensive-good-4 gen_expensive n=20 b=99
tc g1-expensive-good-5 gen_expensive n=40 b=50
tc_manual ../manual_testcases/bug
tc_manual ../manual_testcases/bug2
tc_manual ../manual_testcases/bug3

N=2000
B=5000
T=5000
group group2 18
limits maxn=$N maxb=$B maxt=$T
include_group sample
include_group group1
tc g2-1 gen_rand n=10 b=$B t=$T monster_distribution=spiky spikes=1 stat_mode=ratio
tc g2-2 gen_rand n=10 b=$B t=$T monster_distribution=spiky spikes=1 stat_mode=strong
tc g2-3 gen_rand n=10 b=$B t=$T monster_distribution=spiky spikes=2 stat_mode=ratio
tc g2-4 gen_rand n=10 b=$B t=$T monster_distribution=spiky spikes=2 stat_mode=strong
tc g2-5 gen_rand n=$N b=$B t=$T monster_distribution=backloaded stat_mode=weak
tc g2-6 gen_rand n=$N b=$B t=$T monster_distribution=backloaded stat_mode=single_great
tc g2-7 gen_rand n=$N b=$B t=$T monster_distribution=random stat_mode=weak
tc g2-many1 gen_many n=500  b=$B num_weird=0
tc g2-many2 gen_many n=1000 b=$B num_weird=0
tc g2-many3 gen_many n=$N   b=$B num_weird=0
tc g2-many4 gen_many n=$N   b=$B num_weird=10
tc g2-many5 gen_many n=$N   b=$B num_weird=100
tc g2-one1 gen_one b=$B t=$T mul=1 add=0
tc g2-one2 gen_one b=$B t=$T mul=2 add=5
tc g2-tailheavy1 gen_tailheavy n=$N b=$B t=$T
tc g2-expensive-good-1 gen_expensive n=$N b=$B

B=1000
group group3 20
limits maxb=$B
include_group sample
include_group group1
tc g3-1 gen_rand n=10 b=$B t=$MAXT monster_distribution=spiky spikes=1 stat_mode=ratio
tc g3-2 gen_rand n=10 b=$B t=$MAXT monster_distribution=spiky spikes=1 stat_mode=strong
tc g3-3 gen_rand n=10 b=$B t=$MAXT monster_distribution=spiky spikes=2 stat_mode=ratio
tc g3-4 gen_rand n=10 b=$B t=$MAXT monster_distribution=spiky spikes=2 stat_mode=strong
tc g3-5 gen_rand n=$MAXN b=$B t=$MAXT monster_distribution=backloaded stat_mode=weak
tc g3-6 gen_rand n=$MAXN b=$B t=$MAXT monster_distribution=backloaded stat_mode=single_great
tc g3-7 gen_rand n=$MAXN b=$B t=$MAXT monster_distribution=random stat_mode=weak
tc g3-many1 gen_many n=$MAXN b=$B num_weird=0
tc g3-many2 gen_many n=$MAXN b=$B num_weird=10
tc g3-many3 gen_many n=$MAXN b=$B num_weird=100
tc g3-many4 gen_many n=$MAXN b=$B num_weird=1000
tc g3-one1 gen_one b=$B t=$MAXT mul=1 add=0
tc g3-one2 gen_one b=$B t=$MAXT mul=2 add=5
tc g3-tailheavy1 gen_tailheavy n=$MAXN b=$B t=$MAXT

N=1
group group4 15
limits maxn=$N
tc g4-1 gen_rand n=1 b=$MAXB t=$MAXT monster_distribution=spiky spikes=1 stat_mode=ratio
tc g4-2 gen_rand n=1 b=$MAXB t=$MAXT monster_distribution=spiky spikes=1 stat_mode=strong
tc g4-3 gen_rand n=1 b=$MAXB t=$MAXT monster_distribution=spiky spikes=2 stat_mode=ratio
tc g4-4 gen_rand n=1 b=$MAXB t=$MAXT monster_distribution=spiky spikes=2 stat_mode=strong
tc g4-5 gen_rand n=1 b=$MAXB t=$MAXT monster_distribution=backloaded stat_mode=weak
tc g4-6 gen_rand n=1 b=$MAXB t=$MAXT monster_distribution=backloaded stat_mode=single_great
tc g4-7 gen_rand n=1 b=$MAXB t=$MAXT monster_distribution=random stat_mode=weak
tc g4-one1 gen_one b=$MAXB t=$MAXT mul=1 add=0
tc g4-one2 gen_one b=$MAXB t=$MAXT mul=2 add=5
tc g4-one3 gen_one b=$MAXB t=$MAXT mul=3 add=10
tc g4-one4 gen_one b=$MAXB t=$MAXT mul=5 add=100
tc g4-one5 gen_one b=$MAXB t=100 mul=1 add=0
tc g4-one6 gen_one b=$MAXB t=1 mul=1 add=0
tc_manual ../manual_testcases/staircase_bug

group group5 10
limits max_dungeon=10
include_group sample
tc g5-1 gen_rand n=$MAXN b=$MAXB t=$MAXT monster_distribution=even stat_mode=weak
tc g5-2 gen_rand n=$MAXN b=$MAXB t=$MAXT monster_distribution=even stat_mode=single_great
tc g5-3 gen_rand n=30000 b=$MAXB t=$MAXT monster_distribution=even stat_mode=weak
tc g5-4 gen_rand n=30000 b=$MAXB t=$MAXT monster_distribution=even stat_mode=single_great
tc g5-many1 gen_many n=40000 b=$MAXB num_weird=0
tc g5-many2 gen_many n=$MAXN b=$MAXB num_weird=10
tc g5-many3 gen_many n=$MAXN b=$MAXB num_weird=100
tc g5-many4 gen_many n=$MAXN b=$MAXB num_weird=1000
tc_manual ../manual_testcases/lowerbound_bug
tc staircase_bug
tc bug
tc bug2
tc bug3

group group6 27
include_group group2
include_group group3
include_group group4
include_group group5
tc g6-1 gen_rand n=10    b=$MAXB t=$MAXT monster_distribution=spiky spikes=1 stat_mode=ratio
tc g6-2 gen_rand n=10    b=$MAXB t=$MAXT monster_distribution=spiky spikes=1 stat_mode=strong
tc g6-3 gen_rand n=10    b=$MAXB t=$MAXT monster_distribution=spiky spikes=2 stat_mode=ratio
tc g6-4 gen_rand n=10    b=$MAXB t=$MAXT monster_distribution=spiky spikes=2 stat_mode=strong
tc g6-5 gen_rand n=$MAXN b=$MAXB t=$MAXT monster_distribution=backloaded stat_mode=weak
tc g6-6 gen_rand n=$MAXN b=$MAXB t=$MAXT monster_distribution=backloaded stat_mode=single_great
tc g6-7 gen_rand n=$MAXN b=$MAXB t=$MAXT monster_distribution=random stat_mode=weak
tc g6-tailheavy gen_tailheavy n=$N b=$B t=$T
