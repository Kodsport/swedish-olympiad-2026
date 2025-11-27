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
N T
x1 y1 x2 y2
*/

// T = 0 (sample, must use less than 366 queries)
// T = 1 (point system)

// ./a.out ../../data/secret/group3/091-g3-38.in  ../../data/secret/group3/091-g3-38.ans . < ../../submissions/accepted/harry2.py 

int test_group_score(int T) {
    if (T == 0) {
        return 0;
    }
    else if (T == 1) return 5;
    else if (T == 2) return 9;
    else if (T == 3) return 11;

    assert(0);
}

int compute_score(int Q) {
    double score;

    if (Q <= 5) {
        score = 75.0;
    } else if (Q <= 6) {
        score = 63.0;
    } else if (Q <= 10) {
        score = 55.0 * pow(10.0/55.0, (Q-10.0)/85);
    } else if (Q <= 75) {
        score = 53.0 * pow(8.0 / 53.0, (Q - 10.0) / 64.0);
    } else if (Q <= 365) {
        score = 4.0 * pow(1.0 / 4.0, (Q - 160.0) / 205.0);
    } else {
        score = 0.0;
    }

    // Round to nearest integer
    return (int) lround(score);
}

int main(int argc, char **argv) {
    init_io(argc, argv);

    const int Q = 365;
    
    int N,T,x1,y1,x2,y2;
    judge_in >> N >> T >> x1 >> y1 >> x2 >> y2;

    cout << N << "\n";
    rep(i,0,N) {
        rep(j,0,N) {
            if (i % 2 == 1 and j % 2 == 1) cout << "H";
            else cout << ".";
        }
        cout << "\n";
    }


    //std::seed_seq seed(all(words[0])); 
    //std::mt19937 generator(seed);   // mt19937 is a standard mersenne_twister_engine

    // "game loop"

    int usedQueries = 0;

    rep(c1,0,Q+1) {
        char start;
        if(!(cin >> start)) {
            wrong_answer("Could not read first char of query %d", c1+1);
        }

        if(start != '?' && start != '!') {
			wrong_answer("Query %d started with invalid char (%c)", c1+1, start);
		}

        if (start == '?') {
            usedQueries++;
            if (c1 == Q) {
                wrong_answer("Too many queries. Used more than %d queries.", c1+1);
            }

            vector<string> grid;
            rep(i,0,N) {
                string row;
                if (!(cin >> row)) {
                    wrong_answer("Got a ? query but could not read row %d, for query %d.", i+1, c1+1);
                }
                
                if (sz(row) != N) {
                    wrong_answer("Got a ? query but row %d does not contain N characters. It contains %d characters.", i+1, sz(row));
                }

                rep(j,0,N) {
                    if (i % 2 == 1 and j % 2 == 1) {
                        if (row[j] != 'H') {
                            wrong_answer("Got a ? query, but cell (%d, %d) should be a house (H). Instead it was (%c).", i,j ,row[j]);
                        }
                    }
                    else {
                        if (row[j] != '#' and row[j] != '.') {
                            wrong_answer("Got a ? query, but cell (%d, %d) should be a either a wall (#) or empty (.). Instead it was (%c).", i,j ,row[j]);
                        }
                    }
                }
                
                grid.push_back(row);
            }
            
            // Do a BFS from (x1,y1) until it reaches (x2,y2)
            
            vector<vi> seen(N,vi(N,0));
            seen[x1][y1] = 1;
            vi dists(1,0);
            vector<pii> BFS = {make_pair(x1,y1)};
            vector<pii> adj = {{0,1},{1,0},{-1,0},{0,-1}};
            int ind = 0;

            int ret = -1;
            while (ind < sz(BFS)) {
                int cx,cy;
                tie(cx,cy) = BFS[ind];

                for (auto &[dx,dy]: adj) {
                    if (0 <= cx+dx and cx+dx < N and 0 <= cy+dy and cy+dy < N) {
                        if (grid[cx+dx][cy+dy] == '#') {
                            continue;
                        }

                        if (seen[cx+dx][cy+dy]) continue;

                        seen[cx+dx][cy+dy] = 1;
                        BFS.push_back({cx+dx,cy+dy});
                        dists.push_back(dists[ind]+1);
                        if (cx+dx == x2 and cy+dy == y2) {
                            ret = dists[ind]+1;
                            break;
                        }
                    }
                }

                if (ret != -1) break;
                ind++;
            }

            cout << ret << '\n';

        }

        if (start == '!') {
            vi ANS(4);
            rep(i,0,4) {
                if (!(cin >> ANS[i])) {
                    wrong_answer("Got a ! response, but could not read %d-th integer.", i+1);
                }
            }

            if (ANS[0] == x1 and ANS[1] == y1 and ANS[2] == x2 and ANS[3] == y2) {
                break;
            }

            if (ANS[0] == x2 and ANS[1] == y2 and ANS[2] == x1 and ANS[3] == y1) {
                break;
            }

            wrong_answer("The output is incorrect. (%d, %d) and (%d, %d) were given.", ANS[0], ANS[1], ANS[2], ANS[3]);

        }
    }

    string trailing;
	if(cin >> trailing){
		wrong_answer("Trailing output");
	}

    judge_message("Accepted, used %d queries.\n", usedQueries);

    if (T < 4) {
        accept_with_score_integer(test_group_score(T));
    } 
    else if (T == 4){
        accept_with_score_integer(compute_score(usedQueries));
    }
  	
    assert(0);

}