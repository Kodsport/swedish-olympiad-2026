#!/usr/bin/env python3
# requires Python 3.8+
from collections import deque, defaultdict
import itertools
import math
from itertools import product
import random
import subprocess
import tempfile
import os

# ---------- buggy solver (wrap-around BFS in n*D x m*D) ----------
def buggy_answer(grid, R, C, D, queries):
    """
    Calls ../submissions/z3_chatgpt.py as an external process.
    grid: list of strings
    queries: list of (j1,i1,j2,i2)
    Returns a list of answers corresponding to the queries.
    """

    # Create temporary input file
    with tempfile.NamedTemporaryFile("w", delete=False) as f:
        tmpname = f.name
        
        # Write input in EXACT format your solver expects
        f.write(f"{R} {C}\n")
        for row in reversed(grid):
            f.write(row + "\n")
        for (j1,i1,j2,i2) in queries:
            f.write(f"{j1} {i1} {j2} {i2}\n")

    with open(tmpname, "r") as infile:
        completed = subprocess.run(
            [
                "./cheese.out",
                str(D)
            ],
            stdin=infile,
            stdout=subprocess.PIPE,
            stderr=subprocess.PIPE,
            text=True
        )

    # Split lines, strip whitespace, ignore empty lines
    output = [line.strip() for line in completed.stdout.splitlines() if line.strip()]
    return output
# ---------- correct solver (torus + translation lattice) ----------

def correct_answer_via_subprocess(grid, R, C, queries):
    """
    Calls ../submissions/z3_chatgpt.py as an external process.
    grid: list of strings
    queries: list of (j1,i1,j2,i2)
    Returns a list of answers corresponding to the queries.
    """

    # Create temporary input file
    with tempfile.NamedTemporaryFile("w", delete=False) as f:
        tmpname = f.name
        
        # Write input in EXACT format your solver expects
        f.write(f"{R} {C}\n")
        for row in reversed(grid):
            f.write(row + "\n")
        for (j1,i1,j2,i2) in queries:
            f.write(f"{j1} {i1} {j2} {i2}\n")

    with open(tmpname, "r") as infile:
        completed = subprocess.run(
            ["python3", "../submissions/z3_chatgpt.py"],
            stdin=infile,
            stdout=subprocess.PIPE,
            stderr=subprocess.PIPE,
            text=True
        )

    os.remove(tmpname)

    # Split lines, strip whitespace, ignore empty lines
    output = [line.strip() for line in completed.stdout.splitlines() if line.strip()]
    return output


# ---------- search driver ----------
def find_bad_D_for_patterns(R, C, max_patterns=10, maxD=200, num_queries=5):
    """
    Automatically searches for patterns that cause buggy BFS to disagree with correct solver.
    Returns a list of (grid, queries, D_list) that trigger false positives.
    """
    results = []

    def generate_random_grid(R, C):
        # Random grid: '.' or '#' with 50% chance
        return [''.join(random.choice(".#") for _ in range(C)) for _ in range(R)]
    
    def generate_random_queries(grid, R, C, q=5, max_coord=500):
        """
        Generate q queries such that both endpoints are unblocked.
        grid: list of strings, base pattern
        """
        queries = []
        for _ in range(q):
            while True:
                i1 = random.randint(0, max_coord)
                j1 = random.randint(0, max_coord)
                i2 = random.randint(0, max_coord)
                j2 = random.randint(0, max_coord)
                # check that both endpoints are unblocked in the base pattern
                if grid[i1 % R][j1 % C] != '#' and grid[i2 % R][j2 % C] != '#':
                    queries.append((j1, i1, j2, i2))
                    break
        return queries


    pattern_count = 0
    alive_D = [i for i in range(1, maxD+1)]
    killed_D = []
    # [1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 24, 26, 28, 30, 32, 33, 34, 35, 36, 38, 39, 40, 42, 44, 45, 48, 51, 52, 55, 56, 57, 60, 63, 64, 65, 66, 68, 70, 72, 76, 77, 78, 80, 84, 85, 88, 90, 91, 95, 96, 99, 102, 104, 105, 110, 112, 114, 117, 119, 120, 126, 130, 132, 133, 136, 140, 143, 144, 152, 153, 154, 156, 160, 165, 168, 170, 171, 176, 180, 182, 187, 190, 192, 195, 198, 204, 208, 209, 210, 220, 221, 224, 228, 231, 234, 238, 240, 247, 252, 255, 260, 264, 266, 272, 273, 280, 285, 286, 304, 306, 308, 312, 315, 320, 323, 330, 336, 340, 342, 352, 357, 360, 364, 374, 380, 385, 390, 396, 399, 408, 416, 418, 420, 429, 440, 442, 448, 455, 456, 462, 468, 476, 480, 494, 495, 504, 510, 520, 528, 532, 544, 546, 560, 561, 570, 572, 585, 595, 608, 612, 616, 624, 627, 630, 646, 660, 663, 665, 672, 680, 684, 693, 704, 714, 715, 720, 728, 741, 748, 760, 765, 770, 780, 792, 798, 816, 819, 832, 836, 840, 855, 858, 880, 884, 910, 912, 924, 935, 936, 952, 960, 969, 988, 990
    #             ]+\
    #             [23, 25, 27, 29, 31, 46, 50, 58, 62, 69, 75, 87, 92, 93, 100, 115, 116, 124, 135, 138, 145, 150, 155, 174, 186, 207, 225, 230, 261, 276, 279, 290, 300, 310, 345, 348, 372, 414, 435, 450, 460, 465, 522, 558, 575, 580, 620, 621, 667, 675, 690, 713, 725, 775, 783, 828, 837, 870, 899, 900, 930
    #              ]+\
    #              [37, 41, 43, 47, 49, 259, 287, 301, 329]+\
    #              [53, 54, 59, 61, 106, 118, 122, 159, 177, 183, 212, 236, 244, 318, 354, 366, 424, 472, 477, 488, 531, 549, 636, 708, 732, 954]


    alive_D = [d for d in alive_D if d not in killed_D]

    import math

    def lcm(a, b):
        return abs(a*b) // math.gcd(a, b)

    current = 1
    for v in alive_D:
        nxt = lcm(current, v)
        if nxt * 4 > 10**9:
            break
        current = nxt
        print(f"{v}: {current} {math.log10(current)}")
    print(f"Max D to test: {current}")

    while len(alive_D):
        R = random.randint(1, 3)
        C = random.randint(1, 3)
        grid = generate_random_grid(R,C)
        if all(grid[r] == '#'*C for r in range(R)):
            continue  # skip fully blocked grids

        queries = generate_random_queries(grid, R,C)
        grid="""##
#.""".splitlines()
        R=2
        C=2
        print(f"Testing grid")
        print("\n".join(grid))

        D=1000
        queries_str = f"""1 1 {1 + 3*D} 1
1 1 {1 + 3*D} 1
1 1 {1 + 3*D} 1
1 1 {1 + 3*D} 1
1 1 1 {1 + 3*D}"""
        queries_str = f"""1 1 2958136 1
1 1 2904766 1
1 1 2878261 1
1 1 2831437 1
1 1 2790781 1"""
        queries = []
        for line in queries_str.splitlines():
            j1,i1,j2,i2 = map(int, line.split())
            queries.append((j1,i1,j2,i2))
        print("Queries:")
        print(queries_str)
        for q in queries:
            j1,i1,j2,i2 = q
            if grid[i1 % R][j1 % C] == '#' or grid[i2 % R][j2 % C] == '#':
                print("BAD QUERY GENERATED!")
                print(f"{j1} {i1} {j2} {i2}")

        bad_D = []
        num_buggy = 0
        for D in alive_D:
            bug = buggy_answer(grid,R,C,D,queries)
            corr = correct_answer_via_subprocess(grid,R,C,queries)
            if bug != corr:
                num_buggy += 1
                bad_D.append(D)
                killed_D.append(D)
            print(f"Testing pattern #{pattern_count+1}, D={D}...: percent buggy: {num_buggy / D}", end='\r')
        if bad_D:
            results.append((grid, queries, bad_D))
            print(f"Found bad pattern #{pattern_count+1} with {len(bad_D)} D values: {bad_D}")
            pattern_count += 1
        alive_D = [d for d in alive_D if d not in killed_D]
    return results

if __name__ == "__main__":
    # Example: search for R=1, C=3 patterns
    R, C = 2, 2
    found = find_bad_D_for_patterns(R, C, max_patterns=1, maxD=1000)
    
    # Print results in judge-friendly format
    for idx, (grid, queries, badD) in enumerate(found):
        print(f"\n=== Pattern #{idx+1} ===")
        print(f"{R} {C}")
        for row in grid:
            print(row)
        for q in queries:
            print(' '.join(map(str, q)))
        print("Bad D values:", badD)