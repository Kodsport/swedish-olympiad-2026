#!/usr/bin/env bash

PPATH=$(realpath ..)
. ../../../testdata_tools/gen.sh

use_solution joshua.cpp

compile gen_rand.py
compile gen_1.py
compile gen_anti_pisinger.py
compile chatgpt_killer.py

MAX_T=1000000

samplegroup
limits maxn=10 maxl=5 maxt=10
sample 1
sample 2

group group1 7
limits maxl=1
tc g1-1 gen_1 n=200000 t=$(($MAX_T-200)) mode=nope
tc g1-2 gen_1 n=200000 t=$MAX_T mode=one
tc g1-3 gen_1 n=200000 t=$MAX_T mode=arithm

group group2 12
limits maxn=500 maxt=500
include_group sample
tc_manual ../manual_tests/antigreedy/random_fail_g2.in
tc_manual ../manual_tests/antishuffle/small_manyshuffles.in
tc_manual ../manual_tests/antishuffle/antishuffle_g2.in
tc_manual ../manual_tests/antigreedy/antigreedy_dp_g2.in
tc_manual ../manual_tests/antigreedy/anti_convolution_g2.in
tc_manual ../manual_tests/antigreedy/anti_chatgptgreedy_g2.in
tc_manual ../manual_tests/antigreedy/anti_vincent_rollback_g2.in
tc_manual ../manual_tests/antigreedy/anti_vincent_rollback_min_g2.in
tc_manual ../manual_tests/antigreedy/anti_dc_holes_g2.in
tc_manual ../manual_tests/antigreedy/antivlad_budget6_g2.in
tc_manual ../manual_tests/antigreedy/antivlad_v5swap_g2.in
tc_manual ../manual_tests/strengthen/anti_convolution_g2_2.in
tc_manual ../manual_tests/strengthen/anti_mans_martin_g2.in
tc_manual ../manual_tests/strengthen/anti_teo_g2.in
tc_manual ../manual_tests/strengthen/anti_vincent_window5_g2.in
tc_manual ../manual_tests/strengthen/anti_vincent_smallwindow_g2.in
tc g2-1 chatgpt_killer n=500 t=500
tc g2-2 gen_rand n=500 l=3 t=500 mode=random
tc g2-3 gen_rand n=500 l=4 t=500 mode=random
tc g2-4 gen_rand n=500 l=5 t=500 mode=random
tc g2-5 gen_rand n=500 l=5 t=500 mode=corr mul=100 add=10
tc g2-6 gen_rand n=500 l=5 t=500 mode=corr mul=10 add=100
tc g2-7 gen_rand n=500 l=5 t=500 mode=corr mul=2 add=100
tc g2-8 gen_rand n=500 l=5 t=500 mode=corr mul=2 add=5
tc g2-9 gen_rand n=500 l=5 t=500 mode=corr mul=2 add=5
tc g2-10 gen_rand n=500 l=5 t=500 mode=stair len=25 scale=2
tc g2-11 gen_rand n=500 l=5 t=500 mode=stair len=50 scale=20
tc g2-12 gen_rand n=500 l=5 t=500 mode=stair len=3 scale=50

group group3 24
limits maxn=5000
include_group group2
tc_manual ../manual_tests/antigreedy/random_fail_g3.in
tc_manual ../manual_tests/antigreedy/anti_vincent.in
tc_manual ../manual_tests/antishuffle/antishuffle_g3.in
tc_manual ../manual_tests/antigreedy/cheese_bait_g3.in
tc_manual ../manual_tests/antigreedy/anti_vincent_rollback_g3.in
tc_manual ../manual_tests/antigreedy/anti_dc_holes_g3.in
tc_manual ../manual_tests/strengthen/anti_heuristic_bb_g3.in
tc g3-1 chatgpt_killer n=5000 t=$MAX_T
tc g3-2 gen_rand n=5000  l=4 t=$MAX_T mode=random
tc g3-3 gen_rand n=5000  l=5 t=$MAX_T mode=random
tc g3-4 gen_rand n=5000  l=5 t=$MAX_T mode=corr mul=100 add=10
tc g3-5 gen_rand n=5000  l=5 t=$MAX_T mode=corr mul=10 add=100
tc g3-6 gen_rand n=5000  l=5 t=$MAX_T mode=corr mul=2 add=100
tc g3-7 gen_rand n=5000  l=5 t=$MAX_T mode=corr mul=2 add=5
tc g3-8 gen_rand n=5000  l=5 t=$MAX_T mode=stair len=25 scale=2
tc g3-9 gen_rand n=5000  l=5 t=$MAX_T mode=stair len=50 scale=20
tc g3-10 gen_rand n=5000 l=5 t=$MAX_T mode=stair len=3 scale=50

group group4 17
limits maxl=2
include_group group1
tc_manual ../manual_tests/antigreedy/21_bait.in
tc_manual ../manual_tests/strengthen/anti_edwin_g4.in
tc g4-1 gen_rand n=200000 l=2 t=$MAX_T mode=random
tc g4-2 gen_rand n=200000 l=2 t=$MAX_T mode=random
tc g4-3 gen_rand n=200000 l=2 t=$MAX_T mode=corr mul=100 add=10
tc g4-4 gen_rand n=200000 l=2 t=$MAX_T mode=corr mul=10 add=100
tc g4-5 gen_rand n=200000 l=2 t=$MAX_T mode=corr mul=2 add=100
tc g4-6 gen_rand n=200000 l=2 t=$MAX_T mode=corr mul=2 add=5
tc g4-7 gen_rand n=200000 l=2 t=$MAX_T mode=stair len=25 scale=2
tc g4-8 gen_rand n=200000 l=2 t=$MAX_T mode=stair len=50 scale=20
tc g4-9 gen_rand n=200000 l=2 t=$MAX_T mode=stair len=3 scale=5

group group5 40
include_group group3
include_group group4
tg_manual ../manual_tests/g5
tg_manual ../manual_tests/anti_vlad
tc_manual ../manual_tests/antishuffle/antishuffle_g5.in
tc_manual ../manual_tests/antigreedy/anti_vincent_rollback_g5.in
tc_manual ../manual_tests/antigreedy/anti_dc_holes_g5.in
tc g5-1 chatgpt_killer n=200000 t=$MAX_T
tc g5-2 gen_rand n=200000 l=3 t=$MAX_T mode=random
tc g5-3 gen_rand n=200000 l=4 t=$MAX_T mode=random
tc g5-4 gen_rand n=200000 l=5 t=$MAX_T mode=random
tc g5-5 gen_rand n=200000 l=5 t=$MAX_T mode=corr mul=100 add=10
tc g5-6 gen_rand n=200000 l=5 t=$MAX_T mode=corr mul=10 add=100
tc g5-7 gen_rand n=200000 l=5 t=$MAX_T mode=corr mul=2 add=100
tc g5-8 gen_rand n=200000 l=5 t=$MAX_T mode=corr mul=2 add=5
tc g5-9 gen_rand n=200000 l=5 t=$MAX_T mode=stair len=25 scale=2
tc g5-10 gen_rand n=200000 l=5 t=$MAX_T mode=stair len=50 scale=20
tc g5-11 gen_rand n=200000 l=5 t=$MAX_T mode=stair len=3 scale=5
tc g5-12 gen_anti_pisinger n=200000 t=800003 l=5

