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
    vi solved,problem_tries;
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

int main()
{
    cin.tie(0)->sync_with_stdio(0);

    int n, p, h, f;
    cin >> n >> p >> h >> f;

    vector<Team> public_scores(n, Team(p));
    rep(i, h)
    {
        int team, prob;
        string verdict;
        cin >> team >> prob >> verdict;
        team--; prob--;
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
            return public_scores[team_ind].score; // p2 (solved, penalty)
        };

    auto team_upperbound = [&](int team_ind)
        {
            Team me = public_scores[team_ind];
            vi any(p);
            vi numsubs(p);
            for (auto [team, prob, verdict] : frozensubs)
            {
                if (team == team_ind)
                {
                    any[prob] = 1;
                    numsubs[prob]++;
                }
            }
            rep(i, p)
            {
                if (any[i])
                {
                    rep(j, numsubs[i] - 1) me.fail(i);
                    me.solve(i);
                }
            }
            return me.score; // p2 (solved, penalty)
        };

    // key: larger is better => compare (solved, -penalty) lexicographically
    auto key = [&](p2 a)
    {
        return pair<ll,ll>(a.first, -a.second);
    };

    int num_subs = 0;
    while (sz(frozensubs))
    {
        rep(i, n)
        {
            if (done[i]) continue;
            bool any_cover = 0;
            rep(j, n)
            {
                if (i == j) continue;
                auto other_lb_key = key(team_lowerbound(j));
                auto other_ub_key = key(team_upperbound(j));
                if (other_lb_key == other_ub_key) continue;
                auto my_final_key = key(private_scores[i].score);
                if (my_final_key >= other_lb_key && my_final_key < other_ub_key)
                {
                    any_cover = 1;
                    break;
                }
            }
            if (!any_cover) ans[i] = num_subs, done[i] = 1;
        }
        num_subs++;
        public_scores[get<0>(frozensubs[0])].apply(frozensubs[0]);
        frozensubs.erase(begin(frozensubs));
    }

    repe(t, ans)
    {
        assert(t != -1);
        cout << t << ' ';
    }
    cout << '\n';

    return 0;
}
