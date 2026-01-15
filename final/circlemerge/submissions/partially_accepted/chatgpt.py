import sys
from collections import defaultdict
import bisect

def min_moves(a):
    N = len(a)
    S = sum(a)
    # Snabbfall: alla redan lika
    if all(x == a[0] for x in a):
        return 0

    # Prefix-summor för dubbel-array (för att hantera cirkeln)
    P = [0] * (2 * N + 1)
    for i in range(2 * N):
        P[i+1] = P[i] + a[i % N]

    # Kartlägg var varje prefixsumma förekommer (kan förekomma flera gånger om nollor finns)
    pos = defaultdict(list)
    for idx, val in enumerate(P):
        pos[val].append(idx)

    # Enumerera möjliga k (antal lika block) — dvs divisorer av S som <= N
    ks = []
    if S == 0:
        # Om S==0 och alla är 0 så kan vi dela i N delar; annars (omedelbart) om alla 0 returneras ovan.
        # Här lägger vi till k = N som möjlighet.
        ks = [N]
    else:
        # Hitta divisorer av S
        i = 1
        while i * i <= S:
            if S % i == 0:
                if i <= N:
                    ks.append(i)
                d = S // i
                if d != i and d <= N:
                    ks.append(d)
            i += 1

    ks.sort(reverse=True)  # testa största k först (minimera antal drag)

    for k in ks:
        target = 0 if k == 0 else S // k  # om S==0 hanteras tidigare
        # för varje möjlig startposition s (prefix-index s)
        ok_for_some_start = False
        for s in range(0, N):
            prev = s
            good = True
            # vi behöver hitta k-1 "inre" delningspunkter (slutet på block 1..k-1)
            for j in range(1, k):
                need = P[s] + j * target
                lst = pos.get(need)
                if not lst:
                    good = False
                    break
                # hitta första förekomst > prev
                idx = bisect.bisect_right(lst, prev)
                if idx == len(lst) or lst[idx] > s + N:
                    good = False
                    break
                prev = lst[idx]
            if good:
                # sista delningen hamnar exakt vid s+N (eftersom total = k*target)
                # (det finns implicit P[s+N] == P[s]+S)
                ok_for_some_start = True
                break
        if ok_for_some_start:
            return N - k

    # Om inget annat funkar så kan vi alltid slå ihop allt till 1 tal
    return N - 1

if __name__ == "__main__":
    data = sys.stdin.read().strip().split()
    if not data:
        sys.exit(0)
    it = iter(data)
    N = int(next(it))
    a = [int(next(it)) for _ in range(N)]
    print(min_moves(a))
