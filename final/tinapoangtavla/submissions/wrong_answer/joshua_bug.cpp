// incorrectly applies frozen submissions
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
    p2 score=p2(0,0);
    vi problem_tries;
    vi solved;
    Team(int p) : solved(p), problem_tries(p) {}
    Team() {}

    void solve(int p, int t)
    {
        assert(!solved[p]);
        solved[p] = 1;
        score.first++;
        score.second += t + 20 * problem_tries[p];
    }

    void fail(int p)
    {
        assert(!solved[p]);
        problem_tries[p]++;
    }

    void apply(tuple<int, int, int, char> sub)
    {
        auto [team, prob, tim, verdict] = sub;
        if (verdict == 'W') fail(prob);
        else
        {
            solve(prob, tim);
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
    rep(i, n)
    {
        int team, prob, tim;
        char verdict;
        cin >> team >> prob >> tim >> verdict;
        team--; prob--;
        public_scores[team].apply(make_tuple(team, prob, tim, verdict));
    }
    vector<Team> private_scores = public_scores;

    vi ans(n, f);
    vi done(n);

    // per problem and team, frozen submissions compress to a potential +(0,0) or +(1, t)

    vector<tuple<int, int, int, char>> frozensubs;
    rep(i, f)
    {
        int team, prob, tim;
        char verdict;
        cin >> team >> prob >> tim >> verdict;
        team--; prob--;
        frozensubs.emplace_back(team, prob, tim, verdict);
        private_scores[team].apply(frozensubs.back());
    }

    auto team_lowerbound = [&](int team_ind)
    {
        return public_scores[team_ind].score;
    };

    auto team_upperbound = [&](int team_ind)
    {
        Team me = public_scores[team_ind];
        for (auto [team, prob, tim, verdict] : frozensubs)
        {
            if (team == team_ind)
            {
                me.apply(make_tuple(team, prob, tim, verdict));
            }
        }
        return me.score;
    };

    auto canonicalize = [&](p2 p)
    {
        return BIG*p.first+p.second;
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
                int otherteam_lb = canonicalize(team_lowerbound(j));
                int otherteam_ub = canonicalize(team_upperbound(j));
                int my_final = canonicalize(private_scores[i].score);
                if (my_final >= otherteam_lb && my_final <= otherteam_ub)
                {
                    any_cover = 1;
                    break;
                }
            }
            if (!any_cover) ans[i] = num_subs, done[i]=1;
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


    return 0;
}
