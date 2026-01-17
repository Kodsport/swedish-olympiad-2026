#include "validate.h"

#include <bits/stdc++.h>
using namespace std;

#define rep(i, b) for(int i = 0; i < (b); i++)
#define all(x) begin(x), end(x)
#define sz(x) (int)(x).size()
typedef long long ll;
typedef pair<int, int> p2;
typedef vector<ll> vi;


template <size_t N>
bitset<N> operator+(const bitset<N>& a, const bitset<N>& b) {
    bitset<N> result;
    bool carry = false;

    for (size_t i = 0; i < N; ++i) {
        bool bitA = a[i];
        bool bitB = b[i];

        result[i] = bitA ^ bitB ^ carry;
        carry = (bitA && bitB) || (bitA && carry) || (bitB && carry);
    }
    if (carry) judge_error("Got carry adding two bitsets");
    return result;
}

const int MAX_ROUNDS = 256;
int main(int argc, char **argv) {
	init_io(argc, argv);

	int n;
	judge_in >> n;

	vi target(n);
	for (ll& t : target) judge_in >> t;
    vector<char> allowed_moves = {
        'p',
        '<',
        '>',
        '+',
        '^',
        '&',
        '|'
    };

	auto check = [&](istream& sol, feedback_function feedback){
        using bs = bitset<260>;
        vector<bs> shrooms(n);

        int rounds;
        sol >> rounds;
        if (rounds < 0) {
            author_message("Tried to use negative number of rounds");
            feedback("Negative rounds");
        }
        if (rounds > MAX_ROUNDS) {
            author_message("Tried to use too many rounds");
            feedback("Too many rounds");
        }

        rep(i, rounds) {
            vector<tuple<char,int>> moves(n);
            rep(j, n) {
                char c;
                if (!(sol >> c)) {
                    author_message("Expected more output");
                    feedback("Expected more output");
                }
                c = tolower(c);
                if (c=='p') {
                    moves[j] = {c, -1};
                    continue;
                }
                if (find(all(allowed_moves), c) == allowed_moves.end()) {
                    author_message("Outputted illegal operation type");
                    feedback("Unknown operation");
                }

                int a;
                if (!(sol >> a)) {
                    author_message("Expected more output");
                    feedback("Expected more output");
                }
                
                if (a < 1 || a > n) {
                    author_message("Index in operation points to cell that doesn't exist");
                    feedback("Out of range operation target");
                }
                a--;
                moves[j] = {c,a};
            }

            vector<bs> new_shrooms(n);

            rep(j, n) {
                auto [c, a] = moves[j];
                if (c=='p') new_shrooms[j] = shrooms[j];
                else if (c=='<') new_shrooms[j] = shrooms[a] << 1;
                else if (c=='>') new_shrooms[j] = shrooms[a] >> 1;
                else if (c=='+') new_shrooms[j] = shrooms[a] + bs(1);
                else if (c=='^') new_shrooms[j] = shrooms[j] ^ shrooms[a];
                else if (c=='&') new_shrooms[j] = shrooms[j] & shrooms[a];
                else if (c=='|') new_shrooms[j] = shrooms[j] | shrooms[a];
                else judge_error("Unknown character type slipped by");

                if (new_shrooms[j][MAX_ROUNDS]) {
                    judge_error("Impossibly large value set");
                }
            }

            shrooms = new_shrooms;
        }

        rep(i, n) {
            bs goal(target[i]);
            if (goal != shrooms[i]) {
                author_message("Incorrect final array");
                feedback("Incorrect final array");
            }
        }

		string trailing;
		if(sol >> trailing) {
            author_message("More output than indicated by number of rounds");
            feedback("Trailing output");
        }
        return rounds;
	};

	int judge_rounds = check(judge_ans, judge_error);
	int author_rounds = check(author_out, wrong_answer);

    auto eval_line_between = [](p2 a, p2 b, int x) { // take line a -> b (coordinates), and eval at x, floored
        int dy = b.second - a.second;
        int dx = b.first - a.first;
        int intercept = a.second * dx - a.first * dy;

        return (dy * x + intercept) / dx;
    };

    auto scoring_function = [eval_line_between](int rounds) {
        if (rounds <= 9) return 100;
        else if (rounds == 10) return 70;
        else if (rounds == 11) return 60;
        else if (rounds == 12) return 50;
        else if (rounds <= 40) {
            p2 a = {12,50};
            p2 b = {40,20};
            return eval_line_between(a, b, rounds);
        }
        else if (rounds <= MAX_ROUNDS) {
            p2 a = {40, 20};
            p2 b = {256,10};
            return eval_line_between(a, b, rounds);
        }
        else if (rounds > MAX_ROUNDS) judge_error("Let more than MAX_ROUNDS slip past");
        else judge_error("Impossible rounds");
    };

    int score = scoring_function(author_rounds);
    if (score < 0 || score > 100) judge_error("Invalid score given");
    accept_with_score(score);
}
