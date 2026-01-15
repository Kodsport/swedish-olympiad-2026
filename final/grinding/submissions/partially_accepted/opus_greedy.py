#!/usr/bin/env python3
"""
Greedy solution for the Grinding problem.

Strategy: Fix the prefix of dungeons to unlock. Then:
1. If you can defeat the rightmost dungeon (to unlock next), do it.
2. Otherwise, take the dungeon that gives most xp.
3. Once all desired dungeons are unlocked, always take the one giving most xp.

Try all possible prefixes and take the minimum.
"""

def solve():
    line = input().split()
    K, B = int(line[0]), int(line[1])
    
    dungeons = []
    for _ in range(K):
        N = int(input())
        monsters = []
        for _ in range(N):
            s, e = map(int, input().split())
            monsters.append((s, e))
        dungeons.append(monsters)
    
    def run_dungeon(dungeon_idx, strength):
        """
        Run a dungeon with given strength.
        Returns (new_strength, xp_gained, completed)
        """
        dungeon = dungeons[dungeon_idx]
        xp_gained = 0
        for s, e in dungeon:
            if strength >= s:
                strength += e
                xp_gained += e
            else:
                return (strength, xp_gained, False)
        return (strength, xp_gained, True)
    
    def xp_from_dungeon(dungeon_idx, strength):
        """How much xp can we gain from this dungeon with given strength?"""
        _, xp_gained, _ = run_dungeon(dungeon_idx, strength)
        return xp_gained
    
    def can_complete_dungeon(dungeon_idx, strength):
        """Can we complete this dungeon?"""
        _, _, completed = run_dungeon(dungeon_idx, strength)
        return completed
    
    def min_strength_to_complete(dungeon_idx):
        """Minimum strength needed to complete a dungeon."""
        # Need strength > max(s_i - sum of previous xp)
        dungeon = dungeons[dungeon_idx]
        min_needed = 0
        xp_so_far = 0
        for s, e in dungeon:
            # At this point, we have original_strength + xp_so_far
            # We need original_strength + xp_so_far > s
            # So original_strength > s - xp_so_far
            min_needed = max(min_needed, s - xp_so_far + 1)
            xp_so_far += e
        return max(1, min_needed)
    
    if 1 >= B:
        print(0)
        return
    
    best_answer = float('inf')
    
    # Try each target prefix: unlock dungeons 0..target-1
    for target in range(1, K + 1):
        strength = 1
        unlocked = 1  # dungeons 0..unlocked-1 are available
        runs = 0
        
        # Phase 1: unlock up to target dungeons
        MAX_RUNS = 10**7  # Safety limit
        while unlocked < target and runs < MAX_RUNS:
            # We want to unlock dungeon 'unlocked' (0-indexed)
            # Check if we can complete dungeon unlocked-1 (the rightmost available that unlocks next)
            rightmost = unlocked - 1
            
            if unlocked <= K and can_complete_dungeon(rightmost, strength):
                # Complete the rightmost to unlock next
                new_strength, _, _ = run_dungeon(rightmost, strength)
                strength = new_strength
                unlocked += 1
                runs += 1
            else:
                # Take the dungeon that gives most xp among available
                best_xp = -1
                best_d = -1
                for d in range(unlocked):
                    xp = xp_from_dungeon(d, strength)
                    if xp > best_xp:
                        best_xp = xp
                        best_d = d
                
                if best_xp <= 0:
                    # Can't make progress, this target is impossible
                    break
                
                new_strength, _, _ = run_dungeon(best_d, strength)
                strength = new_strength
                runs += 1
            
            if strength >= B:
                break
        
        if strength >= B:
            best_answer = min(best_answer, runs)
            continue
        
        if unlocked < target:
            # Couldn't unlock target dungeons
            continue
        
        # Phase 2: we have unlocked 'target' dungeons (0..target-1), now grind for xp
        while strength < B and runs < MAX_RUNS:
            best_xp = -1
            best_d = -1
            for d in range(target):
                xp = xp_from_dungeon(d, strength)
                if xp > best_xp:
                    best_xp = xp
                    best_d = d
            
            if best_xp <= 0:
                # Can't make progress
                break
            
            new_strength, _, _ = run_dungeon(best_d, strength)
            strength = new_strength
            runs += 1
        
        if strength >= B:
            best_answer = min(best_answer, runs)
    
    if best_answer == float('inf'):
        print(-1)
    else:
        print(best_answer)

solve()
