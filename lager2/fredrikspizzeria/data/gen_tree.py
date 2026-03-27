#!/usr/bin/python3

import sys
import random
import tree

def cmdlinearg(name, default=None):
    for arg in sys.argv:
        if arg.startswith(name + "="):
            return arg.split("=")[1]
    if default is None:
        print("missing parameter", name)
        sys.exit(1)
    return default


random.seed(int(cmdlinearg('seed', sys.argv[-1])))
n = int(cmdlinearg('n'))
mode = cmdlinearg('mode')

if mode == 'random':
    edges = tree.random_edge_tree(n)
elif mode == 'prufer':
    edges = tree.random_prufer_tree(n)
elif mode=='path':
    extra_nodes = int(cmdlinearg('extra-nodes'))
    edges = tree.gen_path(n-extra_nodes)
    edges = tree.extra_edges(edges, extra_nodes)
elif mode=='star':
    extra_nodes = int(cmdlinearg('extra-nodes'))
    edges = tree.gen_star(n-extra_nodes)
    edges = tree.extra_edges(edges, extra_nodes)
elif mode=='broomstick':
    extra_nodes = int(cmdlinearg('extra-nodes'))
    n_reduced = n-extra_nodes
    path = tree.gen_path(n_reduced//2)
    star = tree.gen_star(n_reduced//2 + n_reduced%2)
    edges = tree.merge_trees(path, star, (tree.min_degree_node(path), tree.max_degree_node(star)))
    edges = tree.extra_edges(edges, extra_nodes)
elif mode=='dumbbell':
    extra_nodes = int(cmdlinearg('extra-nodes'))
    n_reduced = n-extra_nodes
    path = tree.gen_path(n_reduced//3)
    star1 = tree.gen_star(n_reduced//3)
    star2 = tree.gen_star(n_reduced//3 + n_reduced%3)
    edges = tree.merge_trees(path, star1, (tree.node_count(path)-1, tree.max_degree_node(star1)))
    edges = tree.merge_trees(edges, star2, (0, tree.max_degree_node(star2)))
    edges = tree.extra_edges(edges, extra_nodes)


edges = tree.shuffle_labels(edges)
assert len(edges) % 2 == 0
n = len(edges) + 1

pairing = list(range(len(edges)))
random.shuffle(pairing)

partner = [-1]*(n-1)
for i in range(0, len(edges), 2):
    partner[pairing[i]] = pairing[i+1]
    partner[pairing[i+1]] = pairing[i]

print(n, n-1)
for i, (u, v) in enumerate(edges):
    print(u+1, v+1, partner[i]+1)
