#include "validate.h"

#include <bits/stdc++.h>
using namespace std;

#define rep(i, a, b) for(int i = a; i < (b); ++i)
#define trav(i, a) for(auto& i : a)
#define all(x) begin(x), end(x)
#define sz(x) (int)(x).size()
#define pb push_back
typedef long long ll;
typedef pair<int, int> pii;
typedef vector<int> vi;

auto Start = chrono::high_resolution_clock::now();
void resettimer() { Start = chrono::high_resolution_clock::now(); }
int elapsedmillis() { return chrono::duration_cast<chrono::milliseconds>(chrono::high_resolution_clock::now() - Start).count(); }

/*
Input contains the information of how the judge should behave.
Inputformat:
N Q T
x1 x2 x3 ... xN
*/


// ./a.out ../../data/secret/group1/001-random-01.in  ../../data/secret/group1/001-random-01.ans . < ../../submissions/accepted/harry.py 

vector<vector<int>> g;

int main(int argc, char **argv) {
    init_io(argc, argv);
    
    int n,q,t;
    judge_in >> n >> q >> t;

    vi pos(n);
    rep(i,0,n) judge_in >> pos[i];

    sort(all(pos));

    //std::seed_seq seed(all(words[0])); 
    //std::mt19937 generator(seed);   // mt19937 is a standard mersenne_twister_engine

    cout << t << "\n";
    // "game loop"
    rep(c1,0,q+1) {
        char start;
        if(!(cin >> start)) {
            wrong_answer("Could not read first char of query %d", c1+1);
        }

        if(start != '?' && start != '!') {
			wrong_answer("Query %d started with invalid char", c1+1);
		}

        if (start == '?') {
            ll x;
            if (!(cin >> x)) {
				wrong_answer("Got a ? query but could not read x, query %d", c1+1);
			}

            if (x < -1e9 || x > 2e9) {
                wrong_answer("x out of range (%d)", x);
            }

            if (c1 == q) {
                wrong_answer("Too many queries");
            }


            if (x >= pos[n-1]) cout << pos[n-1] << "\n";
            else if (x <= pos[0]) cout << pos[0] << "\n";
            else {

                // Binary search to find the largest element <= x
                int l = 0;
                int r = n-1;

                while (l < r) {
                    int m = (l+r+1)/2;
                    if (pos[m] <= x) l = m;
                    else r = m-1;
                }

                // Check wether l och l+1 is smaller
                // If tiebreak, choose l.

                ll best = pos[l+1];
                if (abs((ll)best-(ll)x) >= abs((ll)pos[l]-(ll)x)) best = pos[l];
                //cout << l << "\n";
                //cout << pos[l] << " " << pos[l+1] << " " << x << "\n";
                cout << best << "\n";
            }
        }

        if (start == '!') {
            int n_ANS;
            if (!(cin >> n_ANS)) {
                wrong_answer("Could not read the number n when reading answer");
            }

            if (n_ANS != n) {
                wrong_answer("Number of ducks found was incorrect (got %d wanted %d)", n_ANS, n);
            }


            // Necessary to check newline here? Maybe not?


            vi X_ANS;
            rep(c2,0,n) {
                int x;
                if (!(cin >> x)) {
                    wrong_answer("Could not read %d:th number when reading answer", c2+1);
                }
                X_ANS.push_back(x);
            }
            sort(all(X_ANS));

            rep(c2,0,n){
				if(pos[c2] != X_ANS[c2]){
					int a = X_ANS[c2];
					int b = pos[c2];
					wrong_answer("Number %d of answer was wrong (got %d wanted %d)", c2+1, a, b);
				}
			}
			break;
        }
    }

    string trailing;
	if(cin >> trailing){
		wrong_answer("Trailing output");
	}

  	accept();

}