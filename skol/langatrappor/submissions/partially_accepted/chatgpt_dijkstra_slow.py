import heapq

def main():
    r, c = map(int, input().split())

    start_val = list(map(int, input().split()))

    s2, s1 = map(int, input().split())
    g2, g1 = map(int, input().split())
    start = (s1, s2)
    goal  = (g1, g2)

    def cost_at(p):
        return start_val[p[1]] - p[0]

    pq = []
    heapq.heappush(pq, (-cost_at(start), start))

    vis = set()
    dirs = [(0,1),(0,-1),(1,0),(-1,0)]

    while pq:
        d, p = heapq.heappop(pq)

        if p in vis:
            continue
        vis.add(p)

        if p == goal:
            print(-d)
            return

        for dy, dx in dirs:
            np = (p[0] + dy, p[1] + dx)
            if np[0] < 0 or np[1] < 0 or np[0] >= r or np[1] >= c:
                continue
            heapq.heappush(pq, (d - cost_at(np), np))

    assert False

if __name__ == "__main__":
    main()
