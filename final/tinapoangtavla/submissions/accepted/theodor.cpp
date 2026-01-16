#include <bits/stdc++.h>
using namespace std;
    
    
#define INF ((ll)(1e9+7))
#define fo(i, n) for(ll i=0;i<((ll)n);i++)
#define deb(x) cout << #x << " = " << (x) << endl
#define deb2(x, y) cout << #x << " = " << (x) << ", " << #y << " = " << (y) << endl
#define pb push_back
typedef long long ll;
typedef pair<ll, ll> pl;
typedef vector<ll> vl;

struct Node{
    Node *lnode = 0, *rnode = 0;
    int l, r;
    pl best = {0, -1}, sbest = {0, -1};
    Node(int l, int r) : l(l), r(r){}

    int query(int lo, int hi, int from){
        if(r <= lo || hi <= l) return -1;
        if(r <= hi && lo <= l) return (from == best.second?sbest.first:best.first);
        push();
        return max(lnode->query(lo, hi, from), rnode->query(lo, hi, from));
    }
    
    void update(int lo, int hi, pl x){
        if(r <= lo || hi <= l) return;
        if(r <= hi && lo <= l){
            if(x.second == best.second)best = max(best, x);
            else sbest = max(sbest, x);
            if(best<sbest) swap(best, sbest);
            return;
        }
        push();
        lnode->update(lo, hi, x);
        rnode->update(lo, hi, x);
    }

    void push(){
        if(r-l == 1) return;
        if(!lnode){
            int mid = (r+l)/2;
            lnode = new Node(l, mid);
            rnode = new Node(mid, r);
        }
        if(best.second == lnode->best.second)lnode->best = max(lnode->best, best);
        else lnode->sbest = max(lnode->sbest, best);
        if(lnode->best<lnode->sbest) swap(lnode->best, lnode->sbest);
        if(sbest.second == lnode->best.second)lnode->best = max(lnode->best, sbest);
        else lnode->sbest = max(lnode->sbest, sbest);
        if(lnode->best<lnode->sbest) swap(lnode->best, lnode->sbest);

        if(best.second == rnode->best.second)rnode->best = max(rnode->best, best);
        else rnode->sbest = max(rnode->sbest, best);
        if(rnode->best<rnode->sbest) swap(rnode->best, rnode->sbest);
        if(sbest.second == rnode->best.second)rnode->best = max(rnode->best, sbest);
        else rnode->sbest = max(rnode->sbest, sbest);
        if(rnode->best<rnode->sbest) swap(rnode->best, rnode->sbest);
    }
};


int main(){
    cin.tie(0)->sync_with_stdio(0);
    // cout << fixed << setprecision(20);
    
    ll n, p, h, f;
    cin >> n >> p >> h >> f;

    
    ll team, prob;
    string resin;
    vector<vl> submissions(n, vl(p, 0));
    vector<vl> ac(n, vl(p, 0));
    vector<pl> scores(n, {0, 0});
    fo(i, h){
        cin >> team >> prob >> resin;
        submissions[--team][--prob]++;
        if(resin == "AC"){
            scores[team].first++;
            scores[team].second+=submissions[team][prob];
        }
    }
    vector<pl> final = scores;
    vector<pl> optimal = scores;
    vector<tuple<ll, ll, char>> saved;
    fo(i, f){
        cin >> team >> prob >> resin;
        submissions[--team][--prob]++;
        saved.pb({team, prob, resin=="AC"?'A':'W'});
        if(resin == "AC"){
            final[team].first++;
            final[team].second+=submissions[team][prob];
        }
    }
    
    char verdict;
    fo(i, f){
        tie(team, prob, verdict) = saved[f-1-i];
        if(ac[team][prob]) continue;
        ac[team][prob] = f-i;
        optimal[team].first++;
        optimal[team].second+=submissions[team][prob];
    }
    int subs = f+h+10;
    Node st(0, (p+1)*subs);
    
    fo(i, f){
        tie(team, prob, verdict) = saved[i];
        // deb2(team, prob);
        // deb2(scores[team].first, optimal[team].first);
        st.update(scores[team].first*subs+subs-scores[team].second, optimal[team].first*subs+subs-optimal[team].second, {i+1, team});
        if(verdict == 'A'){
            scores[team].first++;
            scores[team].second+=submissions[team][prob];
        }else if(ac[team][prob] == i+1){
            optimal[team].first--;
            optimal[team].second-=submissions[team][prob];
        }
        
    }
    
    fo(i, n){
        // deb2(scores[i].first, scores[i].second);
        cout << st.query(scores[i].first*subs+subs-scores[i].second, scores[i].first*subs+subs-scores[i].second+1, i) << " ";
    }
    
    
    return 0;
}