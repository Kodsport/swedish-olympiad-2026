#!/usr/bin/python3
import sys
input = sys.stdin.readline

def main():
    N, M = map(int, input().split())
    
    lost = set()
    weights = [0] * (N + 1)
    total_queries = 0
    
    def query(i, s):
        nonlocal total_queries
        total_queries += 1
        print(f"? {i} {s}", flush=True)
        resp = input().strip()
        if resp == "borta":
            lost.add(i)
            return True
        return False
    
    # solve(ilo, ihi, vlo, vhi):
    # All alive berries in [ilo..ihi] have weights in [vlo..vhi].
    # Determine their weights.
    def solve(ilo, ihi, vlo, vhi):
        if vlo == vhi:
            for i in range(ilo, ihi + 1):
                if i not in lost:
                    weights[i] = vlo
            return
        if ilo > ihi:
            return
        
        # Find alive index nearest to middle
        imid_target = (ilo + ihi) // 2
        found = -1
        for delta in range(0, ihi - ilo + 2):
            up = imid_target + delta
            dn = imid_target - delta
            if up <= ihi and up not in lost:
                found = up
                break
            if dn >= ilo and dn not in lost:
                found = dn
                break
        
        if found == -1:
            return  # all lost
        
        imid = found
        vmid = (vlo + vhi) // 2
        
        if query(imid, vmid):
            # borta: w_imid <= vmid
            # [ilo..imid-1]: all have w <= w_imid <= vmid, range [vlo, vmid]
            # [imid+1..ihi]: w in [vlo, vhi] (not narrowed)
            solve(ilo, imid - 1, vlo, vmid)
            solve(imid + 1, ihi, vlo, vhi)
        else:
            # kvar: w_imid > vmid, so w_imid in [vmid+1, vhi]
            # [imid+1..ihi]: all have w >= w_imid > vmid, range [vmid+1, vhi]
            # [ilo..imid-1]: w in [vlo, vhi] (not narrowed)
            # imid: w in [vmid+1, vhi]
            solve(ilo, imid - 1, vlo, vhi)
            solve(imid, ihi, vmid + 1, vhi)
    
    sys.setrecursionlimit(100000)
    solve(1, N, 1, M)
    
    for i in lost:
        weights[i] = -1
    
    result = " ".join(str(weights[i]) for i in range(1, N + 1))
    print(f"! {result}", flush=True)

main()