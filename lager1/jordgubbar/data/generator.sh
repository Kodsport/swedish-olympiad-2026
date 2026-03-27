#!/usr/bin/env bash
REQUIRE_SAMPLE_REUSE=0
PPATH=$(realpath ..)
#. ../../testdata_tools/gen.sh
. gen.sh

#ulimit -s unlimited

use_solution nils.py

compile gen_random.py

MAXN=300

samplegroup
limits T=0
sample_manual 1

group group1 16
limits T=1
tc m2-1 gen_random T=1 n=273 m=2
tc m2-2 gen_random T=1 n=$MAXN mode=m2 ones=0
tc m2-3 gen_random T=1 n=$MAXN mode=m2 ones=1
tc m2-4 gen_random T=1 n=$MAXN mode=m2 ones=11
tc m2-5 gen_random T=1 n=$MAXN mode=m2 ones=290
tc m2-6 gen_random T=1 n=$MAXN mode=m2 ones=299
tc m2-7 gen_random T=1 n=$MAXN mode=m2 ones=300
tc m2-8 gen_random T=1 n=2 m=2

group group2 30
limits T=2
tc mn-1 gen_random n=$MAXN m=$MAXN T=2
tc mn-2 gen_random n=263 m=263 T=2
tc mn-3 gen_random n=3 m=3 T=2
tc mn-4 gen_random n=2 m=2 T=2
tc mn-5 gen_random n=$MAXN m=$MAXN mode=list w="list(range(1,m+1))" T=2
tc mn-6 gen_random n=$MAXN m=$MAXN mode=list w="[1]*n" T=2
tc mn-7 gen_random n=$MAXN m=$MAXN mode=list w="[m]*n" T=2

group group3 54
limits T=3
tc g3-1 gen_random n=$MAXN m=$MAXN T=3
tc g3-2 gen_random n=$MAXN m=100 T=3
tc g3-3 gen_random n=$MAXN m=30 T=3
tc g3-4 gen_random n=$MAXN m=10 T=3
tc g3-5 gen_random n=$MAXN m=5 T=3
tc g3-6 gen_random n=$MAXN m=2 T=3
tc g3-7 gen_random T=3 m=200 mode=list w="[1]*290 + [200]*10"
tc g3-8 gen_random T=3 m=150 mode=list w="[1]*150 + [150]*150"
tc g3-9 gen_random T=3 m=$MAXN mode=list w="[100]*150 + [150]*150"
tc g3-10 gen_random T=3 m=100 mode=list w="[1]*10 + [97]*290"
tc g3-11 gen_random T=3 m=3 mode=list w="[1,1,2,3,3]"

