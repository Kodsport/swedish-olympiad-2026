#!/usr/bin/python3
import random
from collections import deque, defaultdict

# --------------------------------------------------
# BFS from a single source → returns full distance map
# --------------------------------------------------
def bfs_from_source(grid, sx, sy, N):
    dist = [[-1]*N for _ in range(N)]
    if grid[sx][sy] == '#':
        return dist

    q = deque([(sx, sy)])
    dist[sx][sy] = 0

    while q:
        x, y = q.popleft()
        d = dist[x][y]
        for dx, dy in ((1,0),(-1,0),(0,1),(0,-1)):
            nx, ny = x+dx, y+dy
            if 0 <= nx < N and 0 <= ny < N:
                if grid[nx][ny] != "#" and dist[nx][ny] == -1:
                    dist[nx][ny] = d+1
                    q.append((nx,ny))
    return dist


# --------------------------------------------------
# Random query generator
# --------------------------------------------------
def random_query(original, p=0.10):
    N = len(original)
    out = []
    for i in range(N):
        row = []
        for j in range(N):
            if original[i][j] == 'H':
                row.append('H')
            else:
                row.append('#' if random.random() < p else '.')
        out.append("".join(row))
    return out

def query1(A):
    return [row[:] for row in A]

def query2(A):
    B = [[*row] for row in A]
    for i in range(len(A)-1):
        for j in range(0,len(A),2):
            B[i][j] = "#"
    return ["".join(row) for row in B]

    

def query3(A):

    B = [[*row] for row in A]
    n = len(A)
    for i in range(1,n):
        for j in range(0,n,2):
            B[i][j] = "#"

    return ["".join(row) for row in B]


def query4(A):


    n = len(A)
    B = [[*row] for row in A]
    for i in range(0,n,2):
        for j in range(1,n):
            B[i][j] = "#"

    return ["".join(row) for row in B]


def queryT1(A):
    n = len(A)
    B = [[*row] for row in A]
    for i in range(0,n,2):
        if i%4:
            B[i] = ["#"]*n
            B[i][-1] = "."
            #B[i][-2] = "."
        else:
            B[i] = ["#"]*n
            B[i][0] = "."
            #B[i][1] = "."
    return ["".join(row) for row in B]

def queryT2(A):
    n = len(A)
    B = [[*row] for row in A]
    for i in range(0,n,2):
        if i%4:
            for j in range(n-1):
                B[j][i] = "#"
            
        else:
            for j in range(1,n):
                B[j][i] = "#"

    return ["".join(row) for row in B]

# --------------------------------------------------
# Evaluate a query using O(N^4):
#   - Precompute BFS from every house
#   - Bucket candidates by resulting distance
# Returns: bucket (distance -> list of candidate pairs)
# --------------------------------------------------
def evaluate_query(query, candidates, houses, N):
    # Precompute BFS for each house
    all_dist = {}
    for (hx, hy) in houses:
        all_dist[(hx, hy)] = bfs_from_source(query, hx, hy, N)

    # Bucket candidate answers by observed distance
    bucket = defaultdict(list)
    for (src, dst) in candidates:
        d = all_dist[src][dst[0]][dst[1]]
        bucket[d].append((src, dst))
    #bucket = {36:bucket[36], 578:bucket[578]}
    return bucket


# --------------------------------------------------
# Pick the query that minimizes the largest bucket (worst-case)
# --------------------------------------------------
def choose_best_query(roundid, original_grid, candidates, houses, trials=40, p=0.10):
    N = len(original_grid)
    best_query = None
    best_max_bucket = len(candidates)  # we want to minimize this
    best_bucket = None

    for i in range(trials):
        if roundid == 1:
            Q = queryT1(original_grid)
        elif roundid == 2:
            Q = queryT2(original_grid)
        # elif roundid == 2:
        #     Q = query3(original_grid)
        # elif roundid == 3:
        #     Q = query4(original_grid)
        else:
            Q = random_query(original_grid, p)
        bucket = evaluate_query(Q, candidates, houses, N)

        # compute largest bucket size (worst-case remaining after adversary answer)
        max_size = max(len(v) for v in bucket.values())

        # minimize the worst-case size
        if max_size < best_max_bucket:
            best_max_bucket = max_size
            best_query = Q
            best_bucket = bucket

    return best_query, best_bucket


# --------------------------------------------------
# MAIN LOOP: repeatedly choose best query under worst-case responder
# --------------------------------------------------
def eliminate_until_one(original_grid, wall_prob=0.10, trials_per_round=40, verbose=True):
    N = len(original_grid)

    # Collect houses
    houses = [(i, j)
              for i in range(N)
              for j in range(N)
              if original_grid[i][j] == 'H']

    # Candidates (unordered pairs with row!= and col!=)
    candidates = [(a, b) for a in houses for b in houses
                  if a < b and a[0] != b[0] and a[1] != b[1]]

    if verbose:
        print(f"Initial candidate count: {len(candidates)}")

    round_id = 0
    queries_used = []

    while len(candidates) > 1:
        round_id += 1
        if verbose:
            print(f"\n=== ROUND {round_id} ===")
            print(f"Candidates remaining: {len(candidates)}")
            print("Searching for best query...")
            if len(candidates) < 25:
                print(candidates)
        best_query, bucket = choose_best_query(
            round_id,
            original_grid,
            candidates,
            houses,
            trials=trials_per_round,
            p=wall_prob
        )

        if best_query is None:
            # fallback: if we couldn't find better than current, pick random
            best_query = random_query(original_grid, wall_prob)
            bucket = evaluate_query(best_query, candidates, houses, N)

        # Adversary returns the distance corresponding to the largest bucket
        best_d = max(bucket.keys(), key=lambda d: len(bucket[d]))
        new_candidates = bucket[best_d]

        eliminated = len(candidates) - len(new_candidates)
        if verbose:
            print(f"Worst-case bucket size after chosen query: {len(new_candidates)}")
            print(f"Best query (by trials) eliminated at least {eliminated} candidates")

        queries_used.append(best_query)
        candidates = new_candidates

    if verbose:
        print("\n=== FINISHED ===")
        print("Unique answer:", candidates[0])
    return queries_used, candidates[0]


# --------------------------------------------------
# Example for testing
# --------------------------------------------------
if __name__ == "__main__":
    N = 35
       # smaller N in example to run quickly; set to 75 if you want
    grid = [["."]*N for _ in range(N)]
    for i in range(1, N, 2):
        for j in range(1, N, 2):
            grid[i][j] = "H"
    g = ["".join(row) for row in grid]

    print(queryT1(g))
    print(queryT2(g))

    queries, answer = eliminate_until_one(
        g,
        wall_prob=0.52,
        trials_per_round=10,
        verbose=True
    )

    print("\nTotal queries used:", len(queries))
