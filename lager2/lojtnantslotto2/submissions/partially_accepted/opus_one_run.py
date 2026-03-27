import sys
import random

def solve():
    data = sys.stdin.buffer.read().split()
    N = int(data[0])
    p = [int(x) - 1 for x in data[1:N + 1]]

    # Determine cycle structure
    visited = [False] * N
    cycle_lengths = []
    for i in range(N):
        if not visited[i]:
            length = 0
            c = i
            while not visited[c]:
                visited[c] = True
                c = p[c]
                length += 1
            cycle_lengths.append(length)

    # NO iff there is exactly one even-length cycle and it has length 2.
    # Equivalently: there is a 2-cycle and no other even-length cycle exists.
    two_cycle_count = sum(1 for L in cycle_lengths if L == 2)
    other_even = any(L % 2 == 0 and L != 2 for L in cycle_lengths)
    if two_cycle_count == 1 and not other_even:
        print("NO")
        return

    # Compute orbits of the map T(i,j) = (j, p[i]) on the N×N grid.
    # Any matrix constant on these orbits satisfies M[i][j] = M[j][p[i]].
    orbit_id = [-1] * (N * N)
    num_orbits = 0
    for start in range(N * N):
        if orbit_id[start] >= 0:
            continue
        ci, cj = divmod(start, N)
        cells = []
        while True:
            idx = ci * N + cj
            if orbit_id[idx] >= 0:
                break
            cells.append(idx)
            orbit_id[idx] = num_orbits
            ci, cj = cj, p[ci]
        num_orbits += 1

    # Each row's pattern is determined by orbit values.
    row_orbs = []
    for i in range(N):
        base = i * N
        row_orbs.append(tuple(orbit_id[base + j] for j in range(N)))

    # Assign random 0/1 to each orbit; rows will be distinct with overwhelming probability.
    random.seed(42)
    for _ in range(1):
        val = [random.getrandbits(1) for _ in range(num_orbits)]
        seen = set()
        ok = True
        for i in range(N):
            row = tuple(val[o] for o in row_orbs[i])
            if row in seen:
                ok = False
                break
            seen.add(row)
        if ok:
            out = []
            for i in range(N):
                out.append(''.join(str(val[o]) for o in row_orbs[i]))
            sys.stdout.write("YES\n" + '\n'.join(out) + '\n')
            return

    print("NO")

solve()