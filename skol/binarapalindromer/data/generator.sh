#!/usr/bin/env bash

PPATH=$(realpath ..)

. ../../testdata_tools/gen.sh

#ulimit -s unlimited

use_solution harry.py

compile gen_rand.py


samplegroup
sample 1
sample 2
sample 3

group g1 20
limits maxn=1000000000
#include_group sample
tc g1-1 gen_rand a=123456789 b=987654321 k=16

#for i in {2..50}; do
#  tc g1-$i gen_rand a=1 b=$((RANDOM * RANDOM % 1000000 + 1))
#done

group g2 20
limits maxk=4
#include_group g1
tc g2-1 gen_rand a=268451841 b=4503599694479361 k=3

#group g3 60
#include_group g2
group g3 20
tc g3-1 gen_rand a=123456789123456789 b=989999999999999992 k=29

group g4 20
tc g3-2 gen_rand a=664545914252227017 b=909090909090909090 k=34

group g5 20
tc g3-3 gen_rand a=77777777777        b=898662376785629667 k=30



