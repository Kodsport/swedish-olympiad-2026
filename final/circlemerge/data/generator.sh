#!/usr/bin/env bash

PPATH=$(realpath ..)

. ../../testdata_tools/gen.sh

ulimit -s unlimited

use_solution joshua.cpp opt

compile gen_rand.py
compile gen_evil2.py
compile gen_1s.py
compile gen_2s.py
compile gen_reverse.py

samplegroup
sample 1
sample 2

group g1 4
#limits start=1 
# make sure g1.py gets 4 points!
tc 2
tc g1-1 gen_reverse m=1 d=481880599200 r=0 lim=481880599200 start=1
tc g1-2 gen_reverse m=2 d=12957057192 r=0 start=1
tc g1-3 gen_reverse m=2 d=99999999999 r=0 start=1
tc g1-4 gen_reverse m=10 d=135 r=2 lim=10 start=1
tc g1-5 gen_reverse m=30 d=481880599200 r=2 start=1
tc g1-6 gen_reverse m=600 d=481880599200 r=1 start=1
tc g1-7 gen_reverse m=2000 d=321253732800 r=3 start=1
tc g1-8 gen_reverse m=150 d=3827939009895 r=4 start=1
tc g1-9 gen_reverse m=15360 d=183061376951 r=0 lim=100000 start=1

tc g2-allsame1 gen_reverse m=84 d=321253732800 lim=321253732800 r=0
tc g2-allsame2 gen_reverse m=100 d=4398046511104 lim=4398046511104 r=0

tc g3-allsame1 gen_reverse m=3000 d=187 lim=187 r=0
tc g4-allsame1 gen_reverse m=200000 d=187 lim=187 r=0
tc_manual ../manual/allsame.in
tc_manual ../manual/allsame2.in

# for i in {1..10}; do
#     tc g1-$i gen_rand n=5 mode=rand m=100000
# done
# tc g1-11 gen_2s x=6 
# for i in {12..15}; do
#     tc g1-$i gen_reverse m=$i d=10000
# done

group g2 15
limits maxn=100
include_group sample
tc g1-1 
tc g1-2 
tc g1-3 
tc g1-4
tc g2-rand1 gen_rand n=8 mode=rand m=10
tc g2-rand2 gen_rand n=100 mode=evil m=200
tc g2-rand3 gen_rand n=100 mode=pow2 m=5000000000000
tc g2-ones1 gen_1s n=100 base=1
tc g2-ones2 gen_1s n=100 base=183061376951
tc g2-twos1 gen_2s x=48 
tc g2-twos2 gen_2s x=48 base=9515003179 shift=10
tc g2-twos3 gen_2s x=41 base=2199023255552 shift=19
tc g2-allsame1 
tc g2-allsame2 
tc g2-HCN01 gen_reverse m=3 d=963761198400 lim=100000000 r=2 shift=99
tc g2-HCN02 gen_reverse m=9 d=642507465600 lim=4900000000 r=1 shift=3
tc g2-HCN03 gen_reverse m=30 d=7220177644680 lim=5000000000000 r=20000 shift=99
tc g2-HCN04 gen_reverse m=31 d=6987268688400 lim=5000000000000 r=20000 shift=98
tc g2-rand4 gen_rand n=100 mode=1and2 base=160626866400
tc_manual ../manual/specialn100.in
tc_manual ../manual/special2n100.in

group g3 22
limits maxn=3000 maxa=200
include_group sample
tc g1-4
tc g2-rand1
tc g2-rand2
tc g2-ones1
tc g2-twos1
tc g3-rand1 gen_rand n=3000 mode=1and2 base=91
tc g3-rand2 gen_rand n=3000 mode=rand m=200
tc g3-ones1 gen_1s n=3000 base=33
tc g3-twos1 gen_2s x=1498 base=91 shift=999
tc g3-allsame1 
tc g3-HCN01 gen_reverse m=1309 d=156 lim=155 r=0 shift=5
tc_manual ../manual/specialn3000.in
tc_manual ../manual/special2n3000.in
#187


group g4 29
limits maxa=200
include_group g3
tc g4-rand1 gen_rand n=200000 mode=rand m=200
tc g4-rand2 gen_rand n=200000 mode=evil m=200
tc g4-rand3 gen_rand n=200000 mode=pow2 m=200
#tc g4-rand4 gen_evil2
tc g4-ones1 gen_1s base=65
tc g4-twos1 gen_2s x=99998 base=91
tc g4-big1 gen_reverse m=62832 d=195 r=0 lim=120 shift=1
tc g4-big2 gen_reverse m=5712 d=2145 r=20 lim=200 shift=11
tc g4-big3 gen_reverse m=2145 d=5712 r=40 lim=123 shift=3
tc g4-allsame1
tc g4-rand5 gen_rand n=200000 mode=1and2 base=91

group g5 30
include_group g1
include_group g2
include_group g4
tc g5-rand1 gen_rand n=200000 mode=rand m=5000000000000
tc g5-rand2 gen_rand n=200000 mode=evil m=5000000000000
tc g5-rand3 gen_rand n=200000 mode=1and2 base=2199023255552
tc g5-rand4 gen_rand n=200000 mode=1and2 base=2199023255552 #4398046511104
tc g5-twos1 gen_2s x=99998 base=2199023255552
tc g5-twos2 gen_2s x=83159 base=2199023255552 shift=12345
tc g5-twos3 gen_2s x=83159 base=222893965954 shift=1000
tc_manual ../manual/bigcase.in

# small nums
tc g5-HCN01 gen_reverse m=83160 d=1 shift=11
tc g5-HCN02 gen_reverse m=10395 d=8 r=0 shift=11
tc g5-HCN03 gen_reverse m=8 d=10395 r=3 shift=11
tc g5-HCN04 gen_reverse m=10 d=8316 r=5 shift=11
tc g5-HCN05 gen_reverse m=8316 d=10 r=2 shift=11
tc g5-HCN06 gen_reverse m=1 d=83160 r=13 shift=11
tc g5-HCN07 gen_reverse m=308 d=270 r=4 shift=11
tc g5-HCN08 gen_reverse m=270 d=308 r=3 shift=11


# bigger nums
tc g5-HCN09 gen_reverse m=1287 d=190400 r=5 lim=5 shift=2
tc g5-HCN10 gen_reverse m=6175 d=1696464 r=4 lim=50000 shift=1
tc g5-HCN11 gen_reverse m=714 d=628580171419200 r=2000 lim=4500000000000  shift=12581
tc g5-HCN12 gen_reverse m=451 d=850011822714600 r=2000 lim=4100000000000  shift=125
tc g5-HCN13 gen_reverse m=216 d=1774793203908725 r=2000 lim=4350000000000  shift=69
tc g5-HCN14 gen_reverse m=1015 d=359704745056800 r=2000 lim=4700000000000  shift=420
tc g5-HCN15 gen_reverse m=6512 d=70642874455335 r=2000 lim=4700000000000  shift=1285
tc g5-HCN16 gen_reverse m=30030 d=15318894387384 r=2000 lim=4700000000000 shift=11
tc g5-HCN17 gen_reverse m=90720 d=494715875654 r=0 lim=125000000000 shift=1
tc g5-HCN18 gen_reverse m=31 d=14477620722364800 r=2000 lim=4500000000000 shift=2
tc g5-HCN19 gen_reverse m=33 d=13600189163433600 r=2000 lim=4600000000000 shift=102059
tc g5-HCN20 gen_reverse m=2093 d=214432031721600 r=2000 lim=4700000000000 shift=112059
tc g5-HCN21 gen_reverse m=1 d=383355332044284600 r=2000 lim=3857000000000 shift=1
tc g6-HCN22 gen_reverse m=2 d=224403121196654400 r=2000 lim=4500000000000 shift=100001
tc g6-HCN23 gen_reverse m=11 d=40800567490300800 r=2000 lim=4500000000000 shift=123456
tc g6-HCN24 gen_reverse m=323 d=1543881122784000 r=2000 lim=5000000000000 shift=49124
tc g6-HCN25 gen_reverse m=37638 d=9316358251200 r=2000 lim=3510000000000 shift=66667
tc g6-HCN26 gen_reverse m=66667 d=963761198400 r=0 lim=49000000000 shift=66667
tc_manual ../manual/allsame3.in                         
tc_manual ../manual/allsame4.in
tc_manual ../manual/allsame5.in
tc_manual ../manual/special.in
tc_manual ../manual/special2.in

# 2 3 5 7...
#tc g5-
# 766710664088569200
# 897612484786617600

# For each group
# 1 of each randomized mode
# evil2?
# 1s cases (change the basis as well)
# 2s cases (change the basis as well)
# create good HCN cases # https://gist.github.com/dario2994/fb4713f252ca86c1254d


#   $1$    & $4$        & Det finns en lösning med minimalt antal drag, där antalet stenar i varje påse är $a_1$. \\ \hline % O(n)
#   $2$    & $15$       & $N \le 100$ \\ \hline %O(n^2) for each divisor, or even O(n^3) - for each interval, check rest
#   $3$    & $22$       & $N \le 3000, a_i \le 200$ \\ \hline % O(divisors * n^2)
#   $4$    & $29$       & $a_i \le 200$ \\ \hline % Number of divisors is small. O(n * 100 ish) = O(n d(n))  + the sum is less than 10^8. no need for optimize divisors testing
#   $5$    & $30$       & Inga ytterligare begränsningar \\ \hline % O(n d(sum)/4). sum = 10^11? eller 10^17 = 10^12*10^5? eller 10^9*10^5 = 10^14