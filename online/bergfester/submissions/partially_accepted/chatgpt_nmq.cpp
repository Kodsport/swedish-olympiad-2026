#include <bits/stdc++.h>
using namespace std;

/*
Läser in:
N M Q
r_1 ... r_N    (rutschkanor: up[i] = r_i)
h_1 ... h_M    (person i bor i houseOf[i])
f_1 ... f_M    (cirkulär lista som beskriver cykeln i ordning)

T händelser:
T=1 a b   -> byt invånare i hus a och b
T=2 p r   -> fråga: person p är värd, rolighet r. Skriv max antal inbjudna.

Implementation:
- up[1..N]: hus->nästa hus nedåt
- occ[1..N]: person id som bor i huset (0 om tomt)
- houseOf[1..M]: husnummer där personen bor
- cycle[0..M-1] = f array (0-indexerat internt)
- posInCycle[person] = index i i cycle så att cycle[i]==person
*/

int main(){
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    int N,M,Q;
    if(!(cin>>N>>M>>Q)) return 0;
    vector<int> up(N+1);
    for(int i=1;i<=N;i++) cin>>up[i];
    vector<int> houseOf(M+1, 0);
    vector<int> occ(N+1, 0);
    for(int i=1;i<=M;i++){
        int h; cin>>h;
        houseOf[i]=h;
        occ[h]=i;
    }
    vector<int> cycle(M);
    for(int i=0;i<M;i++){
        cycle[i]=i+1;
    }
    vector<int> posInCycle(M+1);
    for(int i=0;i<M;i++) posInCycle[ cycle[i] ] = i;

    // Process Q events
    for(int qi=0; qi<Q; ++qi){
        int T; cin>>T;
        if(T==1){
            int a,b; cin>>a>>b;
            int pa = occ[a];
            int pb = occ[b];
            // swap occupants
            occ[a] = pb;
            occ[b] = pa;
            if(pa) houseOf[pa] = b;
            if(pb) houseOf[pb] = a;
        } else if(T==2){
            int p, r; cin>>p>>r;
            // reachers[h] = lista med personer som kan nå hus h inom r steg
            vector<vector<int>> reachers(N+1);
            // för varje person klättra upp till r steg och lägg personen i reachers[curHouse]
            for(int person=1; person<=M; ++person){
                int cur = houseOf[person];
                int steps = 0;
                while(true){
                    if(cur==0) break; // bör inte hända men säkerhetscheck
                    reachers[cur].push_back(person);
                    if(steps==r) break;
                    if(up[cur]==cur) break; // rot
                    cur = up[cur];
                    ++steps;
                }
            }

            int best = 1; // minst värden själv kan alltid bjudas (K>=1)
            int startIdx = posInCycle[p];

            // hjälparrays för att markera kan nå H
            vector<char> can(M+1, 0);

            for(int H=1; H<=N; ++H){
                // fyll can utifrån reachers[H]
                for(int person : reachers[H]) can[person] = 1;

                // framåt (i cykelriktning increasing index)
                int cnt = 0;
                for(int d=0; d<M; ++d){
                    int idx = (startIdx + d) % M;
                    int person = cycle[idx];
                    if(can[person]) cnt = d+1;
                    else break;
                }
                best = max(best, cnt);

                // bakåt (minskande index)
                cnt = 0;
                for(int d=0; d<M; ++d){
                    int idx = (startIdx - d) % M;
                    if(idx<0) idx += M;
                    int person = cycle[idx];
                    if(can[person]) cnt = d+1;
                    else break;
                }
                best = max(best, cnt);

                // reset can för de vi satte (för att inte O(M) clear varje gång — iterera reachers[H])
                for(int person : reachers[H]) can[person] = 0;
            }

            cout<<best<<"\n";
        }
    }
    return 0;
}
