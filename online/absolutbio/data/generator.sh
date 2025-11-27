#!/usr/bin/env bash

PPATH=$(realpath ..)
. ../../../testdata_tools/gen.sh

use_solution harry.py

compile gen_rand.py

samplegroup
sample 1
sample 2
sample 3
sample 4

group group1 15
limits maxn=10000 maxq=1000
include_group sample
tc g1-01 gen_rand N=4 Q=20 fillup=1
tc g1-02 gen_rand N=8 Q=20 fillup=1
tc g1-03 gen_rand N=20 Q=20 fillup=3
tc g1-04 gen_rand N=1000 Q=100 fillup=10
tc g1-05 gen_rand N=5000 Q=1000 fillup=5
tc g1-06 gen_rand N=10 mode=onlyadd Q=10 fillup=1
tc g1-07 gen_rand N=100 mode=onlyadd Q=20 fillup=1
tc g1-08 gen_rand N=100 mode=onlyadd Q=200 fillup=1
tc g1-09 gen_rand N=10000 Q=1000 fillup=100 
tc g1-10 gen_rand N=10000 Q=1000 fillup=700 

group group2 25
limits maxn=10000
include_group sample
include_group group1
tc g2-01 gen_rand N=100 Q=200000 fillup=20
tc g2-02 gen_rand N=10000 Q=200000 fillup=500
tc g2-03 gen_rand N=10000 Q=200000 fillup=9000
tc g2-04 gen_rand N=10000 Q=200000 fillup=1
tc g2-05 gen_rand N=1000 mode=onlyadd Q=200000 fillup=1
tc g2-06 gen_rand N=10000 fillup=5000 mode=onlyadd

group group3 20
limits onlyadd=1
tc 1
tc g1-06
tc g1-07
tc g1-08
tc g2-05
tc g2-06
tc g3-01 gen_rand N=20000000 mode=onlyadd Q=10 fillup=1
tc g3-02 gen_rand mode=onlyadd Q=20 fillup=1
tc g3-03 gen_rand mode=onlyadd Q=200 fillup=1
tc g3-04 gen_rand mode=onlyadd Q=5000 fillup=1
tc g3-05 gen_rand mode=onlyadd Q=200000 fillup=1 
tc g3-06 gen_rand mode=onlyadd Q=200000 fillup=1 p=0.5
tc g3-07 gen_rand mode=onlyadd Q=200000 fillup=50 p=0.5
tc g3-08 gen_rand mode=onlyadd Q=200000 fillup=1000 p=0.1
tc g3-09 gen_rand mode=onlyadd Q=200000 fillup=1 p=0.9
tc g3-10 gen_rand mode=onlyadd Q=200000 fillup=5000 p=0.9

group group4 40
include_group sample
include_group group1
include_group group2
include_group group2
tc g4-01 gen_rand fillup=1
tc g4-02 gen_rand 
tc g4-03 gen_rand fillup=1 p=0.5
tc g4-04 gen_rand fillup=1 p=0.1
tc g4-05 gen_rand fillup=1 p=0.9
tc g4-06 gen_rand fillup=1 Q=200000
tc g4-07 gen_rand fillup=2 Q=200000 p2=1000
tc g4-08 gen_rand fillup=150000 p=0.4 Q=200000 p2=1000
tc g4-09 gen_rand fillup=1 p=0.3 Q=200000
tc g4-10 gen_rand fillup=1 p=0.99 Q=200000

