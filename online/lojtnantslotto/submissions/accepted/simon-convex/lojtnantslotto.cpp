#include <iostream>
#include <vector>
#include <set>
#include <unordered_map>
#include <stack>
#include <optional>
#include <algorithm>
#include <queue>
#include <cctype>
#include <string>
#include <math.h>
#include <cstdint>
#include <climits>
#include <map>
#define ll long long
#define int ll
#define endl "\n"
using namespace std;

struct convexHullVal{
    int startval;
    int starti;
    int l;
    int r;
    int mini;
    int maxi;
    int evaluate(int i,vector<int>& pref){
        if(i + starti == 0) return startval;
        return pref[i+starti-1] + startval; 
    }
};
int getl(convexHullVal& a, convexHullVal& b,vector<int>& pref){
    int l = b.mini-1;
    int r = a.maxi+1;
    //l är den som b är större än a på
    //r är den som a är större än b på
    while(l +1 < r){
        int mid = (l+r)/2;
        if(a.evaluate(mid, pref) < b.evaluate(mid,pref)) l = mid;
        else r = mid;
    }
    return r;
}
struct convexHull{
    vector<int>& pref;
    //first = 
    deque<convexHullVal> daque;
    convexHull(vector<int>& prefarr) : pref(prefarr) {};
    void insert(pair<int,int> val){
        convexHullVal cur;
        cur.startval = val.first;
        cur.starti = val.second;
        cur.mini = -cur.starti;
        cur.maxi = pref.size() - cur.starti;
        cur.l = cur.mini;
        cur.r = cur.maxi+1;
        
        while(!daque.empty()){
            int intersection = getl(daque.back(),cur,pref);
            if(intersection > daque.back().l){
                daque.back().r = intersection;
                cur.l = intersection;
                break;
            }
            daque.pop_back();
        }
        daque.push_back(cur);
    }
    int getPos(int x){
        while(daque.size() > 1){
            if(x >= daque.front().r){
                daque.pop_front();
            }
            else break;
        }
        //cout << daque.front().starti << " " << daque.front().startval << endl;
        return daque.front().evaluate(x,pref);
    }
};
vector<int> transformConvex(vector<int> answers, vector<int>& prefarr, int level){
    for(int startPos = 0; startPos < level; startPos++){
        convexHull curHull(prefarr);
        for(int i = 0; i*level + startPos < answers.size(); i++){
            int pos = i*level + startPos;
            curHull.insert({answers[pos], -i});
            answers[pos] = curHull.getPos(i);
        }
    }
    return answers;
}
signed main(){
    vector<vector<int>> levels(5);
    int n, t;
    cin >> n >> t;
    for(int i = 0; i< n; i++){
        int l, g;
        cin >> l >> g;
        levels[l-1].push_back(g);
    }
    for(int i = 0; i< levels.size(); i++){
        sort(levels[i].begin(),levels[i].end());
        for(int j = 1; j< levels[i].size(); j++){
            levels[i][j] += levels[i][j-1];
        }
    }
    vector<int> nrSoildersForAnswer(5*n+1,t+1);
    nrSoildersForAnswer[0] = 0;
    for(int i = 0; i< levels.size(); i++){
        nrSoildersForAnswer = transformConvex(nrSoildersForAnswer,levels[i],i+1);
        for(auto num : nrSoildersForAnswer){
            //cout << num << " ";
        }
        //cout << endl;
    }
    for(int i = nrSoildersForAnswer.size()-1; i>= 0; i--){
        if(nrSoildersForAnswer[i] <= t){
            cout << i << endl;
            return 0;
        }
    }
    ////cout << "none found" << endl;
    return 0;
}