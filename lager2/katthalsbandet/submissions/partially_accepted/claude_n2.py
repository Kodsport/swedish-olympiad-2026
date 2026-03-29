def solve():
    n, a, b = map(int, input().split())
    s = input()

    best = float('inf')
    for c in range(n):
        # Rotated string: s[c..n-1] (unshifted, cost a) + s[0..c-1] (shifted, cost a-b)
        # Standard left-to-right stack matching, tracking U vs V
        stack = []  # 'U' or 'V'
        balance = 0
        rd_u = 0
        rd_v = 0

        for i in range(c, n):
            if s[i] == '(':
                stack.append('U')
                balance += 1
            else:
                if balance > 0:
                    stack.pop()
                    balance -= 1
                else:
                    rd_u += 1

        for i in range(c):
            if s[i] == '(':
                stack.append('V')
                balance += 1
            else:
                if balance > 0:
                    stack.pop()
                    balance -= 1
                else:
                    rd_v += 1

        e_u = sum(1 for x in stack if x == 'U')
        e_v = sum(1 for x in stack if x == 'V')
        v_open = sum(1 for i in range(c) if s[i] == '(')

        R = rd_u + rd_v + e_u + e_v
        S = min(e_u, v_open - e_v)
        cost = b * c + a * R - b * S
        best = min(best, cost)

    print(best)

solve()
