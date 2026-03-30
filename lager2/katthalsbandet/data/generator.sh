#!/usr/bin/env bash

PPATH=$(realpath ..)

. ../../../testdata_tools/gen.sh

# ulimit -s unlimited

use_solution gemini_rasmus.cpp

compile gen_rand.py
compile gen_full.py
compile gen_set.py
compile gen_targeted.py

samplegroup
sample 1
sample 2
sample 3

group g1 11
limits mina=1 maxa=1 minb=1000000000
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

group g2 16
limits maxn=15
include_group sample
tc g1-01
tc g1-05
tc_manual ../manual/small.in
tc g2-01 gen_targeted n=1 mode=all_left a=1 b=1
tc g2-02 gen_targeted n=1 mode=all_right a=1000000000 b=1000000000
tc g2-03 gen_set n=2 a=5 b=3 s="()"
tc g2-04 gen_set n=2 a=5 b=3 s=")("
tc g2-05 gen_set n=7 a=4 b=1 s="()))((("
tc g2-06 gen_set n=11 a=9 b=10 s="()))((((())"
tc g2-07 gen_set n=5 a=10 b=3 s="())(("
tc g2-08 gen_set n=9 a=50 b=20 s="())))(((("
tc g2-09 gen_set n=12 a=170 b=160 s=")))()())))(("
tc g2-10 gen_set n=13 a=100 b=50 s="())))))(((((("
tc g2-11 gen_rand n=15 mode=rand a=67 b=69
tc g2-12 gen_rand n=14 mode=equal a=29 b=50
tc g2-13 gen_rand n=15 mode=rand a=1000000000 b=1
tc g2-14 gen_rand n=15 mode=rand a=1 b=1000000000
tc g2-15 gen_rand n=15 mode=rand a=0 b=50
tc g2-16 gen_set n=7 a=0 b=5 s="()))((("

group g3 19
limits maxn=100
include_group g2
tc_manual ../manual/n100_1.in
tc_manual ../manual/n100_2.in
tc_manual ../manual/n100_3.in
tc_manual ../manual/n100_4.in
tc g1-01 
tc g1-02 
tc g1-03
tc g1-04 
tc g1-05 
tc g1-06 
tc g1-07 
tc g1-08
tc g1-09 
tc g1-10
tc g1-11 
tc g1-12 
tc g1-13 
tc g1-14 
tc g3-01 gen_full n=40 k=60 mode=rand a=999999999 b=1000000000 p=0.1
tc g3-02 gen_full n=70 k=30 spin=50 mode=rand a=50 b=20 p=0.5
tc g3-03 gen_full n=90 k=10 spin=10 mode=rand a=15 b=12 p=0.9
tc g3-04 gen_full n=70 k=30 spin=23 mode=fractal a=152 b=258
tc g3-05 gen_rand n=99 mode=rand a=67 b=69
tc g3-06 gen_rand n=100 mode=equal a=29 b=50
tc g3-07 gen_rand n=100 mode=sorted l=60 r=40 swaps=10 a=420 b=100
tc g3-08 gen_rand n=100 mode=sorted rev=1 swaps=50 a=112 b=223
tc g3-09 gen_targeted n=100 mode=half_half a=37 b=75
tc g3-10 gen_set n=20 a=136 b=49 s="())))()))()()())(()("
tc g3-11 gen_set n=50 a=188 b=173 s=")((((()))))(((())))))))))((())))()(()))()((()()(()"
tc g3-12 gen_set n=100 a=23 b=35 s="))))()()))))()))()))(()()(()(())()))((())()))(((((()()((())()((()((((()))((((()())))))()())(((()))(("
tc g3-13 gen_rand n=50 mode=rand a=137 b=41 seed=3
tc g3-14 gen_rand n=100 mode=rand a=137 b=41 seed=1
tc_manual ../manual/low.in

group g4 21
limits maxn=2000
include_group g3
tc_manual ../manual/n2000_1.in
tc_manual ../manual/n2000_2.in
tc g4-01 gen_full n=500 k=100 mode=rand a=100 b=50 p=0.4
tc g4-02 gen_full n=1000 k=0 mode=rand a=5 b=3 p=0.7
tc g4-03 gen_full n=1800 k=200 mode=rand a=1000000000 b=1 p=0.5
tc g4-04 gen_rand n=2000 mode=equal a=500 b=300
tc g4-05 gen_rand n=1999 mode=rand a=1000000000 b=1000000000
tc g4-06 gen_targeted n=2000 mode=half_half a=100 b=1
tc g4-07 gen_rand n=2000 mode=sorted rev=1 swaps=500 a=7 b=3
tc g4-08 gen_targeted n=2000 mode=half_half a=37 b=75
tc g4-09 gen_rand n=2000 mode=rand a=73 b=29 seed=2
tc g4-10 gen_rand n=2000 mode=rand a=50 b=51 seed=5
tc_manual ../manual/medium.in

group g5 9
limits minb=0 maxb=0
tc g5-01 gen_targeted n=1 mode=all_left a=5 b=0
tc g5-02 gen_targeted n=2 mode=all_right a=1000000000 b=0
tc g5-03 gen_rand n=100 mode=equal a=100 b=0
tc g5-04 gen_rand n=99 mode=rand a=42 b=0
tc g5-05 gen_rand n=500000 mode=rand a=1000000000 b=0
tc g5-06 gen_rand n=500000 mode=equal a=999 b=0
tc g5-07 gen_targeted n=499999 mode=skewed a=7 b=0 left=200000
tc g5-08 gen_targeted n=500000 mode=all_right a=1000000000 b=0
tc g5-09 gen_rand n=500000 mode=rand a=0 b=0

group g6 8
limits mina=1000000000 minb=1 maxb=1 equal=1
tc g6-01 gen_rand n=100 mode=equal a=1000000000 b=1
tc g6-02 gen_targeted n=100 mode=half_half a=1000000000 b=1
tc g6-03 gen_targeted n=500000 mode=half_half a=1000000000 b=1
tc g6-04 gen_rand n=500000 mode=equal a=1000000000 b=1
tc g6-05 gen_targeted n=500000 mode=alternating a=1000000000 b=1
tc g6-06 gen_rand n=500000 mode=sorted swaps=10 a=1000000000 b=1



group g7 16
include_group sample
include_group g1
include_group g4
include_group g5
include_group g6
tc_manual ../manual/n5e5_1.in
tc_manual ../manual/n5e5_2.in
tc_manual ../manual/lowb_1.in
tc_manual ../manual/lowb_2.in
tc g7-01 gen_rand n=100 mode=rand a=1000000000 b=1
tc g7-02 gen_rand n=500000 mode=rand a=1000000000 b=1
tc g7-03 gen_targeted n=500000 mode=skewed a=1000000000 b=1 left=200000
tc g7-04 gen_targeted n=499999 mode=all_left a=1000000000 b=1
tc g7-05 gen_rand n=500000 mode=sorted rev=1 swaps=10000 a=1000000000 b=1
tc g7-06 gen_targeted n=500000 mode=all_right a=1000000000 b=1
tc g7-07 gen_rand n=500000 mode=rand a=1000000000 b=1 seed=2
tc g7-08 gen_rand n=500000 mode=sorted swaps=50000 a=1000000000 b=1
tc g7-09 gen_targeted n=500000 mode=skewed a=1000000000 b=1 left=300000
tc g7-10 gen_full n=499000 k=1000 mode=fractal a=1000000000 b=1
tc g7-11 gen_set n=7 a=1000000000 b=1 s="()))((("
tc g7-12 gen_set n=16 a=1000000000 b=1 s="())))))(((((((()"
tc g7-13 gen_set n=34 a=1000000000 b=1 s="())(()((((()(((()(()))(())()())()("
tc g7-14 gen_rand n=500000 mode=rand a=1000000000 b=1 seed=7

tc g8-01 gen_rand n=500000 mode=rand a=1000000000 b=1000000000
tc g8-02 gen_full n=490000 k=10000 mode=rand a=500000 b=500000 p=0.3
tc g8-03 gen_full n=499000 k=1000 mode=fractal a=7 b=3
tc g8-04 gen_targeted n=500000 mode=skewed a=100 b=50 left=100000
tc g8-05 gen_rand n=499999 mode=sorted swaps=100000 a=42 b=17
tc g8-06 gen_rand n=500000 mode=rand a=73 b=29 seed=2
tc g8-07 gen_rand n=500000 mode=rand a=50 b=51 seed=5
tc g8-08 gen_targeted n=500000 mode=skewed a=37 b=75 left=150000
tc g8-09 gen_full n=490000 k=10000 mode=rand a=3 b=7 p=0.6
tc g8-10 gen_targeted n=499999 mode=half_half a=100 b=100
tc g8-11 gen_rand n=500000 mode=rand a=100 b=37 seed=3
tc g8-12 gen_rand n=500000 mode=rand a=0 b=1000000000
tc_manual ../manual/big.in
