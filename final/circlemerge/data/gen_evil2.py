# worst_case_divs.py
S = 6983776800
unit = 200000

full = S // unit            # number of 100000's
rem  = S - full * unit      # remainder <= 100000
if rem == 0:
    N = full
    arr = [unit] * full
else:
    N = full + 1
    arr = [unit] * full + [rem]

# Sanity: constraints satisfied
assert N <= 200000
#assert all(1 <= x <= 200000 for x in arr)
assert sum(arr) == S

print(N)
print(" ".join(map(str, arr)))
