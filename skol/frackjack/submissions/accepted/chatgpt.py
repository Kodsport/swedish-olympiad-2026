# Read input
A=int(input())
B=int(input())

# Maximum allowed distance
target = 21

# Check if Alice and Bertil are valid (<= 21)
alice_valid = A <= target
bertil_valid = B <= target

# Determine winner
if alice_valid and (not bertil_valid or A > B):
    print("Alice")
elif bertil_valid and (not alice_valid or B > A):
    print("Bertil")
else:
    print("Jack")
