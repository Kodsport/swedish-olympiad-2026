#!/usr/bin/env bash

PPATH=$(realpath ..)

. ../../testdata_tools/gen.sh

ulimit -s unlimited

use_solution harry.py

compile gen_even.py
compile gen_rand.py
compile gen_switches.py
compile gen_manyans.py

samplegroup
limits maxn=10
sample 1
sample 2

group group1 10
limits maxm=1000 maxq=1000 maxn=1000
include_group sample
tc g1-1 gen_switches n=20 m=100 q=1000 maxw=1000000000
tc g1-2 gen_switches n=31 m=500 q=1000 maxw=1000000000
tc g1-3 gen_even n=35 m=1000 q=1000 maxw=1000000000
tc g1-4 gen_rand n=40 m=1000 q=1000 maxw=1
tc g1-5 gen_switches n=50 m=1000 q=1000 maxw=10
tc g1-6 gen_switches n=499 m=1000 q=1000 maxw=1000000000
tc g1-7 gen_rand n=1000 m=1000 q=1000 maxw=1000
tc g1-8 gen_switches n=300 m=600 q=1000 maxw=1000000000
tc g1-9 gen_rand n=300 m=300 q=1000 maxw=1000000000
tc g1-10 gen_even n=500 m=500 q=1000 maxw=1000000000
tc g1-11 gen_manyans n=200 m=1000 q=1000 maxw=1000000000
tc g1-12 gen_manyans n=900 s=810 m=1000 noise=1 q=1000 maxw=1000000000
tc g1-13 gen_rand n=400 m=1000 q=1000 maxw=1000000000
tc g1-14 gen_even n=600 m=1000 q=1000 maxw=1000000000
tc g1-15 gen_manyans n=600 m=1000 q=1000 maxw=1000000000
tc g1-16 gen_manyans n=660 m=1000 q=1000 maxw=1000000000 noise=1

group group2 13
limits maxval=10
include_group sample
tc g2-1 gen_rand n=1 m=1000 q=100000 maxw=10
tc g2-2 gen_rand n=20 m=5000 q=100000 maxw=10
tc g2-3 gen_switches n=200 m=10000 q=100000 maxw=10
tc g2-4 gen_even n=20000 m=300000 q=1 maxw=1
tc g2-5 gen_rand n=100000 m=300000 q=100000 maxw=10
tc g2-6 gen_switches n=100000 m=300000 q=100000 maxw=10
tc g2-7 gen_even n=100000 m=300000 q=100000 maxw=10
tc g2-8 gen_rand n=100000 m=300000 q=100000 maxw=10
tc g2-9 gen_even n=100000 m=100000 q=100000 maxw=10
tc g2-10 gen_switches n=100000 m=200000 q=100000 maxw=10
tc g2-11 gen_rand n=100000 m=300000 q=100000 maxw=10
tc g2-12 gen_rand n=100000 m=300000 q=100000 maxw=10
tc g2-13 gen_even n=100000 m=300000 q=100000 maxw=10
tc g2-14 gen_manyans n=100000 s=30000 m=300000 q=100000 maxw=10
tc g2-15 gen_manyans n=100000 noise=1 m=300000 q=100000 maxw=10

group group3 16
limits maxn=2
tc g3-1 gen_rand n=2 m=1000 q=100000 maxw=1000000000
tc g3-2 gen_switches n=2 m=5000 q=100000 maxw=1000000000
tc g3-3 gen_even n=2 m=10000 q=100000 maxw=1000000000
tc g3-4 gen_switches n=2 m=300000 q=1 maxw=1
tc g3-5 gen_switches n=2 m=300000 q=100000 maxw=10
tc g3-6 gen_rand n=2 m=300000 q=100000 maxw=1000000000
tc g3-7 gen_even n=2 m=300000 q=100000 maxw=1000
tc g3-8 gen_switches n=2 m=300000 q=100000 maxw=1000000000
tc g3-9 gen_rand n=2 m=30000 q=100000 maxw=1000000000
tc g3-10 gen_switches n=2 m=5000 q=100000 maxw=1000000000
tc g3-11 gen_manyans n=2 m=300000 q=100000 maxw=1000000000
tc g3-12 gen_rand n=2 m=300000 q=100000 maxw=1000000000
tc g3-13 gen_even n=2 m=300000 q=100000 maxw=1000000000
tc g3-14 gen_manyans n=2 m=300000 noise=1 q=100000 maxw=1000000000
tc g3-15 gen_even n=2 m=300000 q=100000 maxw=1000000000

group group4 22
limits maxn=25
include_group sample
include_group group3
tc g4-1 gen_switches n=3 m=1000 q=100000 maxw=1000000000
tc g4-2 gen_rand n=20 m=5000 q=100000 maxw=1000000000
tc g4-3 gen_even n=20 m=10000 q=100000 maxw=1000000000
tc g4-4 gen_rand n=20 m=300000 q=1 maxw=1000000000
tc g4-5 gen_rand n=25 m=300000 q=100000 maxw=1000000000
tc g4-6 gen_switches n=25 m=300000 q=100000 maxw=1000000000
tc g4-7 gen_even n=25 m=300000 q=100000 maxw=1000
tc g4-8 gen_switches n=25 m=300000 q=100000 maxw=1000000000
tc g4-9 gen_rand n=25 m=30000 q=100000 maxw=1000000000
tc g4-10 gen_switches n=25 m=5000 q=100000 maxw=1000000000
tc g4-11 gen_rand n=25 m=300000 q=100000 maxw=1000000000
tc g4-12 gen_even n=25 m=300000 q=100000 maxw=1000000000
tc g4-13 gen_switches n=25 m=300000 q=100000 maxw=1000000000
tc g4-14 gen_manyans n=25 noise=1 m=300000 q=100000 maxw=1000000000
tc g4-15 gen_manyans n=25 s=5 m=300000 q=100000 maxw=1000000000

group group5 39
include_group group1 group2 group3 group4
tc g5-1 gen_switches n=22222 m=100000 q=100000 maxw=1000000000
tc g5-2 gen_rand n=29999 m=300000 q=100000 maxw=1000000000
tc g5-3 gen_switches n=99999 m=300000 q=100000 maxw=1000000000
tc g5-4 gen_even n=20000 m=300000 q=100000 maxw=1000000000
tc g5-5 gen_rand n=100000 m=300000 q=100000 maxw=1000000000
tc g5-6 gen_switches n=100000 m=300000 q=100000 maxw=1000000000
tc g5-7 gen_switches n=100000 m=300000 q=100000 maxw=1000
tc g5-8 gen_rand n=100000 m=300000 q=100000 maxw=1000000000
tc g5-9 gen_switches n=100000 m=200000 q=100000 maxw=1000000000
tc g5-10 gen_rand n=100000 m=100000 q=100000 maxw=1000000000
tc g5-11 gen_even n=100000 m=300000 q=100000 maxw=1000000000
tc g5-12 gen_switches n=100000 m=300000 q=100000 maxw=1000000000
tc g5-13 gen_manyans n=100000 s=12345 m=300000 q=100000 maxw=1000000000
tc g5-14 gen_manyans n=100000 s=90000 m=300000 q=100000 maxw=1000000000
tc g5-15 gen_manyans n=100000 noise=1 m=300000 q=100000 maxw=1000000000

