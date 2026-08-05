#!/bin/bash
. ../../../testdata_tools/gen.sh

use_solution nils.py

compile gen_rand.py
compile gen_pair.py
compile gen_rect.py

samplegroup
sample 1
sample 2
sample 3

group group1 31
limits maxn=1
tc 2
tc_manual ../skolkval_tests/skol_1.in
tc line-1 gen_rand n=1 m=8 p=2
# smallest legal row: the square is squeezed between two adjacent-but-one farms
tc g1-tight gen_pair n=1 m=3 r1=0 c1=0 r2=0 c2=2
# widest possible separation, with slack at the right edge so the grid is not
# the bounding box of the farms
tc g1-span gen_pair n=1 m=10 r1=0 c1=0 r2=0 c2=8
# odd width, farm on the very last column, whole left half of the row empty
tc g1-right gen_pair n=1 m=9 r1=0 c1=6 r2=0 c2=8

group group2 21
limits rectangle=1
tc_manual ../skolkval_tests/skol_2.in
# smallest legal grid; the rectangle is the entire grid
tc g2-min gen_rect n=3 m=3 r1=0 r2=2 c1=0 c2=2
# extreme aspect ratio (2 x 8) in a non-square grid
tc g2-flat gen_rect n=3 m=10 r1=0 r2=2 c1=0 c2=8
# rectangle pushed into a corner, slack on the other two sides
tc g2-corner gen_rect n=10 m=10 r1=5 r2=9 c1=1 c2=7

group group3 48
include_group group1
include_group group2
tc 1
tc 3
tc_manual ../skolkval_tests/skol_3.in
tc_manual ../skolkval_tests/skol_4.in
tc_manual ../skolkval_tests/skol_5.in
tc g2-1 gen_rand n=10 m=10 p=2
tc g2-2 gen_rand n=10 m=10 p=2
tc g2-3 gen_rand n=10 m=10 p=2
tc g2-4 gen_rand n=10 m=10 p=3
tc g2-5 gen_rand n=10 m=10 p=3
tc g2-6 gen_rand n=10 m=10 p=3
tc g2-7 gen_rand n=2 m=10 p=4
tc g2-8 gen_rand n=10 m=10 p=5
tc g2-9 gen_rand n=10 m=10 p=6
tc g2-10 gen_rand n=10 m=10 p=10
tc g2-11 gen_rand n=10 m=10 p=40
tc g2-12 gen_rand n=10 m=10 p=40
# 82 of the 100 cells are town squares: the answer is two components, 80 of the
# cells lie outside the bounding box of the farms, and two are grid corners
tc g3-blob gen_pair n=10 m=10 r1=0 c1=1 r2=1 c2=0
# the opposite extreme: the answer is the main diagonal, i.e. 10 town squares
# that are pairwise non-adjacent (10 connected components)
tc g3-diag gen_pair n=10 m=10 r1=0 c1=9 r2=9 c2=0
