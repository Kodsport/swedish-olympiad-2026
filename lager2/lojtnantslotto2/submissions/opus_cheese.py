#!/usr/bin/python3
import sys
import random

def solve():
    data = sys.stdin.buffer.read().split()
    N = int(data[0])
    p = [int(x)-1 for x in data[1:N+1]]
    
    # Compute orbits of T(i,j) = (j, p[i])
    # M constant on T-orbits => M[i][j] = M[j][p[i]] satisfied
    
    orbit_id = [-1] * (N * N)
    num_orbits = 0
    
    for start in range(N * N):
        if orbit_id[start] >= 0:
            continue
        # Trace orbit
        ci = start // N
        cj = start % N
        cells = []
        while True:
            idx = ci * N + cj
            if orbit_id[idx] >= 0:
                break
            cells.append(idx)
            orbit_id[idx] = num_orbits
            ci, cj = cj, p[ci]
        num_orbits += 1
    
    # Check if orbit-signature per row is unique
    sigs = set()
    dup = False
    row_orbs = []
    for i in range(N):
        sig = tuple(orbit_id[i*N+j] for j in range(N))
        row_orbs.append(sig)
        if sig in sigs:
            dup = True
        sigs.add(sig)
    
    if dup:
        print("NO")
        return
    
    # Find val[0..num_orbits-1] in {0,1} so all rows distinct
    random.seed(42)
    
    tot = 0
    for attempt in range(500):
        val = [random.getrandbits(1) for _ in range(num_orbits)]
        seen = set()
        ok = True
        for i in range(N):
            tot += 1
            row = tuple(val[o] for o in row_orbs[i])
            if row in seen:
                ok = False
                break
            seen.add(row)
        if ok:
            out = []
            for i in range(N):
                out.append(''.join(str(val[o]) for o in row_orbs[i]))
            print("YES")
            #print(attempt)
            print('\n'.join(out))
            return
    
    print("NO")
    #print(tot)

solve()