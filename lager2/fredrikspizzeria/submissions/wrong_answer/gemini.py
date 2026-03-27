import sys

sys.setrecursionlimit(300000)
d = sys.stdin.read().split()
if not d:
    sys.exit(0)

n = int(d[0])
m = int(d[1])

adj = [[] for _ in range(n + 1)]
ov = [0] * (m + 1)

idx = 2
for i in range(1, m + 1):
    u = int(d[idx])
    v = int(d[idx+1])
    o = int(d[idx+2])
    idx += 3
    adj[u].append((i, v))
    adj[v].append((i, u))
    ov[i] = o

dep = [0] * (n + 1)
pn = [0] * (n + 1)
pe = [0] * (n + 1)
cid = [0] * (m + 1)
cyc = [[]]
ccyc = 0

def dfs(u, p_node, p_edge, depth_val):
    global ccyc
    dep[u] = depth_val
    pn[u] = p_node
    pe[u] = p_edge
    for eid, v in adj[u]:
        if v == p_node:
            continue
        if dep[v] > 0:
            if dep[v] < dep[u]:
                ccyc += 1
                cid[eid] = ccyc
                cyc.append([eid])
                cur = u
                while cur != v:
                    p_e = pe[cur]
                    cid[p_e] = ccyc
                    cyc[-1].append(p_e)
                    cur = pn[cur]
        else:
            dfs(v, u, eid, depth_val + 1)

dfs(1, 0, 0, 1)

p12 = set()
cur = 2
while cur != 1:
    p12.add(pe[cur])
    cur = pn[cur]

p13 = set()
cur = 3
while cur != 1:
    p13.add(pe[cur])
    cur = pn[cur]

f12 = []
b0 = {}
b1 = {}
cp12 = set()

for e in p12:
    c = cid[e]
    if c == 0:
        f12.append(e)
    else:
        cp12.add(c)

for c in cp12:
    b0[c] = []
    b1[c] = []
    for e in cyc[c]:
        if e in p12:
            b0[c].append(e)
        else:
            b1[c].append(e)

ok = [True] * (m + 1)
adict = [{} for _ in range(m + 1)]

for e in f12:
    ok[ov[e]] = False

for c in cp12:
    for e in b0[c]:
        k = ov[e]
        if adict[k].get(c, 1) == 0:
            ok[k] = False
        else:
            adict[k][c] = 1
    for e in b1[c]:
        k = ov[e]
        if adict[k].get(c, 0) == 1:
            ok[k] = False
        else:
            adict[k][c] = 0

ass = [tuple() for _ in range(m + 1)]
for k in range(1, m + 1):
    if ok[k] and adict[k]:
        ass[k] = tuple(sorted(adict[k].items()))

ans = "Nej"
cp13 = set()
cuts = []

for e in p13:
    if cid[e] == 0:
        cuts.append((0, e))
    else:
        cp13.add(cid[e])

for c in cp13:
    a0 = []
    a1 = []
    for e in cyc[c]:
        if e in p13:
            a0.append(e)
        else:
            a1.append(e)
    cuts.append((1, c, a0, a1))

for cut in cuts:
    if cut[0] == 0:
        e = cut[1]
        if ok[ov[e]]:
            ans = "Ja"
            break
    else:
        c = cut[1]
        a0 = cut[2]
        a1 = cut[3]
        v0 = {ov[e] for e in a0 if ok[ov[e]]}
        v1 = {ov[e] for e in a1 if ok[ov[e]]}
        
        if not v0 or not v1:
            continue
            
        mv1 = {}
        for k1 in v1:
            for cp, v in ass[k1]:
                if (cp, v) not in mv1:
                    mv1[(cp, v)] = []
                mv1[(cp, v)].append(k1)
                
        chk = set()
        fnd = False
        
        for k0 in v0:
            sg = ass[k0]
            if sg in chk:
                continue
            chk.add(sg)
            
            ssz = 0
            for cp, v in sg:
                op = (cp, 1 - v)
                if op in mv1:
                    ssz += len(mv1[op])
                    
            if ssz < len(v1):
                fnd = True
                break
                
            bd = set()
            for cp, v in sg:
                op = (cp, 1 - v)
                if op in mv1:
                    bd.update(mv1[op])
                    
            if len(bd) < len(v1):
                fnd = True
                break
                
        if fnd:
            ans = "Ja"
            break

print(ans)