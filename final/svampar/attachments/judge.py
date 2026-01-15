
import sys

if len(sys.argv) != 2:
    print("Usage: python3 judge.py <input_file> < output")
    exit(1)

with open(sys.argv[1], 'r') as f:   
    n = int(f.readline().strip())
    arr = list(map(int, f.readline().strip().split()))

if len(arr) != n:
    print("Wrong length of array")
    exit(1)

if any(x < 0 or x > 255 for x in arr):
    print("Array elements must be between 0 and 255")
    exit(1)

rounds = int(input())
curr_arr = [0] * n
for round in range(rounds):
    new_arr = [0] * n
    def parse_line():
        i = 0
        ops = []
        tokens = input().split()
        while i < len(tokens):
            token = tokens[i]
            if token == 'p':
                ops.append((token, None))
                i += 1
            else:
                if i + 1 >= len(tokens):
                    print("Invalid operation format")
                    exit(1)
                idx = tokens[i + 1]
                if not idx.isdigit():
                    print("Index must be an integer")
                    exit(1)
                idx = int(idx)
                if idx < 1 or idx > n:
                    print(f"Index out of bounds: {idx}")
                    exit(1)
                ops.append((token, idx - 1))
                i += 2
        if len(ops) != n:
            print("Number of operations does not match n")
            exit(1)
        return ops
    
    ops = parse_line()

    for i in range(n):
        op, idx = ops[i]
        if op == "p":
            new_arr[i] = curr_arr[i]
        else:
            if op == '+':
                new_arr[i] = curr_arr[idx] + 1
            elif op == '<':
                new_arr[i] = curr_arr[idx] << 1
            elif op == '>':
                new_arr[i] = curr_arr[idx] >> 1
            elif op == '^':
                new_arr[i] = curr_arr[i] ^ curr_arr[idx]
            elif op == '&':
                new_arr[i] = curr_arr[i] & curr_arr[idx]
            elif op == '|':
                new_arr[i] = curr_arr[i] | curr_arr[idx]
            else:
                print(f"Unknown operation: {op}")
                exit(1)
    curr_arr = new_arr

if curr_arr == arr:
    print("Correct")
    exit(0)
else:
    print("Incorrect")
    print(f"Target array: {arr}")
    print(f"Final array: {curr_arr}")
    diff_pos = [i for i in range(n) if arr[i] != curr_arr[i]]
    if len(diff_pos) < 30:
        print(curr_arr[256], arr[256])
        print(f"Positions with differences: {diff_pos} (0-indexed)")
    exit(1)
