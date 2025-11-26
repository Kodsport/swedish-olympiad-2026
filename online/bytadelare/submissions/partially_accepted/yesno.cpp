#include <bits/stdc++.h>
using namespace std;

#define rep(i, a, b) for (int i = a; i < (b); ++i)
#define trav(a, x) for (auto &a : x)
#define all(x) x.begin(), x.end()
#define sz(x) (int)(x).size()
typedef long long ll;
typedef pair<int, int> pii;
typedef pair<ll, ll> pll;
typedef vector<int> vi;
typedef vector<ll> vl;
typedef long double ld;
typedef unsigned long long ull;

struct UF
{
    vi e;
    UF(int n) : e(n, -1) {}
    bool sameSet(int a, int b) { return find(a) == find(b); }
    int size(int x) { return -e[find(x)]; }
    int find(int x) { return e[x] < 0 ? x : e[x] = find(e[x]); }
    bool join(int a, int b)
    {
        a = find(a), b = find(b);
        if (a == b)
            return false;
        if (e[a] > e[b])
            swap(a, b);
        e[a] += e[b];
        e[b] = a;
        return true;
    }
};

const int MAXN = 1001;

int n;
vi A;
vector<pii> A2;
vi now, goal;
vector<pii> ANS;
vector<vi> graph(MAXN, vi());

int PAR[MAXN] = {0};

bool dfs(int i, int par, int g){
    PAR[i] = par;
    if(now[i] == g){
        int i2 = i;
        vi path;
        while(i2 != -1){
            path.push_back(i2);
            i2 = PAR[i2];
        }
        reverse(all(path));
        rep(c1,0,sz(path)-1){
            ANS.push_back({now[path[c1]], now[path[c1+1]]});
            swap(now[path[c1]], now[path[c1+1]]);
        }

        return 1;
    }

    trav(y, graph[i]){
        if(y != par){
            if(dfs(y,i,g)){
                return 1;
            }
        }
    }

    return 0;

}

int main()
{
    ios_base::sync_with_stdio(0);
    cin.tie(0);

    ll a,b,c,d;

    cin >> n >> a;
    rep(c1,0,n){
        now.push_back(c1);
        goal.push_back(-1);
        cin >> a;
        A.push_back(a);
        A2.push_back({a, c1});
    }
    sort(all(A2));
    rep(c1,0,n){
        goal[A2[c1].second] = c1;
    }

    // Get a spanning tree just to simplify things and not make it cubic
    vi deg(n, 0);
    UF uf(n);
    rep(c1,0,n){
        rep(c2,0,c1){
            if(A[c1]%A[c2] == 0 || A[c2]%A[c1] == 0){
                if(!uf.sameSet(c1, c2)){
                    uf.join(c1, c2);
                    graph[c1].push_back(c2);
                    graph[c2].push_back(c1);
                    deg[c1]++;
                    deg[c2]++;
                }   
            }
        }
    }

    vi Q;
    rep(c1,0,n){
        if(deg[c1] <= 1){
            Q.push_back(c1);
        }
    }

    while(sz(Q) > 0){
        int c1 = Q.back();
        Q.pop_back();
        trav(y, graph[c1]){
            deg[y]--;
            if(deg[y] == 1)Q.push_back(y);
        }
        if(!dfs(c1,-1,goal[c1])){
            cout << "NEJ\n";
            return 0;
        }
    }

    cout << "JA\n";
    /*
    cout << sz(ANS) << "\n";
    trav(p, ANS){
        cout << p.first+1 << " " << p.second+1 << "\n";
    }
    */


    return 0;
}