#!/usr/bin/python3
import sys

n = 3
edges = []
pairings = []

def add_edge(u, v):
    edges.append((u, v))
    pairings.append(-1)
    return len(edges) - 1

def pair_edges(e1, e2):
    pairings[e1] = e2
    pairings[e2] = e1

# Core nodes
A = 4; B = 5; C = 6; D = 7
n = 7

# Entry and exit
add_edge(1, A)
add_edge(C, 3)

# The cycle (Order forces standard DFS to go A->B->C->D->A)
e_AB = add_edge(A, B) # Safe
e_BC = add_edge(B, C) # Forbidden
e_CD = add_edge(C, D) # Safe
e_DA = add_edge(D, A) # Forbidden

# 1 -> 2 Path (Used to create forbidden edges)
n += 1; f1 = n
n += 1; f2 = n
p1 = add_edge(1, f1)
p2 = add_edge(f1, f2)
add_edge(f2, 2)

# Pairings
pair_edges(e_BC, p1) # B->C forbidden
pair_edges(e_DA, p2) # D->A forbidden

# Safe edges get paired with dummies
n += 1; d1 = n; e_d1 = add_edge(2, d1)
n += 1; d2 = n; e_d2 = add_edge(2, d2)
pair_edges(e_AB, e_d1)
pair_edges(e_CD, e_d2)

unpaired = [i for i in range(len(edges)) if pairings[i] == -1]
if len(unpaired) % 2 != 0:
    n += 1
    unpaired.append(add_edge(2, n))
for i in range(0, len(unpaired), 2):
    pair_edges(unpaired[i], unpaired[i+1])

print(f"{n} {len(edges)}")
# Output edges randomly to hide the trick, but adj list order usually sorts by node ID.
# Node IDs are set so DFS natural order is A(4)->B(5)->C(6)->D(7).
for i in range(len(edges)):
    print(f"{edges[i][0]} {edges[i][1]} {pairings[i] + 1}")