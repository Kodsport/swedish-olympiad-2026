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

const int MAXQ = 100000;
const int MAXN = 300;
vi scores = {0, 16, 30, 54};

int main(int argc, char **argv) {
    init_io(argc, argv);

    int N,M,T;
    judge_in >> N >> M >> T;

    vector<int> weights(N);
    for(int c1 = 0; c1 < N; c1++){
        judge_in >> weights[c1];
    }

    cout << N << " " << M << "\n";
    int queries = 0;
    vi is_lost(N, 0);
    int num_lost = 0;

    while(queries <= MAXQ){
        char start;
        if(!(cin >> start)){
            wrong_answer("Could not read first character of query %d", queries+1);
        }

        if(start != '?' && start != '!')wrong_answer("Invalid first character of query %d", queries+1);

        if(start == '?'){
            if(queries == MAXQ){
                wrong_answer("Too many queries");
            }

            int i,s;
            if(!(cin >> i >> s)){
                wrong_answer("Could not read i and s in ?-query %d", queries+1);
            }
            if(!(1 <= i && i <= N))wrong_answer("i oob in query %d", queries+1);
            if(!(1 <= s && s <= M))wrong_answer("s oob in query %d", queries+1);
            i--;
            if(is_lost[i] == 1){
                wrong_answer("Tries to suck a lost strawberry in query %d", queries+1);
            }

            if(weights[i] <= s){
                num_lost++;
                is_lost[i] = 1;
                cout << "borta\n";
            }
            else{
                cout << "kvar\n";
            }

            if(num_lost > M-1){
                wrong_answer("Lost too many strawberries");
            }
        }
        else{
            rep(c1,0,N){
                int ans;
                if(!(cin >> ans)){
                    wrong_answer("Could not read %dth number in final answer", c1+1);
                }
                int correct = weights[c1];
                if(is_lost[c1] == 1)correct = -1;
                if(ans != correct){
                    wrong_answer("Incorrect answer number %d: printed %d, correct is %d", c1+1, ans, correct);
                }
            }
            break;
        }

        queries++;
    }

    string trailing;
	if(cin >> trailing){
		wrong_answer("Trailing output");
	}

    judge_message("Accepted, used %d queries.\n", queries);

    int score = scores[T] / (1 + int(queries > MAXN));
    accept_with_score_integer(score);
  	
    assert(0);

}