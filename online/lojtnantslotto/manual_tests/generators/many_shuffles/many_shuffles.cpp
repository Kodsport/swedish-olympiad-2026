// hard_knapsack_sa.cpp
// Generates hard knapsack instances using noisy simulated annealing.
//
// Usage (example):
//   g++ -O2 -std=c++17 hard_knapsack_sa.cpp -o hard_knapsack_sa
//   ./hard_knapsack_sa --N 100 --capacity 1000 --maxw 200 --maxp 200 --seed 12345 --sa-iters 200 --eval-max-iters 20000
//
// Key ideas:
//  - "Fitness" of an instance = number of iterations that repeated (random-shuffle + greedy)
//    takes to converge. We measure convergence by "patience": the algorithm stops when
//    it observes `patience` consecutive iterations with no improvement. The fitness is
//    the number of shuffle+greedy iterations performed.
//  - The evaluator is deterministic (fixed evaluation seed) so fitness is reproducible.
//  - Simulated annealing searches the space of item weights/profits using noisy acceptance
//    (Gaussian noise added to acceptance criterion) to avoid local optima.
//
// Tunable parameters are available via command-line flags. The program prints progress
// and the final best instance (weights and profits) to stdout.

#include <bits/stdc++.h>
using namespace std;

struct Item {
    int w;
    int p;
};

// Simple hash for instance (used for reproducible seeds if needed)
uint64_t instance_hash(const vector<Item>& items) {
    uint64_t h = 1469598103934665603ULL; // FNV offset
    for (auto &it : items) {
        h ^= (uint64_t)it.w; h *= 1099511628211ULL;
        h ^= (uint64_t)it.p; h *= 1099511628211ULL;
    }
    return h;
}

// Evaluate an instance by repeatedly doing: random shuffle + greedy
// Returns the number of iterations performed until convergence (or max_iters)
int evaluate_instance(const vector<Item>& items, int capacity, uint64_t eval_seed,
                      int max_iters, int patience) {
    const int N = (int)items.size();
    vector<int> perm(N);
    iota(perm.begin(), perm.end(), 0);
    std::mt19937_64 rng(eval_seed);

    long long best_profit = 0;
    int stagnation = 0;
    int iter = 0;
    for (iter = 0; iter < max_iters; ++iter) {
        shuffle(perm.begin(), perm.end(), rng);
        int rem = capacity;
        long long profit = 0;
        for (int idx : perm) {
            int w = items[idx].w;
            if (w <= rem) {
                rem -= w;
                profit += items[idx].p;
            }
        }
        if (profit > best_profit) {
            best_profit = profit;
            stagnation = 0;
        } else {
            stagnation += 1;
            if (stagnation >= patience) {
                return iter + 1; // number of iterations until convergence
            }
        }
    }
    return max_iters;
}

// Propose a random mutation on instance: small perturbation to a random item,
// or swap two items' (w,p), or resample an item. Keeps values in [1,max].
void mutate_instance(vector<Item>& items, std::mt19937_64 &rng,
                     int maxw, int maxp) {
    std::uniform_int_distribution<int> which(0, 2);
    int choice = which(rng);
    int N = (int)items.size();
    if (choice == 0) {
        // perturb an item's weight and/or profit with small gaussian noise
        std::normal_distribution<double> gauss(0.0, 6.0); // stddev tuned
        std::uniform_int_distribution<int> pick(0, N-1);
        int i = pick(rng);
        int neww = items[i].w + (int)round(gauss(rng));
        int newp = max(1, (int)round(gauss(rng)));
        neww = max(1, min(maxw, neww));
        newp = max(1, min(maxp, newp));
        items[i].w = neww;
        items[i].p = newp;
    } else {
        // resample one item uniformly
        std::uniform_int_distribution<int> pick(0, N-1);
        std::uniform_int_distribution<int> rw(1, maxw);
        std::uniform_int_distribution<int> rp(1, maxp);
        int i = pick(rng);
        items[i].w = rw(rng);
        items[i].p = rp(rng);
    }
}

// Copy-and-mutate wrapper for SA proposals
vector<Item> propose(const vector<Item>& base, std::mt19937_64 &rng, int maxw, int maxp) {
    vector<Item> cand = base;
    mutate_instance(cand, rng, maxw, maxp);
    mutate_instance(cand, rng, maxw, maxp);
    return cand;
}

// Utility to print instance
void print_instance(const vector<Item>& items, int capacity) {
    cout << items.size() << " " << capacity << '\n';
    // cout << "N=" << items.size() << " capacity=" << capacity << "\n";
    // cout << "w p\n";
    for (auto &it : items) cout << it.p << ' ' << it.w << '\n';
}

// Command line parsing (simple)
struct CmdOpts {
    int N = 100;
    int capacity = 200;
    int maxw = 200;
    int maxp = 5;
    uint64_t seed = 123456789ULL;
    int sa_iters = 50000;
    double T0 = 20.0;
    double alpha = 0.995; // cooling multiplier per SA iteration
    double noise_scale = 1.0; // scale of gaussian noise in acceptance
    int eval_max_iters = 20000;
    int eval_patience = 500; // number of consecutive no-improve iter to consider converged
    int start_random_items = 0; // if >0, start from random instance instead of greedy
    bool verbose = true;
};

CmdOpts parse_args(int argc, char** argv) {
    CmdOpts opt;
    for (int i = 1; i < argc; ++i) {
        string a = argv[i];
        auto next_int = [&](int &dst){ if (i+1<argc) dst = stoi(argv[++i]); };
        auto next_u64 = [&](uint64_t &dst){ if (i+1<argc) dst = stoull(argv[++i]); };
        auto next_double = [&](double &dst){ if (i+1<argc) dst = stod(argv[++i]); };
        if (a=="--N") next_int(opt.N);
        else if (a=="--capacity") next_int(opt.capacity);
        else if (a=="--maxw") next_int(opt.maxw);
        else if (a=="--maxp") next_int(opt.maxp);
        else if (a=="--seed") next_u64(opt.seed);
        else if (a=="--sa-iters") next_int(opt.sa_iters);
        else if (a=="--T0") next_double(opt.T0);
        else if (a=="--alpha") next_double(opt.alpha);
        else if (a=="--noise-scale") next_double(opt.noise_scale);
        else if (a=="--eval-max-iters") next_int(opt.eval_max_iters);
        else if (a=="--eval-patience") next_int(opt.eval_patience);
        else if (a=="--verbose") opt.verbose = true;
        else if (a=="--quiet") opt.verbose = false;
        else if (a=="--start-random") next_int(opt.start_random_items);
        else {
            cerr << "Unknown arg: "<<a<<"\n";
            exit(1);
        }
    }
    return opt;
}

int main(int argc, char** argv) {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    CmdOpts opt = parse_args(argc, argv);

    // RNGs
    std::mt19937_64 rng(opt.seed);
    std::mt19937_64 rng_propose(opt.seed ^ 0x9e3779b97f4a7c15ULL);
    std::normal_distribution<double> nnoise(0.0, 1.0);

    // initial instance: moderately hard seed
    vector<Item> cur(opt.N);
    if (opt.start_random_items > 0) {
        std::uniform_int_distribution<int> rw(1, opt.maxw);
        std::uniform_int_distribution<int> rp(1, opt.maxp);
        for (int i = 0; i < opt.N; ++i) {
            cur[i].w = rw(rng);
            cur[i].p = rp(rng);
        }
    } else {
        // heuristic initial instance: weights concentrated around capacity / (N/2) and profits correlated
        double meanw = max(1, opt.capacity / max(1, opt.N/4));
        std::normal_distribution<double> gw(meanw, meanw*0.5 + 1.0);
        std::normal_distribution<double> gp(meanw*1.0, meanw*0.7 + 1.0);
        for (int i = 0; i < opt.N; ++i) {
            int w = max(1, min(opt.maxw, (int)round(gw(rng))));
            int p = max(1, min(opt.maxp, (int)round(gp(rng))));
            cur[i].w = w;
            cur[i].p = p;
        }
    }

    // ensure capacity reasonable
    if (opt.capacity < 1) opt.capacity = 1;

    uint64_t eval_seed = opt.seed ^ 0xdeadbeefcafebabeULL; // fixed evaluation RNG seed

    int cur_fit = evaluate_instance(cur, opt.capacity, eval_seed, opt.eval_max_iters, opt.eval_patience);
    vector<Item> best = cur;
    int best_fit = cur_fit;

    if (opt.verbose) {
        cerr << "Initial fitness: " << cur_fit << "\n";
    }

    using namespace chrono;
    double t_start=30, t_end=1.3e-3, timelim=20000; // t_end<t_start
    auto start = high_resolution_clock::now();
    double T = opt.T0;
    for (int it = 0; it < opt.sa_iters; ++it) {
        // Propose candidate
        vector<Item> cand = propose(cur, rng_propose, opt.maxw, opt.maxp);
        int cand_fit = evaluate_instance(cand, opt.capacity, eval_seed, opt.eval_max_iters, opt.eval_patience);

        int delta = cand_fit - cur_fit; // we want larger fitness
        bool accept = false;
        if (delta >= 0) accept = true;
        else {
            double noise = nnoise(rng_propose) * opt.noise_scale; // gaussian noise
            double prob = exp(double(delta) / max(1e-12, T) + noise);
            std::uniform_real_distribution<double> unif(0.0, 1.0);
            double r = unif(rng_propose);
            if (r < prob) accept = true;
        }

        if (accept) {
            cur.swap(cand);
            cur_fit = cand_fit;
            if (cur_fit > best_fit) {
                best = cur;
                best_fit = cur_fit;
                if (opt.verbose) cerr << "New best at SA it="<<it<<" fitness="<<best_fit<<"\n";
            }
        }

        // cooling
        T = t_start * pow(t_end / t_start, ((it+0.)/opt.sa_iters));
        // occasionally print status
        if (opt.verbose && (it % max(1, opt.sa_iters/20) == 0)) {
            cerr << "SA it="<<it<<" cur_fit="<<cur_fit<<" best_fit="<<best_fit<<" T="<<T<<"\n";
        }
    }

    // Output best found
    cerr << "# Best instance found\n";
    cerr << "# fitness=" << best_fit << " (eval_max_iters="<<opt.eval_max_iters<<" patience="<<opt.eval_patience<<")\n";
    print_instance(best, opt.capacity);

    return 0;
}
