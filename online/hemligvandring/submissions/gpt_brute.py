#!/usr/bin/python3
import random
from collections import deque
import itertools

# --------------------------------------------------
# BFS shortest path on a given grid, from source to target
# grid is list of strings; obstacles are '#'
# returns distance or -1 if unreachable
# --------------------------------------------------
def bfs(grid, src, dst, N):
    (sx, sy) = src
    (tx, ty) = dst
    if grid[sx][sy] == "#" or grid[tx][ty] == "#":
        return -1

    q = deque()
    q.append((sx, sy, 0))
    seen = [[False]*N for _ in range(N)]
    seen[sx][sy] = True
    
    while q:
        x, y, d = q.popleft()
        if (x, y) == (tx, ty):
            return d
        
        for dx, dy in [(1,0),(-1,0),(0,1),(0,-1)]:
            nx, ny = x+dx, y+dy
            if 0 <= nx < N and 0 <= ny < N and not seen[nx][ny]:
                if grid[nx][ny] != "#":
                    seen[nx][ny] = True
                    q.append((nx, ny, d+1))
    return -1

# --------------------------------------------------
# Random query generator:
# Converts '.' to '#' with prob p, keeps 'H'
# --------------------------------------------------
def random_query(original_grid, p=0.1):
    N = len(original_grid)
    new = []
    for i in range(N):
        row = []
        for j in range(N):
            if original_grid[i][j] == 'H':
                row.append('H')
            else:
                row.append('#' if random.random() < p else '.')
        new.append("".join(row))
    return new



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


# --------------------------------------------------
# Main test: generate random 5 queries, check uniqueness
# --------------------------------------------------
def test_random_queries(original_grid, max_attempts=200, p=0.1, K = 5):
    N = len(original_grid)

    # collect house locations
    houses = []
    for i in range(N):
        for j in range(N):
            if original_grid[i][j] == 'H':
                houses.append((i, j))

    # all valid ordered pairs (different row AND column)
    pairs = [(a, b) for a in houses for b in houses
             if a != b and a[0] != b[0] and a[1] != b[1] and a < b]

    print(f"Total ordered valid pairs: {len(pairs)}")

    for attempt in range(1, max_attempts+1):
        print(f"[Attempt {attempt}] generating {K} random queries...")

        queries = [random_query(original_grid, random.random()) for _ in range(K-3)] + [query2(original_grid)] + [query4(original_grid)] + [query1(original_grid)]
        #print(queries)
        # for qq in queries:
        #     print("erm")
        #     for row in qq:
        #         print(row)

        signature_map = {}
        collision = False

        for (src, dst) in pairs:
            sig = tuple(bfs(queries[k], src, dst, N) for k in range(K))
            if sig in signature_map and signature_map[sig] != (src, dst):
                collision = True
                print(src,dst,signature_map[sig])
                break
            signature_map[sig] = (src, dst)

        if not collision:
            print(f"SUCCESS: {K} queries uniquely identify all pairs!")
            return queries
        
        print("Collision found, trying another set...")

    print("FAILED: no unique set found in attempts.")
    return None


# --------------------------------------------------
# Example usage
# --------------------------------------------------
if __name__ == "__main__":
    # Example small grid for testing
    # Replace this with actual input
    N = 75
    original_grid = [
        ".........",
        ".H.H.H.H.",
        ".........",
        ".H.H.H.H.",
        ".........",
        ".H.H.H.H.",
        ".........",
        ".H.H.H.H.",
        "........."
    ]

    g2 = [
        ".....",
        ".H.H.",
        ".....",
        ".H.H.",
        "....."
    ]

    g = [["."]*N for _ in range(N)]
    for i in range(1,N,2):
        for j in range(1,N,2):
            g[i][j] = "H"
    g = ["".join(row) for row in g]

    queries = test_random_queries(g, max_attempts=2000, p=0.5, K=4)

    if queries:
        print("\nFound working 5 queries:")
        for i, q in enumerate(queries):
            print(f"\n=== Query {i+1} ===")
            for row in q:
                print(row)
