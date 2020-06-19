#include<bits/stdc++.h>
using namespace std;

struct procedure {
    int type;
    string S;
    int i, j;
    void init(string _S) { type = 1; S = _S; }
    void init(int _i, int _j) { type = 2; i = _i; j = _j; }
};

procedure makeProc(string S) { procedure tmp; tmp.init(S); return tmp; }
procedure makeProc(int i, int j) { procedure tmp; tmp.init(i,j); return tmp; }

void output(int yn, int n, struct procedure *c) {
    if (!yn) {
        printf("NO\n");
    } else {
        printf("YES\n");
        printf("%d\n", n);
        int k;
        for (k = 0; k < n; k++) {
            if (c[k].type == 1) {
                printf("%d %s\n", c[k].type, c[k].S.c_str());
            } else {
                printf("%d %d %d\n", c[k].type, c[k].i, c[k].j);
            }
        }
    }
    fflush(stdout);
}

unsigned int xseed[4];
void initXs(unsigned int seed) {
    for(int i=1; i<=4; i++){
        xseed[i-1] = seed = 1812433253U * (seed^(seed>>30)) + i;
    }
}
unsigned int xs() {
    unsigned int t;
    t=(xseed[0]^(xseed[0]<<11));
    xseed[0]=xseed[1];
    xseed[1]=xseed[2];
    xseed[2]=xseed[3];
    return xseed[3]=(xseed[3]^(xseed[3]>>19))^(t^(t>>8));
}



const int A2Z_NUM = 26;
const int MAX_T = 100000;
const int MAX_S = 200000;
const int MAX_N = 500000;


bool isNoCase(string s, string t);
void endecode(string s, string& t, vector<procedure>& decode);
void shfl(string& s,  vector<procedure>& shfl_proc);
void solver(string s, string t, vector<procedure>& proc);

long long calcCost(vector<procedure> proc);

int main(){
    clock_t start = clock();
    initXs(time(NULL));
    string s,t; cin >> s >> t;
    if(isNoCase(s,t)) {
        output(0,0,NULL);
        return 0;
    }
    vector<procedure> decode, proc;
    endecode(s,t,decode);
    long long now_min_cost = LONG_LONG_MAX;

    while((clock()-start) < (unsigned long)(9.0*CLOCKS_PER_SEC))
    {
        vector<procedure> tmp_proc;
        string shfls = s;

        shfl(shfls, tmp_proc);
        solver(shfls,t,tmp_proc);

        long long cost = calcCost(tmp_proc);
        if (cost < now_min_cost) {
            now_min_cost = cost;
            proc = tmp_proc;
        }
    }

    int n=0;
    procedure *_proc; _proc = new procedure[MAX_N];
    for(const auto& i : proc) _proc[n] = i, ++n;
    for(const auto& i : decode) _proc[n] = i, ++n;

    output(1,n,_proc);

    delete[] _proc;
}

bool isNoCase(string s, string t) {
    int scnt[A2Z_NUM] = {};
    for(auto i : s) scnt[i-'a']++;

    bool dp[MAX_T+1] = {true};
    for(int i=0; i<A2Z_NUM; ++i) {
        int ith_scnt = scnt[i];
        for(int j=ith_scnt; j<=MAX_T; ++j) {
            dp[j] |= dp[j-ith_scnt];
        }
    }

    int tcnt[A2Z_NUM] = {};
    for(auto i : t) tcnt[i-'a']++;

    bool is_NO_case = false;
    for(int i=0; i<A2Z_NUM; ++i) is_NO_case |= !dp[tcnt[i]];
    return is_NO_case;
}

void endecode(string s, string& t, vector<procedure>& decode) {
    int scnt[A2Z_NUM] = {}, tcnt[A2Z_NUM] = {};
    for(auto i : t) tcnt[i-'a']++;
    for(auto i : s) scnt[i-'a']++;

    vector<bool> dp(MAX_T+1);
    dp[0] = true;
    for(int i=0; i<A2Z_NUM; ++i) {
        int ith_scnt = scnt[i];
        for(int j=ith_scnt; j<=MAX_T; ++j) {
            dp[j] = (dp[j] | dp[j-ith_scnt]);
        }
    }

    auto repeat = [](int n, char c) { string result; for(int i=0; i<n; ++i) result.push_back(c); return result; };
    string minT;
    for(int i=0; i<A2Z_NUM; ++i) {
        if (!tcnt[i]) continue;
        for(int j=1; j<=tcnt[i]; ++j) {
            if (tcnt[i]%j == 0 && dp[j]) {
                minT.append(repeat(j,(char)(i+'a')));
                if (tcnt[i]/j != 1) decode.push_back(makeProc(repeat(tcnt[i]/j+1, (char)(i+'a'))));
                break;
            }
        }
    }
    t = minT;
}

void shfl(string& s,  vector<procedure>& shfl_proc) {
    // proc size increase is about (n+1)*2*(A2Z_NUM/n) <= 78 (1<n<=26) or fewer.

    static int exec_num=0;
    ++exec_num;
    if (exec_num==1) return;
    else if (exec_num==2) {
        int tcnt[A2Z_NUM]={};
        for(const auto& i : s) tcnt[i-'a']++;

        vector<pair<int,int>> cnt__idx;
        int ith_char[A2Z_NUM]={},n=0;
        for(int i=0; i<A2Z_NUM; ++i) if (tcnt[i]) {
            cnt__idx.push_back(make_pair(tcnt[i], n));
            ith_char[n] = i;
            n++;
        }
        sort(cnt__idx.begin(), cnt__idx.end());
        int from[A2Z_NUM]={};
        for(int i=0; i<n; ++i) from[i] = -1;
        for(int i=0; i<n; ++i) from[i] = cnt__idx[i].second;
        bool pushed[A2Z_NUM] = {};


        for(int i=0;i<n; ++i) {
            if (pushed[i] || from[i] == -1 || from[i] == i) continue;
            int idx = i;
            char now = 'X';
            while(!pushed[idx]) {
                pushed[idx] = true;
                shfl_proc.push_back(makeProc({(char)(idx+'a'), now}));
                for(auto& j : s) if (j == (char)(idx+'a')) j = now;
                now = (char)(idx+'a');
                idx = from[idx];
            }
            shfl_proc.push_back(makeProc({'X', now}));
            for(auto& j : s) if (j == 'X') j = now;
        }
    }
    else {
        int tcnt[A2Z_NUM]={};
        for(const auto& i : s) tcnt[i-'a']++;

        vector<pair<int,int>> cnt__idx;
        int ith_char[A2Z_NUM]={},n=0;
        for(int i=0; i<A2Z_NUM; ++i) if (tcnt[i]) {
//                cnt__idx.push_back(make_pair(tcnt[i], n));
                cnt__idx.push_back(make_pair(xs()%100000, n));
                ith_char[n] = i;
                n++;
            }
        sort(cnt__idx.begin(), cnt__idx.end());
        int from[A2Z_NUM]={};
        for(int i=0; i<n; ++i) from[i] = -1;
        for(int i=0; i<n; ++i) from[i] = cnt__idx[i].second;
        bool pushed[A2Z_NUM] = {};


        for(int i=0;i<n; ++i) {
            if (pushed[i] || from[i] == -1 || from[i] == i) continue;
            int idx = i;
            char now = 'X';
            while(!pushed[idx]) {
                pushed[idx] = true;
                shfl_proc.push_back(makeProc({(char)(idx+'a'), now}));
                for(auto& j : s) if (j == (char)(idx+'a')) j = now;
                now = (char)(idx+'a');
                idx = from[idx];
            }
            shfl_proc.push_back(makeProc({'X', now}));
            for(auto& j : s) if (j == 'X') j = now;
        }
    }
}

long long calcCost(vector<procedure> proc) {
    long long cost = 0;
    for(const auto& i : proc) {
        if (i.type == 1) cost += i.S.size()+1;
        else cost += i.j - i.i;
    }
    return cost;
}


// ----- solver ----- //


vector<procedure> getSortingProcedure(string s);

vector<string> getLongAsgvEachChar(string s, string t);
char getCharWithLongestAsg(vector<string> asgv);
void deleteUnusedChar(string &s, vector<string> asgv, vector<procedure> &proc);
void makeTheCharPrev(char c, string &s, vector<procedure> &proc);
void execTheCharsAsgAndSorting(string &s, string asg, vector<procedure> &proc);
void execOtherCharsAsgAndSorting(char c, string &s, vector<string> asgv, vector<procedure> &proc);
void upper2lower(string t, vector<procedure> &proc);

int kr(char c, string s, vector<string> asgv, vector<procedure>& _proc);

void solver(string s, string t, vector<procedure>& proc) {
    const vector<string> asgv = getLongAsgvEachChar(s,t);
    const char c = getCharWithLongestAsg(asgv);

    deleteUnusedChar(s, asgv, proc);

    if (kr(c,s,asgv,proc)) {
        makeTheCharPrev(c, s, proc);
        execTheCharsAsgAndSorting(s, asgv[c-'a'], proc);
        execOtherCharsAsgAndSorting(c, s, asgv, proc);
    }

    upper2lower(t,proc);
}


vector<string> getLongAsgvEachChar(string s, string t) {
    vector<string> asgv(A2Z_NUM);

    int scnt[A2Z_NUM]={}, tcnt[A2Z_NUM]={};
    for(auto i : s) scnt[i-'a']++;
    for(auto i : t) tcnt[i-'a']++;

    vector<vector<int>> dp(MAX_T+1, vector<int>(A2Z_NUM+1, 0));
    dp[0][A2Z_NUM] = 1;

    for(int i=0; i<MAX_T; ++i) {
        for(int j=A2Z_NUM-1; j>=0; --j) {
            int cnt = scnt[j];
            if (!cnt || i < cnt || !dp[i-cnt][A2Z_NUM]) continue;
            for(int k=0; k<A2Z_NUM; ++k) {
                dp[i][k] = dp[i-cnt][k];
            }
            dp[i][j]++;
            dp[i][A2Z_NUM] = 1;
        }
    }
    for(int i=0; i<A2Z_NUM; ++i) if (scnt[i]) asgv[i].push_back((char)(i+'a'));
    for(int i=0; i<A2Z_NUM; ++i) {
        if (!tcnt[i]) continue;
        for(int j=0; j<A2Z_NUM; ++j) for(int k=0; k<dp[tcnt[i]][j]; ++k) asgv[j].push_back((char)(i+'A'));
    }
    return asgv;
}
char getCharWithLongestAsg(vector<string> asgv) {
    int longest_asgs_idx = 0;

    for(int i=0; i<asgv.size(); ++i) if (asgv[i].size() > asgv[longest_asgs_idx].size()) longest_asgs_idx = i;

    return asgv[longest_asgs_idx].front();
}
int kr(char c, string s, vector<string> asgv, vector<procedure>& _proc) {
    auto repeatc = [](int n, char c) {string result; for(int i=0; i<n; ++i) result.push_back(c); return result; };
    vector<procedure> proc;
    int cnt[A2Z_NUM]={}, ccnt=0;
    map<char,int> revasgm[A2Z_NUM];
    vector<char> canUse;
    for(const auto& i : s) cnt[i-'a']++;
    for(int i=0; i<A2Z_NUM; ++i) {
        if (asgv[i].size() <= 1) {
            canUse.push_back((char)(i+'a'));
            continue;
        }
        string tmp = asgv[i].substr(1);
        for(const auto& j : tmp) revasgm[tolower(j)-'a'][(char)(i+'a')]++;
    }
    ccnt = cnt[c-'a'];

    int now=0;
    for(char i='a'; i<='z'; ++i) {
        vector<char> tmpCanUse = canUse;
        for(char j = toupper(i); j<='Z'; ++j) tmpCanUse.push_back(j);
        vector<char> v;
        vector<pair<int,char>> vv;
        for(const auto& j : revasgm[i-'a']) if(j.second) v.push_back(j.first);

        if (v.size() > tmpCanUse.size()) return -1;
        {
            string os = s.substr(0,now);
            string S = s.substr(now);
            {
                vector<procedure> tmproc;
                for(auto&& j : S) if (revasgm[i-'a'][j]) j = '0';
                makeTheCharPrev('0', S, tmproc);
                for(const auto& j : tmproc) {
                    int I = j.i+now, J = j.j+now;
                    proc.push_back(makeProc(I,J));
                    swap(s[I-1], s[J-1]);
                }
            }
            {
                string tmpS;
                for(int j=0; j<S.size(); ++j) {
                    if (S[j]=='0') tmpS.push_back(toupper(i));
                    if (!asgv[s[j+now]-'a'].empty() && asgv[s[j+now]-'a'].back() != toupper(i)) tmpS.push_back(s[j+now]);
                }
                S = tmpS;
                for(const auto& j : v) {
                    if (!asgv[j-'a'].empty() && asgv[j-'a'].back() != toupper(i)) proc.push_back(makeProc({j, tmpCanUse.back(), j}));
                    else proc.push_back(makeProc({j,tmpCanUse.back()}));
                    vv.push_back(make_pair(revasgm[i-'a'][j], tmpCanUse.back()));
                    tmpCanUse.pop_back();
                }
            }
            {
                vector<procedure> tmproc;
                makeTheCharPrev(toupper(i), S, tmproc);
                for(const auto& j : tmproc) proc.push_back(makeProc(j.i+now, j.j+now));
                for(const auto& j : vv) if (j.second == toupper(i) && j.first != 1) proc.push_back(makeProc((string){j.second} + repeatc(j.first, toupper(i))));
                for(const auto& j : vv) if (j.second != toupper(i)) proc.push_back(makeProc((string){j.second} + repeatc(j.first, toupper(i))));
            }
            {
                int tmpc = [&v, &cnt](){int cnt1=0; for(const auto& i : v) cnt1 += cnt[i-'a']; return cnt1; }();
                S = S.substr(tmpc);
                int tmpcc = [&revasgm, &cnt, i]() {int cnt1=0; for(const auto& j : revasgm[i-'a']) cnt1 += j.second*cnt[j.first-'a']; return cnt1; }();
                now += tmpcc;
                S = repeatc(tmpcc, toupper(i)) + S;

            }
            s = os+S;
        }

        for(int j=0; j<A2Z_NUM; ++j) if (!asgv[j-'a'].empty() && asgv[j].back() == toupper(i)) canUse.push_back((char)(j+'a'));
    }




    for(const auto& i : proc) _proc.push_back(i);
    return 0;
}

void makeTheCharPrev(char c, string &s, vector<procedure> &proc) {
    int cnt=0;
    deque<int> csidxes;
    for(int i=0; i<s.size(); ++i) if (s[i] == c) cnt++, csidxes.push_back(i);
    while(!csidxes.empty() && csidxes.front() < cnt) csidxes.pop_front();
    for(int i=0; i<cnt; ++i) {
        if (s[i] != c) {
            int csidx = csidxes.front(); csidxes.pop_front();
            proc.push_back(makeProc(i+1, csidx+1));
            swap(s[i], s[csidx]);
        }
    }
}
void deleteUnusedChar(string &s, vector<string> asgv, vector<procedure> &proc) {
    for(int i=0; i<A2Z_NUM; ++i) if (asgv[i].size() == 1) proc.push_back(makeProc({(char)(i+'a')}));
    string tmps;
    for(auto i : s) if (asgv[i-'a'].size() > 1) tmps.push_back(i);
    s = tmps;
}
void execTheCharsAsgAndSorting(string &s, string asg, vector<procedure> &proc) {
    // proc size increase is ((ccnt>>1)+1)*number_of_alphabet_types_in_s < MAX_T/2
    char c = asg.front();
    string S = asg.substr(1);


    string minS = [=]() { string minS; set<char> st; for (auto i : S) st.insert(i); for (auto i : st) minS.push_back(i); return minS; }();
    int ccnt = [=] () {int cnt=0; for(auto i : s) if (i==c) cnt++; return cnt; }();


    if (minS.size() <= 1 || ccnt == 1) proc.push_back(makeProc(asg));
    else {
        for(int i=0; i<minS.size()-1; ++i) {
            if (i == minS.size()-2) proc.push_back(makeProc({c,minS[i],minS[i+1]}));
            else proc.push_back(makeProc({c,minS[i],c}));
            int l=2+i*ccnt, r=(i+2)*ccnt-1;
            while (l<r) {
                proc.push_back(makeProc(l,r));
                l += 2;
                r -= 2;
            }
        }

        int Scnt[A2Z_NUM] = {}, minScnt[A2Z_NUM] = {};
        for(auto i : S) Scnt[tolower(i)-'a']++;
        for(auto i : minS) minScnt[tolower(i)-'a']++;

        auto repeatc = [](int n, char c) {string result; for(int i=0; i<n; ++i) result.push_back(c); return result; };
        for(int i=0; i<A2Z_NUM; ++i) if (Scnt[i] != minScnt[i]) proc.push_back(makeProc(repeatc(Scnt[i]+1, (i+'A'))));
    }
    string tmps;
    for(auto i : s) if (i != c) tmps.push_back(i);
    s = [=](){ string result; for(int i=0; i<ccnt; ++i) result += S; return result; }();
    sort(s.begin(), s.end());
    s += tmps;
}
void execOtherCharsAsgAndSorting(char c, string &s, vector<string> asgv, vector<procedure> &proc) {
    for(auto asg : asgv) {
        if (asg.size() < 2 || asg.front() == c) continue;
        char ci = asg.front();
        string S = asg.substr(1);
        string tmps;

        for(auto i : s) {
            if (i == ci) tmps += S;
            else tmps.push_back(i);
        }
        s = tmps;

        proc.push_back(makeProc(asg));
    }
    vector<procedure> sorting_proc = getSortingProcedure(s);
    sort(s.begin(), s.end());
    copy(sorting_proc.begin(), sorting_proc.end(), back_inserter(proc));
}
void upper2lower(string t, vector<procedure> &proc) {
    int cnt[A2Z_NUM]={};
    for(auto i : t) cnt[i-'a']++;
    for(int i=0; i<A2Z_NUM; ++i) if (cnt[i]) proc.push_back(makeProc({(char)(i+'A'), (char)(i+'a')}));
}


// ----- sort ----- //

vector<procedure> lsort(string s);
vector<procedure> rsort(string s);

vector<procedure> getSortingProcedure(string s) {
    // maximum increase in proc size is |S|*2, but usually about |S|
    for(auto& i : s) i = tolower(i);
    vector<procedure> lv,rv;
    lv = lsort(s);
    rv = rsort(s);
    int lcost=0, rcost=0;
    for(auto i : lv) lcost += i.j-i.i;
    for(auto i : rv) rcost += i.j-i.i;
    if (lcost == rcost) return (lv.size() < rv.size() ? lv : rv);
    else return (lcost < rcost ? lv : rv);
}

vector<procedure> lsort(string s) {
    vector<procedure> result;
    priority_queue<int, vector<int>, greater<int>> next_idx[A2Z_NUM];
    string t = s; sort(t.begin(), t.end(), [](char l, char r){ return l < r; });
    int cnt[A2Z_NUM]={}, end_point[A2Z_NUM]={};
    vector<bool> alreadyOK(s.size(), false);
    for(int i=0; i<s.size(); ++i) {
        if (s[i] == t[i]) alreadyOK[i] = true;
        else next_idx[s[i]-'a'].push(i);
        cnt[s[i]-'a']++;
    }
    for(int i=0; i<A2Z_NUM; ++i) end_point[i] = cnt[i] + (i ? end_point[i-1] : -1);
    for(int i=0; i<s.size(); ++i) {
        char si = s[i], ti = t[i];
        if (alreadyOK[i]) continue;
        if (ti == si) {
            next_idx[si-'a'].pop();
            continue;
        }
        int j = next_idx[ti-'a'].top();
        char sj = s[j], tj = t[j];
        if (si >= tj) {
            swap(s[i], s[j]);
            result.push_back(makeProc(i+1,j+1));
            next_idx[si-'a'].pop();
            next_idx[sj-'a'].pop();
            next_idx[si-'a'].push(j);
        } else {
            bool found = false;
            for(int k=end_point[si-'a']; k>i; --k) {
                if (alreadyOK[k]) continue;
                int sk = s[k];
                if (sk >= tj) {
                    found = true;
                    swap(s[k], s[j]);
                    swap(s[i], s[k]);
                    result.push_back(makeProc(k+1,j+1));
                    result.push_back(makeProc(i+1,k+1));
                    next_idx[si-'a'].pop();
                    next_idx[sk-'a'].pop();
                    next_idx[sj-'a'].pop();
                    next_idx[si-'a'].push(k);
                    next_idx[sk-'a'].push(j);
                    break;
                }
            }
            if (!found) {
                swap(s[i], s[j]);
                result.push_back(makeProc(i+1,j+1));
                next_idx[si-'a'].pop();
                next_idx[sj-'a'].pop();
                next_idx[si-'a'].push(j);
            }
        }
    }
    return result;
}

vector<procedure> rsort(string s) {
    vector<procedure> result;
    priority_queue<int> prev_idx[A2Z_NUM];
    string t = s; sort(t.begin(), t.end(), [](char l, char r){ return l < r; });
    int cnt[A2Z_NUM]={}, start_point[A2Z_NUM]={};
    vector<bool> alreadyOK(s.size(), false);
    for(int i=0; i<s.size(); ++i) {
        if (s[i] == t[i]) alreadyOK[i] = true;
        else prev_idx[s[i]-'a'].push(i);
        cnt[s[i]-'a']++;
    }
    for(int i=1; i<A2Z_NUM; ++i) start_point[i] = start_point[i-1] + cnt[i-1];
    for(int j=s.size()-1; j>=0; --j) {
        char sj = s[j], tj = t[j];
        if (alreadyOK[j]) continue;
        if (tj == sj) {
            prev_idx[sj-'a'].pop();
            continue;
        }
        int i = prev_idx[tj-'a'].top();
        char si = s[i], ti = t[i];
        if (ti >= sj) {
            swap(s[i], s[j]);
            result.push_back(makeProc(i+1,j+1));
            prev_idx[si-'a'].pop();
            prev_idx[sj-'a'].pop();
            prev_idx[sj-'a'].push(i);
        } else {
            bool found = false;
            for(int k=start_point[sj-'a']; k<j; ++k) {
                if (alreadyOK[k]) continue;
                int sk = s[k];
                if (ti >= sk) {
                    found = true;
                    swap(s[i], s[k]);
                    swap(s[k], s[j]);
                    result.push_back(makeProc(i+1,k+1));
                    result.push_back(makeProc(k+1,j+1));
                    prev_idx[si-'a'].pop();
                    prev_idx[sk-'a'].pop();
                    prev_idx[sj-'a'].pop();
                    prev_idx[sj-'a'].push(k);
                    prev_idx[sk-'a'].push(i);
                    break;
                }
            }
            if (!found) {
                swap(s[i], s[j]);
                result.push_back(makeProc(i+1,j+1));
                prev_idx[si-'a'].pop();
                prev_idx[sj-'a'].pop();
                prev_idx[sj-'a'].push(i);
            }
        }
    }
    return result;
}


