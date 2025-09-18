import sys
import random

# Default parameter values
DEFAULT = {
    'n': '10',            # number of mathematicians
    'm': '8',             # number of publications
    'usetot': None,       # target total usages across all publications (sum of sizes); if None will be picked later
    'locs': '2',          # minimum publication size
    'hics': '6',          # maximum publication size
    'mode': 'random',     # generation mode
    'seed': '1',          # RNG seed
}

def cmdlinearg(name, fallback=None):
    """Return value of command line argument name=val, else fallback, else DEFAULT[name]."""
    for arg in sys.argv[1:]:
        if arg.startswith(name + "="):
            return arg.split("=", 1)[1]
    if fallback is not None:
        return fallback
    return DEFAULT[name]

def randomstar(n):
    G = []
    for i in range(n - 1):
        G.append((i, n-1))
    return G

def randomline(n):
    G = []
    for i in range(n - 1):
        G.append((i, i+1))
    return G

def randomtree(n):
    edges = []
    for i in range(1, n):
        edges.append((i, random.randint(0,i-1)))
    return edges

class DisjointSetUnion:
    def __init__(self, n):
        self.parent = list(range(n))
        self.size = [1] * n
        self.num_sets = n

    def find(self, a):
        acopy = a
        while a != self.parent[a]:
            a = self.parent[a]
        while acopy != a:
            self.parent[acopy], acopy = a, self.parent[acopy]
        return a

    def union(self, a, b):
        a, b = self.find(a), self.find(b)
        if a != b:
            if self.size[a] < self.size[b]:
                a, b = b, a

            self.num_sets -= 1
            self.parent[b] = a
            self.size[a] += self.size[b]

    def set_size(self, a):
        return self.size[self.find(a)]

    def __len__(self):
        return self.num_sets

random.seed(int(cmdlinearg('seed', sys.argv[-1])))

n = int(cmdlinearg('n'))
m = int(cmdlinearg('m'))

locs = int(cmdlinearg('locs', '2'))  # min publication size
hics = int(cmdlinearg('hics', str(max(2, min(n, 10)))))  # max publication size
hics = min(hics, n)  # cannot exceed n
if locs < 2:
    locs = 2  # problem constraint: at least 2 authors per publication
if locs > hics:
    locs = hics

mode = cmdlinearg('mode', 'random')  # random, line, star, tree, bigclique

# Target total usages (sum of publication sizes). If not provided pick a value in feasible interval.
min_tot = m * locs
max_tot = m * hics
goaltotused_arg = cmdlinearg('usetot')
if goaltotused_arg is None or goaltotused_arg == 'None':
    # Aim for ~70% of max if possible
    goaltotused = min_tot + (max_tot - min_tot) * 7 // 10
else:
    goaltotused = int(goaltotused_arg)

goaltotused = max(min_tot, min(max_tot, goaltotused))

UF = DisjointSetUnion(n)
allnodes = [*range(n)]
edges = []
edgeset = []
totused = 0


assert m * hics >= goaltotused
assert m * locs <= goaltotused
assert m * hics >= n, "Impossible to include all mathematicians at least once with given m and hics"

def ensure_erdos_inclusion(groups):
    """Ensure ERDOS (node 0) appears in at least one group; if not, add it to first group."""
    if not any(0 in g for g in groups):
        if groups:
            groups[0].append(0)
    return groups

def randombigcliques(n, m, locs, hics):
    """Generate m publications where ERDOS is in each, distributing other nodes roughly evenly."""
    others = list(range(1, n))
    random.shuffle(others)
    groups = []
    idx = 0
    for i in range(m):
        # Remaining publications count
        rem_pubs = m - i
        rem_nodes = len(others) - idx
        # Minimum we must allocate to cover all remaining nodes: distribute roughly evenly
        max_take = min(hics - 1, rem_nodes)  # -1 because ERDOS will be added
        if rem_pubs == 1:
            take = rem_nodes
        else:
            low = max(1, locs - 1)
            hi = max(low, max_take)
            take = random.randint(low, hi) if max_take > 0 else 1
        take = min(max_take, take)
        picked = others[idx:idx+take]
        idx += take
        group = [0] + picked
        if len(group) < 2:
            group.append(random.randrange(1, n))
            if group[1] == 0:
                group[1] = (group[1] + 1) % n
        groups.append(group)
    return groups

if mode == 'random':
    # First ensure connectivity using union operations
    while UF.size[UF.find(0)] < n and len(edges) < m:
        # Pick group size between locs and hics but not exceeding n
        gsize = random.randint(locs, hics)
        temp = random.sample(allnodes, gsize)
        roots = {UF.find(x) for x in temp}
        if len(roots) > 1:  # merges components
            for x in temp:
                UF.union(temp[0], x)
            edges.append(temp[:])
            edgeset.append(set(temp))
            totused += len(temp)

    # Add more publications (random pairs) if we still need more
    while len(edges) < m:
        a, b = random.sample(allnodes, 2)
        edges.append([a, b])
        edgeset.append({a, b})
        totused += 2

    # Increase total usage by adding new (unused-in-publication) nodes to random publications
    attempts = 0
    while totused < goaltotused and attempts < 5 * goaltotused:
        edgeind = random.randint(0, len(edges) - 1)
        if len(edges[edgeind]) >= hics:
            attempts += 1
            continue
        node = random.choice(allnodes)
        if node not in edgeset[edgeind]:
            edgeset[edgeind].add(node)
            edges[edgeind].append(node)
            totused += 1
        attempts += 1

elif mode == 'line':
    # Build a single publication chain by grouping neighbors; ensure sizes between locs and hics
    seq = list(range(n))
    i = 0
    while i < n - 1 and len(edges) < m:
        gsize = min(hics, max(locs, 2))
        group = seq[i:i+gsize]
        if len(group) < 2:
            break
        edges.append(group)
        edgeset.append(set(group))
        totused += len(group)
        i += gsize - 1  # overlap by one to keep connectivity
    ensure_erdos_inclusion(edges)
elif mode == 'star':
    center = 0
    leaves = [i for i in range(n) if i != center]
    random.shuffle(leaves)
    chunk = 0
    while leaves and len(edges) < m:
        take = random.randint(max(1, locs - 1), min(hics - 1, len(leaves)))
        group_leaves = leaves[:take]
        leaves = leaves[take:]
        group = [center] + group_leaves
        if len(group) < 2:
            break
        edges.append(group)
        edgeset.append(set(group))
        totused += len(group)
elif mode == 'tree':
    # Use random tree edges, then optionally bundle them to respect m by merging
    tedges = randomtree(n)  # list of (u,v)
    # Start with each edge as a publication, then merge randomly until count == m (or break if <= m)
    pubs = [[u, v] for u, v in tedges]
    random.shuffle(pubs)
    while len(pubs) > m:
        a = pubs.pop()
        b = pubs.pop()
        merged = list({*a, *b})
        if len(merged) <= hics:
            pubs.append(merged)
        else:
            # fallback: keep them separate (can't exceed hics); reinsert second
            pubs.append(b)
            pubs.append(a)
            break
    edges = pubs[:m]
    for g in edges:
        edgeset.append(set(g))
        totused += len(g)
elif mode == 'bigclique':
    edges = randombigcliques(n, m, locs, hics)
    totused = sum(len(g) for g in edges)
    for g in edges:
        edgeset.append(set(g))
else:
    raise ValueError(f"Unknown mode: {mode}")

# Final safety: trim or pad publication list to exactly m
if len(edges) > m:
    edges = edges[:m]
elif len(edges) < m:
    # Add dummy pair publications involving ERDOS and random nodes
    used_pairs = set()
    its = 0
    while len(edges) < m:
        node = random.randrange(1, n)
        its+=1
        if node not in used_pairs or its > int(1e5):
            edges.append([0, node])
            edgeset.append({0, node})
            totused += 2
            used_pairs.add(node)

ensure_erdos_inclusion(edges)

# Ensure every node appears in at least one publication (coverage & connectivity for BFS usage).
def ensure_all_nodes_used(edges, n, hics):
    present = set()
    for g in edges:
        present.update(g)
    missing = [x for x in range(n) if x not in present]
    if not missing:
        return edges
    # Try to insert missing nodes into publications with spare capacity
    spare_indices = [i for i,g in enumerate(edges) if len(g) < hics]
    si = 0
    for node in missing:
        # Insert into a publication with space if possible
        inserted = False
        while si < len(spare_indices):
            gi = spare_indices[si]
            if len(edges[gi]) < hics and node not in edges[gi]:
                edges[gi].append(node)
                inserted = True
                if len(edges[gi]) == hics:
                    si += 1
                break
            si += 1
        if inserted:
            continue
        # Otherwise replace a non-ERDOS member in some publication
        for g in edges:
            # find candidate to replace (prefer node not unique elsewhere but simple approach)
            for i, member in enumerate(g):
                if member != 0:  # avoid removing ERDOS
                    if node not in g:
                        g[i] = node
                        inserted = True
                        break
            if inserted:
                break
        if not inserted:
            # Fallback: append ignoring hics constraint (should not happen due to earlier assert)
            edges[0].append(node)
    return edges

edges = ensure_all_nodes_used(edges, n, hics)

# Build names: ERDOS + random-looking unique uppercase names (1-10 letters, not duplicating ERDOS)
names = ['ERDOS']

def build_name_encoder(rng):
    alphabet = list('ABCDEFGHIJKLMNOPQRSTUVWXYZ')
    rng.shuffle(alphabet)
    return alphabet

encoder = build_name_encoder(random)

def encode_index(idx):
    # base-26 encoding with shuffled alphabet
    if idx == 0:
        base = encoder[0]
    else:
        chars = []
        x = idx
        while x > 0:
            x, r = divmod(x, 26)
            chars.append(encoder[r])
        base = ''.join(chars)
    # Guarantee length at least 2 by padding with next letter cyclically
    if len(base) == 1:
        pad = encoder[(encoder.index(base[0]) + 1) % 26]
        base = base + pad
    if base == 'ERDOS':
        base = encoder[1] + base  # avoid collision with reserved name
    return base[:10]

for i in range(1, n):
    names.append(encode_index(i))

# Output instance
print(n, len(edges))
for g in edges:
    # Deduplicate within group and cap to hics
    uniq = list(dict.fromkeys(g))[:hics]
    if len(uniq) < 2:
        # force at least size 2 by adding ERDOS or another node
        if 0 not in uniq:
            uniq.append(0)
        else:
            uniq.append((uniq[0] + 1) % n)
    print(len(uniq), ' '.join(names[x] for x in uniq))
