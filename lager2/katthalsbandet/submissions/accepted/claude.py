
def solve():
    n, a, b = map(int, input().split())
    s = input().strip()

    P = [0] * (n + 1)
    for i in range(n):
        P[i + 1] = P[i] + (1 if s[i] == '(' else -1)
    tot = P[n]

    m_right = [0] * (n + 1)
    m_right[n] = P[n]
    for i in range(n - 1, -1, -1):
        m_right[i] = min(P[i], m_right[i + 1])

    m_left = [0] * (n + 1)
    m_left[0] = P[0]
    for i in range(1, n + 1):
        m_left[i] = min(m_left[i - 1], P[i])

    best = float('inf')
    for c in range(n):
        M = min(m_right[c], tot + m_left[c])
        R = 2 * P[c] + tot - 2 * M

        if m_right[c] <= tot + m_left[c]:
            E_U = tot - m_right[c] + m_left[c]
            V_matched = (c - P[c] + 2 * m_left[c]) // 2
            S = min(E_U, V_matched)
        else:
            S = 0

        cost = b * c + a * R - b * S
        best = min(best, cost)

    print(best)

solve()
