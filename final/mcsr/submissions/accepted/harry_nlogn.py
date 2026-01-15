# https://github.com/cheran-senthil/PyRival/blob/master/pyrival/graphs/dijkstra.py

from heapq import heappop, heappush

def dijkstra(graph, start):
    """ 
        Uses Dijkstra's algortihm to find the shortest path from node start
        to all other nodes in a directed weighted graph.
    """
    n = len(graph)
    dist, parents = [float("inf")] * n, [-1] * n
    dist[start] = 0

    queue = [(0, start)]
    while queue:
        path_len, v = heappop(queue)
        if path_len == dist[v]:
            for w, edge_len in graph[v]:
                if edge_len + path_len < dist[w]:
                    dist[w], parents[w] = edge_len + path_len, v
                    heappush(queue, (edge_len + path_len, w))

    return dist, parents

n,s = map(int,input().split())
pos = []
tim = []

for _ in range(n):
    x,t = map(int,input().split())
    pos.append(x)
    tim.append(t)

# 0, x1*8, ..., xn*8, S
# x1, x2, ...

start = 2*n
end = 2*n+1
# Overworld portal: i
# Nether portal: i + n
graph = [[] for _ in range(2*n+2)]

graph[start].append((0,pos[0]*8))
graph[n-1].append((end,s-pos[n-1]*8))

for i in range(n-1):
    graph[i].append((i+1, 8*(pos[i+1]-pos[i])))
    graph[i+n].append((i+1+n, pos[i+1]-pos[i]))

for i in range(n):
    graph[i].append((i+n, tim[i]))
    graph[i+n].append((i, tim[i]))

dist,_ = dijkstra(graph, start)

print(dist[end])
