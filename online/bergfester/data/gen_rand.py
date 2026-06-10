#!/usr/bin/python3

import sys
import random
from random import randint

def cmdlinearg(name, default=None):
    for arg in sys.argv:
        if arg.startswith(name + "="):
            return arg.split("=")[1]
    if default is None:
        print("missing parameter", name)
        sys.exit(1)
    return default


def shuffle_tree_labels(parents):
    n = len(parents)

    old_nodes = list(range(n))
    
    perm = old_nodes[:]  
    random.shuffle(perm)
    
    perm_inv = [0] * n
    for old, new in enumerate(perm):
        perm_inv[new] = old

    new_par = [0] * n
    for new_child in range(n):
        old_child = perm_inv[new_child]
        old_parent = parents[old_child]
        new_parent = perm[old_parent]
        new_par[new_child] = new_parent

    return new_par


random.seed(int(cmdlinearg('seed', sys.argv[-1])))
n = int(cmdlinearg('n'))
m = int(cmdlinearg('m'))
q = int(cmdlinearg('q'))
assert m <= n
print(n,m,q)

## GENERATE TREE
treemode=cmdlinearg('treemode')
querymode=cmdlinearg('querymode')

if treemode=='errichto':
    lo = float(cmdlinearg("lo"))
    hi = float(cmdlinearg("hi"))

    parents = [0]

    for i in range(1,n):
        par = randint(max(0, int(i*lo)), max(0, min(i-1,int(i*hi))))
        parents.append(par)
elif treemode=='prufer':
    import heapq
    if n == 1:
        parents = [0]   # single node is root of itself
    else:
        # generate a random Prufer sequence (values in 0..n-1)
        prufer = [randint(0, n-1) for _ in range(max(0, n-2))]

        # degrees: start with 1 for each node, then add count from prufer
        deg = [1] * n
        for v in prufer:
            deg[v] += 1

        # min-heap of leaves (nodes with degree == 1)
        leaves = [i for i in range(n) if deg[i] == 1]
        heapq.heapify(leaves)

        # build undirected edge list from prufer
        edges = []  # list of (u, v) edges
        for v in prufer:
            leaf = heapq.heappop(leaves)
            edges.append((leaf, v))
            deg[leaf] -= 1
            deg[v] -= 1
            if deg[v] == 1:
                heapq.heappush(leaves, v)

        # two remaining nodes
        if len(leaves) >= 2:
            a = heapq.heappop(leaves)
            b = heapq.heappop(leaves)
            edges.append((a, b))
        elif len(leaves) == 1:
            # should not happen for n>=2 with correct prufer algorithm, but be safe
            a = heapq.heappop(leaves)
            # find any other node to connect
            for j in range(n):
                if j != a and deg[j] > 0:
                    edges.append((a, j))
                    break

        # convert edges to adjacency
        adj = [[] for _ in range(n)]
        for u, v in edges:
            adj[u].append(v)
            adj[v].append(u)

        # choose a root (random) and create parent array by BFS
        root = randint(0,n-1)
        parents = [-1] * n
        parents[root] = root
        qu = [root]
        qi = 0
        while qi < len(qu):
            u = qu[qi]; qi += 1
            for w in adj[u]:
                if parents[w] == -1:
                    parents[w] = u
                    qu.append(w)
elif treemode=='broomstick':
    parents = [0]
    while len(parents) * 2 < n:
        parents.append(len(parents)-1)
    root = len(parents)-1
    while len(parents) < n:
        parents.append(root)
    #print(parents)
elif treemode=='line':
    parents = [0]
    for i in range(n-1):
        parents.append(i)
else:
    assert 0

if treemode!='line':
    parents = shuffle_tree_labels(parents)
print(*(p + 1 for p in parents))

def print_tree_edges(parents):
    n = len(parents)
    for child_idx, par in enumerate(parents, start=1):
        if par != child_idx:
            print(par, child_idx)


def get_leaves(parents):
    n = len(parents)
    child_count = [0] * n

    for child_idx, par in enumerate(parents):
        if par != child_idx:
            child_count[par] += 1

    leaves = [i for i in range(n) if child_count[i] == 0]
    return leaves

if 0:
    print("begin tree")
    print_tree_edges(parents)
    print("end tree")

mint=int(cmdlinearg('mint',1))
maxr=int(cmdlinearg('maxr',n-1))

if querymode=='random':
    houses = [i+1 for i in range(n)]
    random.shuffle(houses)
    print(*houses[:m])

    for i in range(q):
        t = randint(mint,2)
        if t == 1:
            a = randint(1,n)
            b = randint(1,n)
            while a==b:
                b = randint(1,n)
            print(t,a,b)
        else:
            print(t,randint(1,m), randint(1,maxr))
elif querymode=='leafs':
    houses = get_leaves(parents)
    if len(houses) > m:
        houses = houses[:m]
    num_leaf = len(houses)
    if len(houses) < m:
        seen = set(houses)
        p = [i for i in range(n)]
        random.shuffle(p)
        while len(houses)<m:
            cand = p[-1]
            del p[-1]
            if cand not in houses:
                houses.append(cand)
                seen.add(cand)
    
    houses = [h+1 for h in houses]
    print(*houses)
    
    for i in range(q):
        x = randint(1, 100)
        t=2
        if x==1 and mint==1:
            t=1
        if t == 1:
            a = randint(1,n)
            b = randint(1,n)
            while a==b:
                b = randint(1,n)
            print(t,a,b)
        else:
            print(t,randint(1,min(m, num_leaf-1)), randint(1,maxr))
elif querymode=="line":
    houses = [i+1 for i in range(n)][:m]
    for i in range(10):
        a=randint(0,n-1)
        b=randint(0,n-1)
        houses[a],houses[b]=houses[b],houses[a]
    print(*houses)

    for i in range(q):
        t = randint(mint,2)
        if t == 1:
            a = randint(1,n)
            b = randint(1,n)
            while a==b:
                b = randint(1,n)
            print(t,a,b)
        else:
            print(t,randint(1,m), randint(1,maxr))
else:
    assert 0