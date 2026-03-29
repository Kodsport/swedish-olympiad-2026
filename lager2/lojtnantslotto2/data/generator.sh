#!/usr/bin/env bash

PPATH=$(realpath ..)
. ../../../testdata_tools/gen.sh

#ulimit -s unlimited

use_solution nils.py

compile gen.py

samplegroup
sample_manual 1
sample_manual 2

MAXN=1000

group group1 11
limits identity=1
tc g1-1 gen n=2 mode=identity
tc g1-2 gen n=3 mode=identity
tc g1-3 gen n=$(($MAXN-1)) mode=identity
tc g1-4 gen n=$MAXN mode=identity

group group2 12
limits maxn=5
tc 1
tc 2
tc g2-1 gen n=5 mode=random
tc g2-2 gen n=4 mode=random
tc g2-3 gen n=3 mode=random
tc g2-4 gen n=5 mode=random
tc g2-5 gen n=5 mode=random
tc g2-6 gen n=5 mode=random
tc g2-7 gen n=5 mode=random
tc g2-8 gen n=5 mode=random
tc g2-9 gen n=5 mode=partition part="[2,3]"
tc g2-10 gen n=4 mode=random
tc g2-11 gen n=4 mode=random
tc g2-12 gen n=4 mode=random
tc g2-13 gen n=4 mode=random
tc g2-14 gen n=3 mode=random
tc g2-15 gen n=3 mode=random
tc g2-16 gen n=3 mode=random
tc g2-17 gen n=3 mode=random
tc g2-18 gen n=3 mode=random
tc g2-19 gen n=2 mode=partition part="[2]"
tc g2-20 gen n=4 mode=partition part="[2,2]"

group group3 15
limits one_cycle=1
tc 2
tc g3-1 gen n=3 mode=one_cycle
tc g3-2 gen n=$(($MAXN-1)) mode=one_cycle
tc g3-3 gen n=$MAXN mode=one_cycle
tc g3-4 gen n=4 mode=one_cycle
tc g3-5 gen n=5 mode=one_cycle
tc g3-6 gen n=6 mode=one_cycle
tc g3-7 gen n=7 mode=one_cycle
tc g3-8 gen n=2 mode=one_cycle

group group4 24
limits no_short_cycles=1
tc g4-1 gen n=$MAXN mode=random_no_short
tc g4-2 gen n=$MAXN mode=random_no_short
tc g4-3 gen n=$MAXN mode=random_no_short
tc g4-4 gen n=$MAXN mode=random_no_short
tc g4-5 gen n=$MAXN mode=partition part="[$(($MAXN-4)), 4]"
tc g4-6 gen n=5 mode=random_no_short
tc g4-7 gen n=5 mode=random_no_short
tc g4-8 gen n=5 mode=random_no_short
tc g4-9 gen n=5 mode=random_no_short
tc g4-10 gen n=5 mode=random_no_short
tc g4-11 gen n=5 mode=random_no_short
tc g4-12 gen n=4 mode=partition part="[4]"
tc g4-13 gen n=4 mode=partition part="[4]"
tc g4-14 gen n=4 mode=partition part="[4]"
tc g4-15 gen n=4 mode=partition part="[4]"
tc g4-16 gen n=4 mode=partition part="[4]"
tc g4-17 gen n=999 mode=partition part="[3,4,5,7,11,13,907]"
tc g4-18 gen n=999 mode=partition part="[3,4,5,7,11,13,907]"
tc g4-19 gen n=999 mode=partition part="[3,4,5,7,11,13,907]"
tc g4-20 gen n=999 mode=partition part="[3,4,5,7,11,13,907]"

group group5 38
include_group group1
include_group group2
include_group group3
include_group group4
tc g5-1 gen n=$MAXN mode=random
tc g5-2 gen n=$MAXN mode=random
tc g5-3 gen n=$MAXN mode=random
tc g5-4 gen n=$MAXN mode=random
tc g5-5 gen n=$(($MAXN-1)) mode=partition part="[$(($MAXN-3)), 2]"
tc g5-6 gen n=999 mode=partition part="[995, 2, 2]"
tc g5-7 gen n=999 mode=partition part="[111,111,111,111,111,111,111,111,109, 2]"
tc g5-8 gen n=998 mode=p23
tc g5-9 gen n=998 mode=p23_reverse
tc g5-10 gen n=999 mode=partition part="[2,2]+[3]*300"
tc g5-11 gen n=999 mode=partition part="[2,2]+[1]*990"
tc g5-12 gen n=999 mode=partition part="[2]+[1]*990"
tc g5-13 gen n=999 mode=partition part="[2]*499"
tc g5-14 gen n=999 mode=partition part="[2,666] + [3]*100"
tc g5-15 gen n=999 mode=partition part="[2,2]+[3]*300"
tc g5-16 gen n=999 mode=partition part="[2,2]+[1]*990"
tc g5-17 gen n=999 mode=partition part="[2,2]+[3]*300"
tc g5-18 gen n=999 mode=partition part="[2,2]+[1]*990"
tc g5-19 gen n=1000 mode=primes
tc g5-20 gen n=1000 mode=primes twos=1
tc g5-21 gen n=999 mode=partition part="[3,4,5,7,11,13]"
tc g5-22 gen n=999 mode=partition part="[2,3,4,5,7,11,13,907]" shuff=0
tc g5-23 gen n=999 mode=partition part="[3,4,5,7,11,13] + [1]*950" shuff=0
tc g5-24 gen n=1000 mode=partition part="[2,3,4,5,7,11,13] + [1]*955" shuff=0
tc g5-25 gen n=1000 mode=partition part="[2,2,3,5,7,11,13] + [1]*955" shuff=0
tc g5-26 gen n=1000 mode=partition part="[2,3,4,5,7,11,13] + [1]*955" shuff=0
tc g5-27 gen n=1000 mode=partition part="[2,3,4,5,7,11,13] + [1]*955" shuff=0
tc g5-28 gen n=999 mode=partition part="[2]*256"
tc g5-29 gen n=1000 mode=partition part="[2,3,4,5,7,11,13] + [1]*955" shuff=0
tc g5-30 gen n=1000 mode=partition part="[2,3,4,5,7,11,13] + [1]*955" shuff=0
tc g5-31 gen n=1000 mode=partition part="[2,3,4,5,7,11,13] + [1]*955" shuff=0
tc g5-32 gen n=1000 mode=partition part="[2,3,4,5,7,11,13] + [1]*955" shuff=0
tc g5-33 gen n=1000 mode=partition part="[2,3,4,5,7,11,13] + [1]*955" shuff=0
tc g5-34 gen n=1000 mode=partition part="[2,3,4,5,7,11,13] + [1]*955" shuff=0
tc g5-35 gen n=1000 mode=partition part="[2,3,4,5,7] + [1]*950"
tc g5-36 gen n=1000 mode=primes twos=-1

tc g5-37 gen n=1000 mode=partition part="[2,3,4,5,7,11,13] + [1]*955" shuff=0
tc g5-38 gen n=1000 mode=partition part="[2,3,4,5,7,11,13] + [1]*955" shuff=0
tc g5-39 gen n=1000 mode=partition part="[2,3,4,5,7,11,13] + [1]*955" shuff=0
tc g5-40 gen n=1000 mode=partition part="[2,3,4,5,7,11,13] + [1]*955" shuff=0
tc g5-41 gen n=1000 mode=partition part="[2,3,4,5,7,11,13] + [1]*955" shuff=0
tc g5-42 gen n=1000 mode=partition part="[2,3,4,5,7,11,13] + [1]*955" shuff=0
tc g5-43 gen n=1000 mode=partition part="[2,3,4,5,7,11,13] + [1]*955" shuff=0
tc g5-44 gen n=1000 mode=partition part="[2,3,4,5,7,11,13] + [1]*955" shuff=0
tc g5-45 gen n=1000 mode=partition part="[2,3,4,5,7,11,13] + [1]*955" shuff=0
tc g5-46 gen n=1000 mode=partition part="[2,3,4,5,7,11,13] + [1]*955" shuff=0
tc g5-47 gen n=1000 mode=partition part="[2,3,4,5,7,11,13] + [1]*955" shuff=0
tc g5-48 gen n=1000 mode=partition part="[2,3,4,5,7,11,13] + [1]*955" shuff=0
tc g5-49 gen n=1000 mode=partition part="[2,3,4,5,7,11,13] + [1]*955" shuff=0
tc g5-50 gen n=1000 mode=partition part="[2,3,4,5,7,11,13] + [1]*955" shuff=0
tc g5-51 gen n=1000 mode=partition part="[2,3,4,5,7,11,13] + [1]*955" shuff=0


