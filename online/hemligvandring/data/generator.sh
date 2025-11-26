#!/bin/bash
REQUIRE_SAMPLE_REUSE=0
PPATH=$(realpath ..)
. gen.sh

use_solution harry.py

compile gen_rand.py

samplegroup
limits graded=0
sample 1
sample 2

group group1 5
limits maxn=5 graded=1
#tc 1
tc g1-01 gen_rand T=1 n=5 x1=3 y1=3 x2=1 y1=1
tc g1-02 gen_rand T=1 n=5 x1=1 y1=3 x2=3 y1=1
tc g1-03 gen_rand T=1 n=5 x1=1 y1=1 x2=3 y1=3
tc g1-04 gen_rand T=1 n=5 x1=3 y1=1 x2=1 y1=3

group group2 9
limits maxn=9 graded=2
tc g2-01 gen_rand T=2 n=5 x1=3 y1=3 x2=1 y1=1
tc g2-02 gen_rand T=2 n=5 x1=1 y1=3 x2=3 y1=1
tc g2-03 gen_rand T=2 n=5 x1=1 y1=1 x2=3 y1=3
tc g2-04 gen_rand T=2 n=5 x1=3 y1=1 x2=1 y1=3
tc g2-05 gen_rand T=2 n=7 x1=1 y1=1 x2=3 y2=3
tc g2-06 gen_rand T=2 n=7 x1=1 y1=1 x2=3 y2=5
tc g2-07 gen_rand T=2 n=7 x1=1 y1=1 x2=5 y2=3
tc g2-08 gen_rand T=2 n=7 x1=1 y1=1 x2=5 y2=5
tc g2-09 gen_rand T=2 n=7 x1=1 y1=3 x2=3 y2=5
tc g2-10 gen_rand T=2 n=7 x1=1 y1=3 x2=5 y2=5
tc g2-11 gen_rand T=2 n=7 x1=3 y1=1 x2=5 y2=3
tc g2-12 gen_rand T=2 n=7 x1=3 y1=1 x2=5 y2=5
tc g2-13 gen_rand T=2 n=7 x1=3 y1=3 x2=5 y2=5
tc g2-14 gen_rand T=2 n=9 x1=1 y1=1 x2=3 y2=3
tc g2-15 gen_rand T=2 n=9 x1=1 y1=1 x2=3 y2=5
tc g2-16 gen_rand T=2 n=9 x1=1 y1=1 x2=3 y2=7
tc g2-17 gen_rand T=2 n=9 x1=1 y1=3 x2=3 y2=5
tc g2-18 gen_rand T=2 n=9 x1=1 y1=3 x2=3 y2=7
tc g2-19 gen_rand T=2 n=9 x1=1 y1=5 x2=3 y2=7
tc g2-20 gen_rand T=2 n=9 x1=1 y1=1 x2=5 y2=3
tc g2-21 gen_rand T=2 n=9 x1=1 y1=1 x2=5 y2=5
tc g2-22 gen_rand T=2 n=9 x1=1 y1=1 x2=5 y2=7
tc g2-23 gen_rand T=2 n=9 x1=1 y1=3 x2=5 y2=5
tc g2-24 gen_rand T=2 n=9 x1=1 y1=3 x2=5 y2=7
tc g2-25 gen_rand T=2 n=9 x1=1 y1=5 x2=5 y2=7
tc g2-26 gen_rand T=2 n=9 x1=1 y1=1 x2=7 y2=3
tc g2-27 gen_rand T=2 n=9 x1=1 y1=1 x2=7 y2=5
tc g2-28 gen_rand T=2 n=9 x1=1 y1=1 x2=7 y2=7
tc g2-29 gen_rand T=2 n=9 x1=1 y1=3 x2=7 y2=5
tc g2-30 gen_rand T=2 n=9 x1=1 y1=3 x2=7 y2=7
tc g2-31 gen_rand T=2 n=9 x1=1 y1=5 x2=7 y2=7
tc g2-32 gen_rand T=2 n=9 x1=3 y1=1 x2=5 y2=3
tc g2-33 gen_rand T=2 n=9 x1=3 y1=1 x2=5 y2=5
tc g2-34 gen_rand T=2 n=9 x1=3 y1=1 x2=5 y2=7
tc g2-35 gen_rand T=2 n=9 x1=3 y1=3 x2=5 y2=5
tc g2-36 gen_rand T=2 n=9 x1=3 y1=3 x2=5 y2=7
tc g2-37 gen_rand T=2 n=9 x1=3 y1=5 x2=5 y2=7
tc g2-38 gen_rand T=2 n=9 x1=3 y1=1 x2=7 y2=3
tc g2-39 gen_rand T=2 n=9 x1=3 y1=1 x2=7 y2=5
tc g2-40 gen_rand T=2 n=9 x1=3 y1=1 x2=7 y2=7
tc g2-41 gen_rand T=2 n=9 x1=3 y1=3 x2=7 y2=5
tc g2-42 gen_rand T=2 n=9 x1=3 y1=3 x2=7 y2=7
tc g2-43 gen_rand T=2 n=9 x1=3 y1=5 x2=7 y2=7
tc g2-44 gen_rand T=2 n=9 x1=5 y1=1 x2=7 y2=3
tc g2-45 gen_rand T=2 n=9 x1=5 y1=1 x2=7 y2=5
tc g2-46 gen_rand T=2 n=9 x1=5 y1=1 x2=7 y2=7
tc g2-47 gen_rand T=2 n=9 x1=5 y1=3 x2=7 y2=5
tc g2-48 gen_rand T=2 n=9 x1=5 y1=3 x2=7 y2=7
tc g2-49 gen_rand T=2 n=9 x1=5 y1=5 x2=7 y2=7



group group3 11
limits corner=1 graded=3
#tc 1
tc g3-01 gen_rand T=3 n=5 x1=1 y1=1 x2=3 y1=3
tc g3-02 gen_rand T=3 n=7 x1=1 y1=1 x2=3 y2=3
tc g3-03 gen_rand T=3 n=7 x1=1 y1=1 x2=3 y2=5
tc g3-04 gen_rand T=3 n=7 x1=1 y1=1 x2=5 y2=3
tc g3-05 gen_rand T=3 n=7 x1=1 y1=1 x2=5 y2=5
tc g3-06 gen_rand T=3 n=9 x1=1 y1=1 x2=3 y2=3
tc g3-07 gen_rand T=3 n=9 x1=1 y1=1 x2=3 y2=5
tc g3-08 gen_rand T=3 n=9 x1=1 y1=1 x2=3 y2=7
tc g3-09 gen_rand T=3 n=9 x1=1 y1=1 x2=5 y2=3
tc g3-10 gen_rand T=3 n=9 x1=1 y1=1 x2=5 y2=5
tc g3-11 gen_rand T=3 n=9 x1=1 y1=1 x2=5 y2=7
tc g3-12 gen_rand T=3 n=9 x1=1 y1=1 x2=7 y2=3
tc g3-13 gen_rand T=3 n=9 x1=1 y1=1 x2=7 y2=5
tc g3-14 gen_rand T=3 n=9 x1=1 y1=1 x2=7 y2=7
for i in {15..20}; do 
    tc g3-$i gen_rand T=3 n=35 x1=1 y1=1 seed=$i
done

for i in {21..23}; do 
    tc g3-$i gen_rand T=3 n=69 x1=1 y1=1 seed=$i
done

for i in {24..34}; do 
    tc g3-$i gen_rand T=3 n=75 x1=1 y1=1 seed=$i
done

tc g3-35 gen_rand T=3 n=75 x1=1 y1=1 x2=73 y2=73
tc g3-36 gen_rand T=3 n=75 x1=1 y1=1 x2=31 y2=73
tc g3-37 gen_rand T=3 n=75 x1=1 y1=1 x2=25 y2=25
tc g3-38 gen_rand T=3 n=75 x1=1 y1=1 x2=69 y2=67

group group4 75
limits graded=4
tc g4-01 gen_rand T=4 n=5 x1=3 y1=3 x2=1 y1=1
tc g4-02 gen_rand T=4 n=5 x1=1 y1=3 x2=3 y1=1
tc g4-03 gen_rand T=4 n=5 x1=1 y1=1 x2=3 y1=3
tc g4-04 gen_rand T=4 n=5 x1=3 y1=1 x2=1 y1=3
tc g4-05 gen_rand T=4 n=7 x1=1 y1=1 x2=3 y2=3
tc g4-06 gen_rand T=4 n=7 x1=1 y1=1 x2=3 y2=5
tc g4-07 gen_rand T=4 n=7 x1=1 y1=1 x2=5 y2=3
tc g4-08 gen_rand T=4 n=7 x1=1 y1=1 x2=5 y2=5
tc g4-09 gen_rand T=4 n=7 x1=1 y1=3 x2=3 y2=5
tc g4-10 gen_rand T=4 n=7 x1=1 y1=3 x2=5 y2=5
tc g4-11 gen_rand T=4 n=7 x1=3 y1=1 x2=5 y2=3
tc g4-12 gen_rand T=4 n=7 x1=3 y1=1 x2=5 y2=5
tc g4-13 gen_rand T=4 n=7 x1=3 y1=3 x2=5 y2=5
tc g4-14 gen_rand T=4 n=9 x1=1 y1=1 x2=3 y2=3
tc g4-15 gen_rand T=4 n=9 x1=1 y1=1 x2=3 y2=5
tc g4-16 gen_rand T=4 n=9 x1=1 y1=1 x2=3 y2=7
tc g4-17 gen_rand T=4 n=9 x1=1 y1=3 x2=3 y2=5
tc g4-18 gen_rand T=4 n=9 x1=1 y1=3 x2=3 y2=7
tc g4-19 gen_rand T=4 n=9 x1=1 y1=5 x2=3 y2=7
tc g4-20 gen_rand T=4 n=9 x1=1 y1=1 x2=5 y2=3
tc g4-21 gen_rand T=4 n=9 x1=1 y1=1 x2=5 y2=5
tc g4-22 gen_rand T=4 n=9 x1=1 y1=1 x2=5 y2=7
tc g4-23 gen_rand T=4 n=9 x1=1 y1=3 x2=5 y2=5
tc g4-24 gen_rand T=4 n=9 x1=1 y1=3 x2=5 y2=7
tc g4-25 gen_rand T=4 n=9 x1=1 y1=5 x2=5 y2=7
tc g4-26 gen_rand T=4 n=9 x1=1 y1=1 x2=7 y2=3
tc g4-27 gen_rand T=4 n=9 x1=1 y1=1 x2=7 y2=5
tc g4-28 gen_rand T=4 n=9 x1=1 y1=1 x2=7 y2=7
tc g4-29 gen_rand T=4 n=9 x1=1 y1=3 x2=7 y2=5
tc g4-30 gen_rand T=4 n=9 x1=1 y1=3 x2=7 y2=7
tc g4-31 gen_rand T=4 n=9 x1=1 y1=5 x2=7 y2=7
tc g4-32 gen_rand T=4 n=9 x1=3 y1=1 x2=5 y2=3
tc g4-33 gen_rand T=4 n=9 x1=3 y1=1 x2=5 y2=5
tc g4-34 gen_rand T=4 n=9 x1=3 y1=1 x2=5 y2=7
tc g4-35 gen_rand T=4 n=9 x1=3 y1=3 x2=5 y2=5
tc g4-36 gen_rand T=4 n=9 x1=3 y1=3 x2=5 y2=7
tc g4-37 gen_rand T=4 n=9 x1=3 y1=5 x2=5 y2=7
tc g4-38 gen_rand T=4 n=9 x1=3 y1=1 x2=7 y2=3
tc g4-39 gen_rand T=4 n=9 x1=3 y1=1 x2=7 y2=5
tc g4-40 gen_rand T=4 n=9 x1=3 y1=1 x2=7 y2=7
tc g4-41 gen_rand T=4 n=9 x1=3 y1=3 x2=7 y2=5
tc g4-42 gen_rand T=4 n=9 x1=3 y1=3 x2=7 y2=7
tc g4-43 gen_rand T=4 n=9 x1=3 y1=5 x2=7 y2=7
tc g4-44 gen_rand T=4 n=9 x1=5 y1=1 x2=7 y2=3
tc g4-45 gen_rand T=4 n=9 x1=5 y1=1 x2=7 y2=5
tc g4-46 gen_rand T=4 n=9 x1=5 y1=1 x2=7 y2=7
tc g4-47 gen_rand T=4 n=9 x1=5 y1=3 x2=7 y2=5
tc g4-48 gen_rand T=4 n=9 x1=5 y1=3 x2=7 y2=7
tc g4-49 gen_rand T=4 n=9 x1=5 y1=5 x2=7 y2=7

tc g4-50 gen_rand T=4 n=35 x1=1 y1=1 seed=15
tc g4-51 gen_rand T=4 n=35 x1=1 y1=1 seed=16
tc g4-52 gen_rand T=4 n=35 x1=1 y1=1 seed=17
tc g4-53 gen_rand T=4 n=35 x1=1 y1=1 seed=18
tc g4-54 gen_rand T=4 n=35 x1=1 y1=1 seed=19
tc g4-55 gen_rand T=4 n=35 x1=1 y1=1 seed=20

tc g4-56 gen_rand T=4 n=69 x1=1 y1=1 seed=21
tc g4-57 gen_rand T=4 n=69 x1=1 y1=1 seed=22
tc g4-58 gen_rand T=4 n=69 x1=1 y1=1 seed=23

tc g4-59 gen_rand T=4 n=75 x1=1 y1=1 seed=24
tc g4-60 gen_rand T=4 n=75 x1=1 y1=1 seed=25
tc g4-61 gen_rand T=4 n=75 x1=1 y1=1 seed=26
tc g4-62 gen_rand T=4 n=75 x1=1 y1=1 seed=27
tc g4-63 gen_rand T=4 n=75 x1=1 y1=1 seed=28
tc g4-64 gen_rand T=4 n=75 x1=1 y1=1 seed=29
tc g4-65 gen_rand T=4 n=75 x1=1 y1=1 seed=30
tc g4-66 gen_rand T=4 n=75 x1=1 y1=1 seed=31
tc g4-67 gen_rand T=4 n=75 x1=1 y1=1 seed=32
tc g4-68 gen_rand T=4 n=75 x1=1 y1=1 seed=33
tc g4-69 gen_rand T=4 n=75 x1=1 y1=1 seed=34

tc g4-70 gen_rand T=4 n=75 x1=1 y1=1 x2=73 y2=73
tc g4-71 gen_rand T=4 n=75 x1=1 y1=1 x2=31 y2=73
tc g4-72 gen_rand T=4 n=75 x1=1 y1=1 x2=25 y2=25
tc g4-73 gen_rand T=4 n=75 x1=1 y1=1 x2=69 y2=67

for i in {74..88}; do 
    tc g4-$i gen_rand T=4 n=75
done
tc g4-89 gen_rand T=4 n=75 x1=1 y1=71 x2=3 y2=1
tc g4-90 gen_rand T=4 n=75 x1=1 y1=1 x2=73 y2=25