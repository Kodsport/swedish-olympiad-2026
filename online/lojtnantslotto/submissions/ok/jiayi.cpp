// https://pokval26.kattis.com/contests/hs7zye/problems/lojtnantslotto  07/12 - 2025

#include<bits/stdc++.h>
using namespace std;

#define pb push_back

int main() {

    int N, T;
    cin >> N >> T;


    vector<vector<double>> allcastles;
    vector<int> sizes(5, 0);
    for (int i=0; i<N; i++) {
        double l, g;
        cin >> l >> g;
        sizes[l-1]++;
        allcastles.pb({g/l, l, g});
    }
    sort(allcastles.begin(), allcastles.end());

    vector<int> take(5, 0);
    int troops = 0;
    for (int i=0; i<N; i++) {
        if (troops + allcastles[i][2] > T) break;
        troops += allcastles[i][2];
        take[allcastles[i][1]-1]++;
    }

    vector<vector<vector<double>>> castles(5);
    for (int i=0; i<N; i++)
        castles[allcastles[i][1]-1].pb(allcastles[i]);

    vector<vector<int>> prefcost(5, vector<int>(1, 0));
    for (int v=0; v<5; v++) {   
        for (int i=0; i<sizes[v]; i++) {
            prefcost[v].pb(prefcost[v][i]+castles[v][i][2]);
        }
    }

    int maxi = 0;
    for (int a=max(take[0]-15, 0); a<=min(take[0]+15, sizes[0]); a++) {
        for (int b=max(take[1]-15, 0); b<=min(take[1]+15, sizes[1]); b++) {
            for (int c=max(take[2]-15, 0); c<=min(take[2]+15, sizes[2]); c++) {
                for (int d=max(take[3]-15, 0); d<=min(take[3]+15, sizes[3]); d++) {
                    for (int e=max(take[4]-15, 0); e<=min(take[4]+15, sizes[4]); e++) {
                        if (prefcost[0][a] + prefcost[1][b] + prefcost[2][c] + prefcost[3][d] + prefcost[4][e] > T) continue;
                        maxi = max(maxi, a*1 + b*2 + c*3 + d*4 + e*5);
                    }
                }
            }
        }
    }

    cout << maxi;

    return 0;
}
