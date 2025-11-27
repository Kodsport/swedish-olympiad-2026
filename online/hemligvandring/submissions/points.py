#!/usr/bin/python3
def g(Q):
    if Q <= 5:
        return 75
    elif Q <= 6:
        return 63
    elif Q <= 10:
        return 55 * (10/55)**((Q - 10) / 85)
    elif Q <= 75:
        return 53 * (8/53)**((Q - 10) / 64)
    elif Q <= 365:
        return 4 * (1/4)**((Q - 160) / 205)
    else:
        return 0


data = [(Q, round(g(Q))) for Q in range(0, 366)]
header_q = "Number of queries"
header_p = "Points"
qw = max(len(str(q)) for q, _ in data)
pw = max(len(str(p)) for _, p in data)
qw = max(qw, len(header_q))
pw = max(pw, len(header_p))

sep = f"+-{'-' * qw}-+-{'-' * pw}-+"
print(sep)
print(f"| {header_q.ljust(qw)} | {header_p.ljust(pw)} |")
print(sep)
for q, p in data:
    print(f"| {str(q).rjust(qw)} | {str(p).rjust(pw)} |")
print(sep)