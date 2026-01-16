#include <bits/stdc++.h>
using namespace std;

using ll = long long;
using vi = vector<ll>;
using vvi = vector<vi>;
using p2 = pair<ll, ll>;
const ll inf = 1e18;

#define rep(i,n) for (ll i = 0; i < (n); i++)
#define repp(i,a,n) for (ll i = (a); i < (n); i++)
#define repe(i, arr) for (auto& i : arr)
#define all(x) begin(x),end(x)
#define sz(x) ((ll)(x).size())

struct Team
{
    p2 score = p2(0, 0);
    vi problem_tries;
    vi solved;
    Team(int p) : solved(p), problem_tries(p) {}
    Team() {}

    void solve(int p)
    {
        assert(!solved[p]);
        solved[p] = 1;
        score.first++;
        score.second += problem_tries[p];
    }

    void fail(int p)
    {
        assert(!solved[p]);
        problem_tries[p]++;
    }

    void apply(tuple<int, int, char> sub)
    {
        auto [team, prob, verdict] = sub;
        if (verdict == 'W') fail(prob);
        else
        {
            solve(prob);
        }
    }
};

struct Node
{
    Node* lchild = 0, * rchild = 0;
    ll l, r;
    int m;
    int lazy;

    Node(ll l, ll r) : l(l), r(r), m(0), lazy(0) {}

    void push()
    {
        if (l == r) return;
        ll mid = (l + r) / 2;
        if (!lchild)
        {
            lchild = new Node(l, mid);
            rchild = new Node(mid + 1, r);
        }
        lchild->m += lazy;
        lchild->lazy += lazy;

        rchild->m += lazy;
        rchild->lazy += lazy;
        lazy = 0;
    }

    void add(ll ql, ll qr, int v)
    {
        if (l > qr || r < ql) return;
        if (l >= ql && r <= qr)
        {
            m += v;
            lazy = lazy + v;
        }
        else
        {
            push();
            lchild->add(ql, qr, v);
            rchild->add(ql, qr, v);
            m = max(rchild->m, lchild->m);
        }
    }

    int query(ll ql, ll qr)
    {
        if (l > qr || r < ql) return 0;
        if (l >= ql && r <= qr) return m;
        else
        {
            push();
            return max(lchild->query(ql, qr), rchild->query(ql, qr));
        }
    }
};

int main()
{
    cin.tie(0)->sync_with_stdio(0);

    const int BIG = 1e7;

    int n, p, h, f;
    cin >> n >> p >> h >> f;

    vector<Team> public_scores(n, Team(p));
    vvi totsubs(n, vi(p));
    rep(i, h)
    {
        int team, prob;
        string verdict;
        cin >> team >> prob >> verdict;
        team--; prob--;
        totsubs[team][prob]++;
        public_scores[team].apply(make_tuple(team, prob, verdict=="AC"?'A':'W'));
    }
    vector<Team> private_scores = public_scores;

    vi ans(n, f);
    vi done(n);

    // per problem and team, frozen submissions compress to a potential +(0,0) or +(1, t)

    vector<tuple<int, int, char>> frozensubs;
    rep(i, f)
    {
        int team, prob;
        string verdict;
        cin >> team >> prob >> verdict;
        team--; prob--;
        frozensubs.emplace_back(team, prob, verdict=="AC"?'A':'W');
        private_scores[team].apply(frozensubs.back());
    }

    auto team_lowerbound = [&](int team_ind)
    {
        return public_scores[team_ind].score;
    };

    vvi numsubs(n, vi(p));
    vector<Team> public_upperbound = public_scores;
    for (auto [team, prob, verdict] : frozensubs)
    {
        totsubs[team][prob]++;
        numsubs[team][prob]++;
    }
    rep(team, n)
    {
        rep(prob, p)
        {
            if (numsubs[team][prob])
            {
                rep(j, numsubs[team][prob] - 1) public_upperbound[team].fail(prob);
                public_upperbound[team].solve(prob);
            }
        }
    }

    auto team_upperbound = [&](int team_ind)
    {
        return public_upperbound[team_ind].score;
    };

    auto canonicalize = [&](p2 p)
    {
        return BIG * p.first + (BIG - p.second);
    };

    ll max_val = 1e14;
    Node* root = new Node(0, max_val);

    auto add = [&](int team, int val) {
        ll otherteam_lb = canonicalize(team_lowerbound(team));
        ll otherteam_ub = canonicalize(team_upperbound(team));
        otherteam_ub--;
        if (otherteam_lb > otherteam_ub) return;
        assert(otherteam_lb <= otherteam_ub + 1);
        assert(otherteam_lb >= 0);
        assert(otherteam_ub < max_val);
        root->add(otherteam_lb, otherteam_ub, val);
    };

    rep(i, n) add(i, 1);

    int num_subs = 0;
    rep(frozen_idx, sz(frozensubs))
    {
        rep(i, n)
        {
            if (done[i]) continue;
            add(i, -1);
            int my_final = canonicalize(private_scores[i].score);
            int amount_covering = root->query(my_final, my_final);
            add(i, 1);
            if (amount_covering == 0) ans[i] = frozen_idx, done[i] = 1;
        }

        {
            auto [team, prob, verdict] = frozensubs[frozen_idx];

            if (numsubs[team][prob] == 1)
            {
                add(team, -1);
                public_scores[team].apply(frozensubs[frozen_idx]);
                numsubs[team][prob]--;
                if (verdict == 'W')
                {
                    public_upperbound[team].score.first--;
                    public_upperbound[team].score.second -= totsubs[team][prob] - 1;
                }
                add(team, 1);
            }
            else
            {
                public_scores[team].apply(frozensubs[frozen_idx]);
                assert(verdict == 'W');
                numsubs[team][prob]--;
            }
        }
    }

    repe(t, ans)
    {
        assert(t != -1);
        cout << t << ' ';
    }


    return 0;
}
