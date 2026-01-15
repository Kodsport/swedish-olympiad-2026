#!/usr/bin/env python3
"""
BFS solution for the Grinding problem.
State: (strength, unlocked_dungeons)
We explore all possible dungeon runs and find minimum to reach strength >= B.
"""
from collections import deque

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
    
    # Precompute for each dungeon: given a starting strength, 
    # what strength do we end with (or -1 if we can't complete) and how much xp we gain
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
                # Kicked out, but we still gained xp from previous monsters
                return (strength, xp_gained, False)
        return (strength, xp_gained, True)
    
    # BFS: state = (strength, unlocked)
    # unlocked = number of dungeons unlocked (1-indexed, so unlocked=1 means only dungeon 0 accessible)
    # We want minimum runs to reach strength >= B
    
    # Since strength can be huge, we need to be smart. 
    # But for BFS to be "correct", we use a dictionary to track visited states.
    # State: (strength, unlocked)
    
    start_strength = 1
    start_unlocked = 1  # Can access dungeons 0..unlocked-1
    
    if start_strength >= B:
        print(0)
        return
    
    # BFS
    queue = deque()
    queue.append((start_strength, start_unlocked, 0))  # (strength, unlocked, runs)
    visited = {(start_strength, start_unlocked): 0}
    
    answer = float('inf')
    
    while queue:
        strength, unlocked, runs = queue.popleft()
        
        if runs >= answer:
            continue
        
        # Try each available dungeon
        for d in range(min(unlocked, K)):
            new_strength, xp_gained, completed = run_dungeon(d, strength)
            
            if new_strength >= B:
                answer = min(answer, runs + 1)
                continue
            
            new_unlocked = unlocked
            if completed and d == unlocked - 1 and unlocked < K:
                new_unlocked = unlocked + 1
            
            # Only add if we gained something (xp or unlocked)
            if new_strength >= strength or new_unlocked > unlocked:
                state = (new_strength, new_unlocked)
                if state not in visited or visited[state] > runs + 1:
                    visited[state] = runs + 1
                    queue.append((new_strength, new_unlocked, runs + 1))
    
    if answer == float('inf'):
        print(-1)  # Impossible
    else:
        print(answer)

solve()
