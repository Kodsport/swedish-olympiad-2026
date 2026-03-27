# Helper functions for generating trees
# We represent graph as list of tuples (u, v). 0-indexed

import random, heapq

class UF:
    def __init__(self, n):
        self.par=[i for i in range(n)]
    
    def find(self,u):
        root=u
        while self.par[root]!=root:
            root=self.par[root]

        while u != root:
            self.par[u], u = root, self.par[u]

        return root

    def merge(self,a,b):
        a=self.find(a)
        b=self.find(b)
        self.par[b]=a

def node_count(edges):
    return len(edges) + 1

def edge_count(edges):
    return len(edges)

def shuffle_labels(edges, n=None):
    if n is None:
        n = node_count(edges)
    perm = list(range(n))
    random.shuffle(perm)

    new_edges = []
    for u, v in edges:
        new_edges.append((perm[u], perm[v]))

    random.shuffle(new_edges)
    return new_edges


def max_degree_node(edges):
    degree = [0] * node_count(edges)
    ret = (0,0)
    for u, v in edges:
        degree[u]+=1
        degree[v]+=1
        ret = max(ret, (degree[u],u))
        ret = max(ret, (degree[v],v))
    return ret[1]

def min_degree_node(edges):
    degree = [0] * node_count(edges)
    ret = (node_count(edges)+1,0)
    for u, v in edges:
        degree[u]+=1
        degree[v]+=1
        ret = min(ret, (degree[u],u))
        ret = min(ret, (degree[v],v))
    return ret[1]

def merge_trees(a, b, cut_edge=None):
    merged_edges = []
    node_a_base = node_count(a)
    merged_edges += a
    for u,v in b:
        merged_edges.append((u+node_a_base, v+node_a_base))
    
    if cut_edge:
        u,v = cut_edge
        merged_edges.append((u, v+node_a_base))
    return merged_edges

def make_hairy(edges):
    node_base = node_count(edges)
    hairy_edges = []
    for u in range(node_base):
        hairy_edges.append((u, u+node_base))
    return edges + hairy_edges

def extra_edges(edges, k):
    node_base = node_count(edges)
    extra_edges = []
    for u in range(k):
        extra_edges.append((random.randint(0, node_base-1), u+node_base))
    return edges + extra_edges

def replace_with_stars(edges, num_leafs):
    n = node_count(edges)
    new_edges = []
    
    leaf_base = n
    for center in range(n):
        for i in range(num_leafs):
            leaf = leaf_base + center * num_leafs + i
            new_edges.append((center, leaf))
    
    return edges + new_edges


def print_edges(edges, base=1):
    for u,v in edges:
        print(u+base, v+base)

# Generators

def single_node():
    return []

def random_edge_tree(n): # Add random acyclic edges
    uf = UF(n)
    edges = []
    while len(edges) < n-1:
        u = random.randint(0, n-1)
        v = random.randint(0, n-1)
        if uf.find(u) != uf.find(v):
            edges.append((u,v))
            uf.merge(u,v)
    return edges

def random_prufer(n):
    return [random.randrange(n) for _ in range(max(0, n-2))]

def prufer_to_edges(prufer):
    n = len(prufer) + 2
    deg = [1]*n
    for x in prufer: deg[x] += 1
    leaves = [i for i in range(n) if deg[i] == 1]
    heapq.heapify(leaves)
    edges = []
    for v in prufer:
        u = heapq.heappop(leaves)
        edges.append((u, v))
        deg[u] -= 1; deg[v] -= 1
        if deg[v] == 1: heapq.heappush(leaves, v)
    u, v = heapq.heappop(leaves), heapq.heappop(leaves)
    edges.append((u, v))
    return edges

def random_prufer_tree(n):
    if n==1:
        return []
    return prufer_to_edges(random_prufer(n))

def gen_star(n):
    assert n
    edges = []
    for i in range(1, n):
        edges.append((0, i))
    return edges

def gen_path(n):
    edges = []
    for i in range(n-1):
        edges.append((i, i+1))
    return edges

