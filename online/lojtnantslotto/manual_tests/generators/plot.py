import sys
import matplotlib.pyplot as plt

def knapsack_dp(profits, weights, capacity):
    n = len(profits)
    # DP table
    dp = [[0] * (capacity + 1) for _ in range(n + 1)]

    for i in range(1, n + 1):
        p = profits[i - 1]
        w = weights[i - 1]
        for c in range(capacity + 1):
            if w <= c:
                dp[i][c] = max(dp[i-1][c], dp[i-1][c-w] + p)
            else:
                dp[i][c] = dp[i-1][c]

    # Backtrack
    chosen = [False] * n
    c = capacity
    for i in range(n, 0, -1):
        if dp[i][c] != dp[i-1][c]:
            chosen[i-1] = True
            c -= weights[i-1]

    return chosen


def greedy_ratio(profits, weights, capacity):
    n = len(profits)
    items = list(range(n))
    items.sort(key=lambda i: profits[i] / weights[i], reverse=True)

    chosen = [False] * n
    total_w = 0

    for i in items:
        if total_w + weights[i] <= capacity:
            chosen[i] = True
            total_w += weights[i]

    return chosen


def main():
    data = sys.stdin.read().strip().split()
    if not data:
        print("No input provided.")
        return

    N = int(data[0])
    capacity = int(data[1])

    profits = []
    weights = []

    idx = 2
    for _ in range(N):
        p = int(float(data[idx]))
        w = int(float(data[idx + 1]))
        profits.append(p)
        weights.append(w)
        idx += 2

    # Compute DP + greedy selections
    chosen_dp = knapsack_dp(profits, weights, capacity)
    chosen_greedy = greedy_ratio(profits, weights, capacity)

    # Color mapping
    colors = []
    for dp, gr in zip(chosen_dp, chosen_greedy):
        if dp and gr:
            colors.append("purple")
        elif dp:
            colors.append("blue")
        elif gr:
            colors.append("green")
        else:
            colors.append("lightgray")

    # Plot
    plt.scatter(weights, profits, c=colors)

    plt.xlabel("Weight")
    plt.ylabel("Profit")
    plt.title("Knapsack Items: DP vs Greedy Selection")
    plt.grid(True)

    # Legend
    import matplotlib.patches as mpatches
    legend_patches = [
        mpatches.Patch(color="blue", label="DP only"),
        mpatches.Patch(color="green", label="Greedy only"),
        mpatches.Patch(color="purple", label="Both"),
        mpatches.Patch(color="lightgray", label="Neither"),
    ]
    plt.legend(handles=legend_patches)

    plt.savefig("res.png")


if __name__ == "__main__":
    main()
