#!/usr/bin/env python3
#
# Testing tool for the task Quack.
#
# Usage:
#
#   python3 testing_tool.py [--silent] program... < input.txt
#
# input.txt uses the following format:
#
#   N
#   x1 y1 x2 y2
#
# where the grid size is N times N.
# x1 y1 x2 y2 are the respective positions of the house and the school.
#
#
# For example, if you have a Python solution that you would run using
# "python3 file.py", you could invoke the testing tool with:
#
#   python3 testing_tool.py python3 file.py < input.txt
#
# where input.txt is a file that contains e.g.
#
# 5
# 1 1 3 3
#
# If --silent is passed, the communication output will not be printed.
#
# The tool is provided as-is, and you should feel free to make
# whatever alterations or augmentations you like to it.
# Notably, this is not the program used to test your solution in Kattis
# 

import subprocess
import sys

MAXQUERIES = 365

def error(msg):
    print("ERROR:", msg)
    sys.exit(1)

def main():
    silent = False
    args = sys.argv[1:]
    if args and args[0] == "--silent":
        args = args[1:]
        silent = True
    if not args or args == ["--help"] or args == ["-h"]:
        print("Usage:", sys.argv[0], "[--silent] program... <input.txt")
        sys.exit(0)
    
    try:
        N = int(input())  
    except Exception:
        error("bad input format: failed to parse first line as an integer N.")
    
    try:
        x1, y1, x2, y2 = map(int, input().split())
    except:
        error("bad input format: could not read second line as a list of integers x1 y1 x2 y2")
    

    if (min(x1, y1, x2, y2) < 0) or (max(x1, y1, x2, y2) >= N-1):
        error(f"invalid positions: positions must be in range [0,{N-1}]")
    
    proc = subprocess.Popen(args, stdin=subprocess.PIPE, stdout=subprocess.PIPE)
    if not silent:
        print(f"[*] Running with {N=} \n{x1=}, {y1=}, {x2=}, {y2=}")

    proc.stdin.write(f"{N}\n".encode("utf8"))
    proc.stdin.flush()
    print(f"< {N}")

    for i in range(N):
        row = ["." for j in range(N)]
        for j in range(N):
            if i % 2 == j % 2 == 1:
                row[j] = "H"
        rowstr = "".join(row)
        proc.stdin.write(f"{rowstr}\n".encode("utf8"))
        proc.stdin.flush()
        if not silent:
            print(f"< {rowstr}")

    queries = 0

    while True:
        line = proc.stdout.readline().decode("utf8")
        line = line.strip()
        
        command = line.split()[0]

        if command == "?":

            if len(line.split()) != 1:
                error(f"Program sent guess with wrong format: {line}")
            if not silent:
                print(f"> {line}")

            queries += 1
            if queries > MAXQUERIES:
                error("Wrong answer: too many queries")
            
            grid = []

            for i in range(N):
                line = proc.stdout.readline().decode("utf8")
                line = line.strip()
                if len(line) != N:
                    error(f"bad query format: query row has length {len(line)} but should be {N}")

                for j, c in enumerate(line):
                    if i % 2 == j % 2 == 1:
                        if c != "H":
                            error(f"bad query format: query position ({i},{j}) should be 'H'")
                    else:
                        if c not in (".", "#"):
                            error(f"bad query format: query row contains invalid character {c}")

                grid.append(line)
                if not silent:
                    print(f"> {line}")

            def finddist(x1,y1,x2,y2,grid):
                dist = [[-1]*N for _ in range(N)]

                BFS = [(x1, y1)]
                dist[x1][y1] = 0
                
                for cx,cy in BFS:
                    for dx,dy in [(-1,0),(0,-1),(0,1),(1,0)]:
                        if 0 <= cx+dx < N and 0 <= cy+dy < N:
                            if grid[cx+dx][cy+dy] != "#" and dist[cx+dx][cy+dy] == -1:
                                dist[cx+dx][cy+dy] = dist[cx][cy] + 1
                                BFS.append((cx+dx,cy+dy))

                return dist[x2][y2]

            dist = finddist(x1,y1,x2,y2,grid)

            proc.stdin.write(f"{dist}\n".encode("utf8"))
            proc.stdin.flush()
            if not silent:
                print(f"< {dist}")
            
        elif command == "!":

            if len(line.split()) != 5:
                error(f"Program sent guess with wrong format: {line}")
            
            try:
                guess_x1, guess_y1, guess_x2, guess_y2 = map(int, line.split()[1:])
            except Exception:
                error(f"bad input format: could not read the final answer ({line}).")

            if (guess_x1, guess_y1) == (x1, y1) and (guess_x2, guess_y2) == (x2, y2):
                print(f"[*] OK: found the correct positions in {queries} queries")
                exit(0)

            if (guess_x1, guess_y1) == (x2, y2) and (guess_x2, guess_y2) == (x1, y1):
                print(f"[*] OK: found the correct positions in {queries} queries")
                exit(0)

            if not silent:
                print(f"> {line}")

            print(f"[*] Wrong answer: did not output the correct solution. Guessed {guess_x1}, {guess_y1}, {guess_x2}, {guess_y2}, correct answer is {x1}, {y1}, {x2}, {y2}")
            exit(0)
        else:
            error(f"Unknown command {command}. The full line was: {line}")
        

if __name__ == "__main__":
    main()