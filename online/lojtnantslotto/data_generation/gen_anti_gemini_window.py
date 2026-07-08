#!/usr/bin/python3
# Worst case for gemini_window.cpp.
#
# gemini_window's DP is hard-capped by its W=5 window (value range <= 150), so it
# is a pure O(N log N) IO+sort solution. Its time is dominated by two std::sort
# calls (introsort): the per-class sort(group[i]) and the sort(all_items) with the
# expensive cross-multiply comparator a.g*b.l < b.g*a.l. We attack both:
#
#  1) Per-class sorts: group[i] is filled in INPUT ORDER, which we control, so we
#     plant a McIlroy "Killer Adversary for Quicksort" permutation that forces
#     introsort to ~3 N log N comparisons (heapsort fallback) instead of ~1.2.
#
#  2) all_items sort: after the per-class sort each class is a sorted run, so
#     all_items is a concatenation of sorted runs. Using exactly TWO classes
#     (values 4 and 5) of equal size with the SAME ratio range makes all_items
#     two long overlapping sorted runs -- the classic median-of-3 worst case,
#     ~15-18ms vs ~10ms for 5 random classes.
#
# Net: ~30% slower than a maximal random test (the existing g5 random cases).
#
# The McIlroy permutation is specific to libstdc++'s std::sort, so we generate it
# by compiling and running a tiny C++ adversary against the real std::sort.

import sys, os, subprocess, tempfile

def arg(name, default=None):
    for a in sys.argv:
        if a.startswith(name + "="):
            return a.split("=")[1]
    if default is None:
        print("missing", name); sys.exit(1)
    return default

N = int(arg('n', '200000'))
T = int(arg('t', '1000000'))
m = N // 2                      # two equal classes

ADV = r'''
#include <bits/stdc++.h>
using namespace std;
int N; vector<int> val; int gas,nsolid,cand;
int main(int argc,char**argv){
    N=atoi(argv[1]); gas=N+1; nsolid=0; cand=0; val.assign(N,gas);
    vector<int> a(N); for(int i=0;i<N;i++) a[i]=i;
    sort(a.begin(),a.end(),[&](const int&x,const int&y){
        if(val[x]==gas&&val[y]==gas){ if(x==cand) val[x]=nsolid++; else val[y]=nsolid++; }
        if(val[x]==gas){cand=x;return false;}
        if(val[y]==gas){cand=y;return true;}
        return val[x]<val[y]; });
    for(int i=0;i<N;i++) if(val[i]==gas) val[i]=nsolid++;
    for(int i=0;i<N;i++) printf("%d\n", val[i]);  // killer key per input position
    return 0;
}
'''

with tempfile.TemporaryDirectory() as d:
    src = os.path.join(d, "adv.cpp"); exe = os.path.join(d, "adv")
    open(src, "w").write(ADV)
    subprocess.run(["g++", "-O2", "-o", exe, src], check=True)
    keys = [int(x) for x in subprocess.run([exe, str(m)],
            capture_output=True, text=True, check=True).stdout.split()]

# Emit classes interleaved so each class's input subsequence is the killer order.
# ratio = 2*(key+1) is monotonic in the killer key (preserves the per-class sort
# killer) and shared by both classes (worst-case 2-run all_items merge); g=v*ratio
# stays <= 1e6 and uses 7 digits (slightly heavier parsing too).
out = [f"{N} {T}"]
for j in range(m):
    r = 2 * (keys[j] + 1)
    out.append(f"4 {4 * r}")
    out.append(f"5 {5 * r}")
sys.stdout.write("\n".join(out) + "\n")
