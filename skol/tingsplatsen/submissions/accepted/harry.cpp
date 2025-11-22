#include <bits/stdc++.h>
using namespace std;

#define rep(i,a,b) for(int i = (a); i < (b); i++)
#define all(x) begin(x),end(x)
#define sz(x) (int)(x).size()
typedef vector<int> vi;
typedef long long ll;
typedef pair<int,int> pii;
typedef vector<ll> vl;

int main() {
    cin.tie(0)->sync_with_stdio(0);
    cin.exceptions(cin.failbit);

    int n,m;
    cin >> n >> m;


    // 0 - .
    // 1 - *
    // 2 - X
    vector<vi> grid(n,vi(m,0));

    rep(i,0,n) {
        string temp;
        cin >> temp;

        rep(j,0,m) {
            if (temp[j] == '*') grid[i][j] = 1;
        }
    }

    rep(i,0,n) {
        rep(j,0,m) {
            if (grid[i][j] == 0) {
                int dist = 0;

                rep(x,0,n) rep(y,0,m) {
                    if (grid[x][y] == 1) {
                        if (dist == 0) {
                            dist = abs(x-i) + abs(y-j);
                        }

                        else {
                            if (dist != abs(x-i) + abs(y-j)) dist = -1;
                        }
                    }
                }

                if (dist != -1) {
                    grid[i][j] = 2;
                }
            }
        }
    }

    rep(i,0,n) {
        rep(j,0,m) { 
            cout << ".*X"[grid[i][j]];
        }
        cout << "\n";
    }

}