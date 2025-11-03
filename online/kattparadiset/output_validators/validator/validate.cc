#include "validate.h"

#include <string>
#include <vector>
#include <map>
using namespace std;

#define rep(i, a, b) for(int i = a; i < (b); ++i)
#define all(x) begin(x), end(x)
#define sz(x) (int)(x).size()
#define debug(...) //ignore
typedef long long ll;
typedef pair<int, int> pii;
typedef vector<int> vi;
typedef long double ld;

int main(int argc, char **argv) {
    init_io(argc, argv);

    int n, k;
    judge_in >> n >> k;

    vector<string> names(k);
    vi cnts(k);
    int largest = 0;
    rep(i,0,k) {
        judge_in >> names[i] >> cnts[i];
        largest = max(largest, cnts[i]);
    }


    auto check = [&](istream& sol, feedback_function feedback){
        string firststring;
        string trailing;

        if (!(sol >> firststring)) feedback("Expected more output");

        if (firststring == "Nej") {
            if (sol >> trailing) feedback("Trailing output");

            if (2*largest <= n) feedback("Answer is not Nej.");

            return true;
        }
        else if (firststring == "Ja") {
            vector<string> ans(n);

            rep(i,0,n){
                if(!(sol >> ans[i])) feedback("Expected more output");
            }

            if (sol >> trailing) feedback("Trailing output");
            
            map<string,int> counter;
            rep(i,0,k) {
                counter[names[i]] = cnts[i];
            }

            // Check if it matches input
            rep(i,0,n) {
                if (counter.count(ans[i]) == 0) feedback("Answer contains name of color that is not in input.");
                counter[ans[i]]--;
            }

            rep(i,0,k) {
                if (counter[names[i]] != 0) feedback("Answer does not use all input beds.");
            }

            // Check if the ring has no consecutive two that are the same color
            rep(i,0,n) {
                if (ans[i] == ans[(i+1)%n]) {
                    string temp = (string) ("Answer contains two beds next to each other with the same color at index " + to_string(i) + ". The colors are " + ans[i] + " and " + ans[(i+1)%n] + ".");
                    feedback(temp.c_str());
                }
            }

            return true;
        }
        else {
            feedback("The output is neither Nej or Ja.");
        }
        return false;
    };

    bool judge_solved = check(judge_ans, judge_error);
    // technically redundant: the return false branch is unreachable
    assert(judge_solved);
    bool author_solved = check(author_out, wrong_answer);
    assert(author_solved);


    accept();
}
