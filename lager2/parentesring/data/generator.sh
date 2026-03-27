#!/usr/bin/env bash

PPATH=$(realpath ..)

. ../../testdata_tools/gen.sh

# ulimit -s unlimited

use_solution harry.py

compile gen_rand.py
compile gen_full.py
compile gen_set.py
compile gen_targeted.py

samplegroup
sample 1
sample 2
sample 3

group g1 5
tc g1-01 gen_set n=1 mode=rand a=1 b=1000000000 s="("
tc g1-02 gen_full n=20 k=0 mode=rand a=1 b=1000000000 
tc g1-03 gen_full n=100 k=0 mode=rand a=1 b=1000000000 
tc g1-04 gen_full n=100 k=0 mode=rand a=1 b=1000000000 p=0.7
tc g1-05 gen_set n=1 mode=rand a=1 b=1000000000 s=")"
tc g1-06 gen_full n=10 k=10 mode=rand a=1 b=1000000000 p=0.2 
tc g1-07 gen_full n=90 k=9 mode=rand a=1 b=1000000000 
tc g1-08 gen_full n=70 k=30 mode=rand a=1 b=1000000000 p=0.7
tc g1-09 gen_full n=40 k=60 mode=fractal a=1 b=1000000000 
tc g1-10 gen_full n=100 k=0 mode=fractal a=1 b=1000000000 
tc g1-11 gen_full n=70 k=30 mode=fractal a=1 b=1000000000 
tc g1-12 gen_full n=70 k=30 spin=49 mode=fractal a=1 b=1000000000 
tc g1-13 gen_rand n=100 mode=rand a=1 b=1000000000
tc g1-14 gen_rand n=100 mode=equal a=1 b=1000000000
tc g1-15 gen_full n=1000 k=1000 mode=rand a=1 b=1000000000 p=0.1
tc g1-16 gen_full n=1700 k=299 mode=rand a=1 b=1000000000 p=0.5
tc g1-17 gen_full n=1800 k=199 mode=rand a=1 b=1000000000 p=0.9
tc g1-18 gen_full n=1900 k=99 mode=fractal a=1 b=1000000000
tc g1-19 gen_rand n=1999 mode=rand a=1 b=1000000000
tc g1-20 gen_rand n=2000 mode=equal a=1 b=1000000000
tc g1-21 gen_full n=450000 k=50000 mode=rand a=1 b=1000000000 p=0.1
tc g1-22 gen_full n=490000 k=10000 spin=12355 mode=rand a=1 b=1000000000 p=0.5
tc g1-23 gen_full n=499000 k=1000 mode=rand a=1 b=1000000000 p=0.9
tc g1-24 gen_full n=499000 k=1000 mode=fractal a=1 b=1000000000
tc g1-25 gen_rand n=499999 mode=rand a=1 b=1000000000
tc g1-26 gen_rand n=500000 mode=equal a=1 b=1000000000
tc g1-27 gen_rand n=499999 mode=sorted a=1 b=1000000000
tc g1-28 gen_rand n=500000 mode=sorted swaps=10 a=1 b=1000000000
tc g1-29 gen_rand n=499999 mode=sorted rev=1 a=1 b=1000000000
tc g1-30 gen_rand n=500000 mode=sorted rev=1 swaps=1000 a=1 b=1000000000
tc g1-31 gen_rand n=500000 mode=sorted rev=1 swaps=250000 a=1 b=1000000000


group g2 7
# include_group sample
# for i in {01..14}; do
#     tc g1-$i 
# done
tc g2-01 gen_full n=40 k=60 mode=rand a=999999999 b=1000000000 p=0.1
tc g2-02 gen_full n=70 k=30 spin=50 mode=rand a=50 b=20 p=0.5
tc g2-03 gen_full n=90 k=10 spin=10 mode=rand a=15 b=12 p=0.9
tc g2-04 gen_full n=70 k=30 spin=23 mode=fractal a=152 b=258
tc g2-05 gen_rand n=99 mode=rand a=67 b=69
tc g2-06 gen_rand n=100 mode=equal a=29 b=50
tc g2-07 gen_rand n=100 mode=sorted l=60 r=40 swaps=10 a=420 b=100
tc g2-08 gen_rand n=100 mode=sorted rev=1 swaps=50 a=112 b=223
tc g2-09 gen_targeted n=100 mode=half_half a=37 b=75
# Targeted counterexamples for equal.py's greedy excess-removal choice.
tc g2-10 gen_set n=12 a=170 b=160 s=")))()())))(("
tc g2-11 gen_set n=20 a=136 b=49 s="())))()))()()())(()("
tc g2-12 gen_set n=50 a=188 b=173 s=")((((()))))(((())))))))))((())))()(()))()((()()(()"
tc g2-13 gen_set n=100 a=23 b=35 s="))))()()))))()))()))(()()(()(())()))((())()))(((((()()((())()((()((((()))((((()())))))()())(((()))(("

group g3 8
limits maxn=2000
include_group sample
include_group g2
tc g3-01 gen_full n=500 k=100 mode=rand a=100 b=50 p=0.4
tc g3-02 gen_full n=1000 k=0 mode=rand a=5 b=3 p=0.7
tc g3-03 gen_full n=1800 k=200 mode=rand a=1000000000 b=1 p=0.5
tc g3-04 gen_rand n=2000 mode=equal a=500 b=300
tc g3-05 gen_rand n=1999 mode=rand a=1000000000 b=1000000000
tc g3-06 gen_targeted n=2000 mode=half_half a=100 b=1
tc g3-07 gen_rand n=2000 mode=sorted rev=1 swaps=500 a=7 b=3
tc g3-08 gen_targeted n=2000 mode=half_half a=37 b=75
# Larger equal.py counterexamples, to avoid only killing n<=100 special-cases.
tc g3-09 gen_rand n=500 mode=rand a=137 b=41 seed=3
tc g3-10 gen_rand n=2000 mode=rand a=137 b=41 seed=1

group g4 19
limits minb=0 maxb=0
tc g4-01 gen_targeted n=1 mode=all_left a=5 b=0
tc g4-02 gen_targeted n=2 mode=all_right a=1000000000 b=0
tc g4-03 gen_rand n=100 mode=equal a=100 b=0
tc g4-04 gen_rand n=99 mode=rand a=42 b=0
tc g4-05 gen_rand n=500000 mode=rand a=1000000000 b=0
tc g4-06 gen_rand n=500000 mode=equal a=999 b=0
tc g4-07 gen_targeted n=499999 mode=skewed a=7 b=0 left=200000
tc g4-08 gen_targeted n=500000 mode=all_right a=1000000000 b=0

group g5 11
limits equal=1
tc g5-01 gen_rand n=100 mode=equal a=1000000000 b=1
tc g5-02 gen_targeted n=100 mode=half_half a=1000000000 b=1
tc g5-03 gen_targeted n=500000 mode=half_half a=1000000000 b=1
tc g5-04 gen_rand n=500000 mode=equal a=1000000000 b=1
tc g5-05 gen_targeted n=500000 mode=alternating a=1000000000 b=1
tc g5-06 gen_rand n=500000 mode=sorted swaps=10 a=1000000000 b=1

group g6 14
include_group g5
tc g6-01 gen_rand n=100 mode=rand a=1000000000 b=1
tc g6-02 gen_rand n=500000 mode=rand a=1000000000 b=1
tc g6-03 gen_targeted n=500000 mode=skewed a=1000000000 b=1 left=200000
tc g6-04 gen_targeted n=499999 mode=all_left a=1000000000 b=1
tc g6-05 gen_rand n=500000 mode=sorted rev=1 swaps=10000 a=1000000000 b=1
tc g6-06 gen_targeted n=500000 mode=all_right a=1000000000 b=1
# Large equal.py counterexample under the g6 cost regime.
tc g6-07 gen_rand n=500000 mode=rand a=1000000000 b=1 seed=2

group g7 12
limits equal=1
include_group g5
tc g7-01 gen_rand n=100 mode=equal a=5 b=3
tc g7-02 gen_targeted n=100 mode=half_half a=1 b=1000000000
tc g7-03 gen_rand n=500000 mode=equal a=1 b=100
tc g7-04 gen_targeted n=500000 mode=half_half a=1 b=1
tc g7-05 gen_rand n=500000 mode=equal a=1000000000 b=1000000000
tc g7-06 gen_rand n=500000 mode=equal a=100 b=7
tc g7-07 gen_targeted n=500000 mode=half_half a=37 b=75

group g8 24
include_group sample
include_group g1
include_group g3
include_group g6
include_group g7
tc g8-01 gen_rand n=500000 mode=rand a=1000000000 b=1000000000
tc g8-02 gen_full n=490000 k=10000 mode=rand a=500000 b=500000 p=0.3
tc g8-03 gen_full n=499000 k=1000 mode=fractal a=7 b=3
tc g8-04 gen_targeted n=500000 mode=skewed a=100 b=50 left=100000
tc g8-05 gen_rand n=499999 mode=sorted swaps=100000 a=42 b=17
tc g8-06 gen_targeted n=1 mode=all_left a=1 b=1
# Large arbitrary-constraints equal.py counterexample.
tc g8-07 gen_rand n=500000 mode=rand a=73 b=29 seed=2
