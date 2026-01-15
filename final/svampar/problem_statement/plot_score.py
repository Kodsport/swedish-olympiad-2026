import matplotlib.pyplot as plt

# -----------------------------
# INPUT DATA
# -----------------------------

# List of line segments: each is ((x1,y1), (x2,y2))

# 9: 100
# 10: 80
# 11: 70
# 9: 100

segments = [
    ((9, 100), (10, 70)),
    ((10, 70), (12, 50)),
    ((12, 50), (40, 20)),
    ((40, 20), (256, 10))
]

# x-values where we want to mark the y-intersection
sample_x = [9, 10, 11, 12, 19, 20, 21, 128, 256]

# -----------------------------
# PROCESS + PLOT
# -----------------------------
fig, ax = plt.subplots()

for (x1, y1), (x2, y2) in segments:
    # plot the segment
    ax.plot([x1, x2], [y1, y2], marker='o')

    # compute slope/intercept for the infinite line
    m = (y2 - y1) / (x2 - x1)
    b = y1 - m * x1

    # compute y-values at desired x-values
    for sx in sample_x:
        # only mark if x lies within the segment's x-range
        if min(x1, x2) <= sx <= max(x1, x2):
            sy = m * sx + b
            ax.plot(sx, sy, 'x', markersize=8)
            ax.text(sx, sy, f"{sy:.1f}", fontsize=8, verticalalignment='bottom')

# formatting
ax.set_xlabel('X')
ax.set_ylabel('Y')
ax.set_title('Line Segments with Sampled Points')
ax.grid(True)

plt.show()
