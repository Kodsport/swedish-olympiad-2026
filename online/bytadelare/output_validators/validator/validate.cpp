#include "validate.h"

#include <bits/stdc++.h>
using namespace std;

#define rep(i, a, b) for(int i = a; i < (b); ++i)
#define trav(a, x) for(auto& a : x)
#define all(x) x.begin(), x.end()
#define sz(x) (int)(x).size()
typedef long long ll;
typedef pair<int, int> pii;
typedef pair<ll, ll> pll;
typedef vector<int> vi;
typedef vector<ll> vl;

const int MAX_MOVES = 1e6;
int main(int argc, char **argv) {
    init_io(argc, argv);

    int n,x;
    judge_in >> n >> x;
    vi A(n, 0);
    rep(c1,0,n){
        judge_in >> A[c1]; 
    }

    auto check = [&](istream& sol, feedback_function feedback) {
        string claims_sol;
        if (!(sol >> claims_sol)) feedback("Could not read first row");
        for (char& c : claims_sol) c = tolower(c);
        if(claims_sol != "ja" && claims_sol != "nej") {
            feedback("Answer is not {JA|NEJ}");
        }

        if (claims_sol == "nej") {
            if (x==0) return make_pair(false, true);
            string trailing;
            if(sol >> trailing) {
                feedback("Trailing output");
            }
            return make_pair(false, true);
        }
        assert(claims_sol=="ja");

        int m;
        if(!(sol >> m)) {
            return make_pair(true, false);
        }
        if (m < 0 || m > MAX_MOVES) {
            return make_pair(true, false);
        }

        vi A2 = A;
        rep(c1,0,m) {
            int i,j;
            if(!(sol >> i >> j)) {
                return make_pair(true, false);
            }
            if(i < 1 || i > n || j < 1 || j > n) {
                return make_pair(true, false);
            }
            i--;
            j--;
            if(!(A2[i]%A2[j] == 0 || A2[j]%A2[i] == 0)) {
                return make_pair(true, false);
            }
            swap(A2[i], A2[j]);
        }

        rep(i, 0, sz(A2)-1) {
            if (A2[i]>A2[i+1]) return make_pair(true, false);
        }

		string trailing;
		if(sol >> trailing) {
            return make_pair(true, false);
        }
        return make_pair(true, true);
	};

	auto [judge_claims_sol, judge_sol_is_valid] = check(judge_ans, judge_error);
	auto [contestant_claims_sol, contestant_sol_is_valid] = check(author_out, wrong_answer);

    if (!judge_claims_sol) {
        if (contestant_claims_sol) {
            if (contestant_sol_is_valid) judge_error("Contestant found solution that judge didnt");
            else wrong_answer("Contest incorrectly claims a solution when there is none");
        }
        else {
            accept();
        }
    }
    else {
        // Allow invalid judge solution for sample
        if (!judge_sol_is_valid && n != 4) judge_error("Judge found incorrect solution");
        
        if (!contestant_claims_sol) wrong_answer("Contest incorrectly claims solution doesn't exist");

        if (!contestant_sol_is_valid) {
            if (x==0) accept();
            else wrong_answer("Contest output incorrect solution");
        }
    }

    accept();
}
