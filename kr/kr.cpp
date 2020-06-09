#include<bits/stdc++.h>
using namespace std;


/**
 * 出力の各行に対応する操作の情報を表す。
 * type = 1 のとき、代入を表す。
 * この時、 S は代入の情報を表す。
 * type = 2 のとき、入れ替えを表す。
 * この時、 i 番目の文字と j 番目の文字を入れ替えることを表す。
 */
struct procedure {
    int type;
    string S;
    int i, j;
    void init(string _S) { type = 1; S = _S; }
    void init(int _i, int _j) { type = 2; i = _i; j = _j; }
};

procedure makeProc(string S) { procedure tmp; tmp.init(S); return tmp; }
procedure makeProc(int i, int j) { procedure tmp; tmp.init(i,j); return tmp; }

/**
 * 答えが YES のとき yn = 1、 NO のとき yn = 0 で呼び出す。
 * n は操作の回数を表す。
 * c は操作の情報を持った procedure 型の配列。
 */
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


const int TYPE1 = 1;
const int TYPE2 = 2;
const int A2Z_NUM = 26;
const int MAX_T = 100000;
const int MAX_S = 200000;
const int MAX_N = 500000;

bool isNoCase(string s, string t);
void check(string s, string t, int n, procedure *c);
void check(string s, string t, int n, vector<procedure> c);
void print(vector<procedure> proc) {
    for(auto i : proc) {
        if (i.type == 1) cout << 1 << " " << i.S << endl;
        else cout << 2 << " "<< i.i << " " << i.j << endl;
    }
}

vector<procedure> getSortingProcedure(string s);
vector<procedure> lsort(string s);
vector<procedure> rsort(string s);


void solver(string s, string t, int &n, procedure *procp);
string getReducedT(string s, string t);
vector<string> getLongAsgvEachChar(string s, string t);
char getCharWithLongestAsg(vector<string> asgv);
void deleteUnusedChar(string &s, vector<string> asgv, vector<procedure> &proc);
void makeTheCharAsgPrev(char c, string &s, vector<procedure> &proc);
void executeTheCharsAsgAndSorting(string &s, string asg, vector<procedure> &proc);
void executeOtherCharsAsgAndSorting(char c, string &s, vector<string> asgv, vector<procedure> &proc);
void upper2lower(string s, vector<procedure> &proc);
void decodeT(string mint, string t, vector<procedure> &proc);

int main() {
    int n=0;
    procedure *proc; proc = new procedure[MAX_N];
    string s,t; cin >> s >> t;
    if(isNoCase(s,t)) {
        output(0,0,NULL);
        return 0;
    }
    solver(s,t,n,proc);
//    check(s,t,n,proc);
    output(1,n,proc);
    delete[] proc;
}

void solver(string s, string t, int &n, procedure *procp) {
    vector<procedure> proc;
    string mint = getReducedT(s,t);
    vector<string> asgv = getLongAsgvEachChar(s,mint);
    char c = getCharWithLongestAsg(asgv);


    deleteUnusedChar(s, asgv, proc);
    makeTheCharAsgPrev(c, s, proc);


    executeTheCharsAsgAndSorting(s, asgv[c-'a'], proc);
    executeOtherCharsAsgAndSorting(c, s, asgv, proc);

    upper2lower(s,proc);
    decodeT(mint,t,proc);
    n = proc.size();
    for(int i=0; i<n; ++i) *(procp+i) = proc[i];
}

string getReducedT(string s, string t) {
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

    string mint;
    for(int i=0; i<A2Z_NUM; ++i) {
        if (!tcnt[i]) continue;
        for(int j=1; j<=tcnt[i]; ++j) {
            if (tcnt[i]%j == 0 && dp[j]) {
                for(int k=0; k<j; ++k) mint.push_back(i+'a');
                break;
            }
        }
    }
    return mint;
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
void makeTheCharAsgPrev(char c, string &s, vector<procedure> &proc) {
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
void executeTheCharsAsgAndSorting(string &s, string asg, vector<procedure> &proc) {
    // proc size increase is ((ccnt>>1)+1)*number_of_alphabet_types_in_s < MAX_N/2
    char c = asg.front();
    string S = asg.substr(1);


    string minS = [=]() { string minS; set<char> st; for (auto i : S) st.insert(i); for (auto i : st) minS.push_back(i); return minS; }();
    int ccnt = [=] () {int cnt=0; for(auto i : s) if (i==c) cnt++; return cnt; }();


    if (minS.size() <= 1) {
        proc.push_back(makeProc(minS));
        for(int i=0; i<s.size(); ++i) if (s[i] == c) s[i] = minS.front();
    } else {
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
    }
    {
        string tmps;
        for(auto i : s) if (i != c) tmps.push_back(i);
        s = [=](){ string result; for(int i=0; i<ccnt; ++i) result += S; return result; }();
        sort(s.begin(), s.end());
        s += tmps;
    }

    int Scnt[A2Z_NUM] = {}, minScnt[A2Z_NUM] = {};
    for(auto i : S) Scnt[tolower(i)-'a']++;
    for(auto i : minS) minScnt[tolower(i)-'a']++;

    auto repeatc = [](int n, char c) {string result; for(int i=0; i<n; ++i) result.push_back(c); return result; };
    for(int i=0; i<A2Z_NUM; ++i) if (Scnt[i] != minScnt[i]) proc.push_back(makeProc(repeatc(Scnt[i]+1, (i+'A'))));
}
void executeOtherCharsAsgAndSorting(char c, string &s, vector<string> asgv, vector<procedure> &proc) {
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
void upper2lower(string s, vector<procedure> &proc) {
    set<char> st;
    for(auto i : s) if (isupper(i)) st.insert(i);
    for(auto i : st) proc.push_back(makeProc({i,(char)tolower(i)}));
}
void decodeT(string mint, string t, vector<procedure> &proc) {
    auto repeat = [](int n, char c) { string result; for(int i=0; i<n; ++i) result.push_back(c); return result; };
    int mintcnt[A2Z_NUM]={}, tcnt[A2Z_NUM]={};
    for(auto i : mint) mintcnt[i-'a']++;
    for(auto i : t) tcnt[i-'a']++;
    for(int i=0; i<A2Z_NUM; ++i) if (mintcnt[i] != tcnt[i]) proc.push_back(makeProc((repeat(tcnt[i]/mintcnt[i] + 1, i+'a'))));
}

vector<procedure> getSortingProcedure(string s) {
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
    string t = s; sort(t.begin(), t.end(), [](char l, char r){ return tolower(l) < tolower(r); });
    int cnt[A2Z_NUM]={}, end_point[A2Z_NUM]={};
    vector<bool> alreadyOK(s.size(), false);
    for(int i=0; i<s.size(); ++i) {
        if (tolower(s[i]) == tolower(t[i])) alreadyOK[i] = true;
        else next_idx[tolower(s[i])-'a'].push(i);
        cnt[tolower(s[i])-'a']++;
    }
    for(int i=0; i<A2Z_NUM; ++i) end_point[i] = cnt[i] + (i ? end_point[i-1] : -1);
    for(int i=0; i<s.size(); ++i) {
        char si = tolower(s[i]), ti = tolower(t[i]);
        if (alreadyOK[i]) continue;
        if (ti == si) {
            next_idx[si-'a'].pop();
            continue;
        }
        int j = next_idx[ti-'a'].top();
        char sj = tolower(s[j]), tj = tolower(t[j]);
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
                int sk = tolower(s[k]);
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
    string t = s; sort(t.begin(), t.end(), [](char l, char r){ return tolower(l) < tolower(r); });
    int cnt[A2Z_NUM]={}, start_point[A2Z_NUM]={};
    vector<bool> alreadyOK(s.size(), false);
    for(int i=0; i<s.size(); ++i) {
        if (tolower(s[i]) == tolower(t[i])) alreadyOK[i] = true;
        else prev_idx[tolower(s[i])-'a'].push(i);
        cnt[tolower(s[i])-'a']++;
    }
    for(int i=1; i<A2Z_NUM; ++i) start_point[i] = start_point[i-1] + cnt[i-1];
    for(int j=s.size()-1; j>=0; --j) {
        char sj = tolower(s[j]), tj = tolower(t[j]);
        if (alreadyOK[j]) continue;
        if (tj == sj) {
            prev_idx[sj-'a'].pop();
            continue;
        }
        int i = prev_idx[tj-'a'].top();
        char si = tolower(s[i]), ti = tolower(t[i]);
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
                int sk = tolower(s[k]);
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

void check(string s, string t, int n, procedure *c) {
    if (n > MAX_N) {
        cout << "n is too large!" << endl;
        return;
    }
    int cost=0,c1=0,c2=0;
    for(procedure *proc = c; proc < (c+n); ++proc) {
        int type = proc->type;
        if (type == 1) {
            char c = proc->S.front();
            string S = proc->S.substr(1);
            string tmps;
            for(auto i : s) {
                if (i == c) tmps += S;
                else tmps.push_back(i);
            }
            s = tmps;
            cost += proc->S.size();
            c1 += proc->S.size();
        }
        else {
            int i = proc->i, j = proc->j;
            swap(s[i-1], s[j-1]);
            cost += j-i;
            c2 += j-i;
        }
        if (s.size() > MAX_S) {
            cout << "size of s exceeds 200,000!" << endl;
            return;
        }
    }
    cout << "s: " << s << endl;
    cout << "t: " << t << endl;
    cout << "s == t: " << (s == t ? "true" : "false") << endl;
    cout << "cost  : " << cost << " (" << c1 << ", " << c2 << ")" << endl;

}
void check(string s, string t, int n, vector<procedure> c) {
    if (n > MAX_N) {
        cout << "n is too large!" << endl;
        return;
    }
    int cost=0,c1=0,c2=0;
    for(auto I : c) {
        int type = I.type;
        if (type == 1) {
            char c = I.S.front();
            string S = I.S.substr(1);
            string tmps;
            for(auto i : s) {
                if (i == c) tmps += S;
                else tmps.push_back(i);
            }
            s = tmps;
            cost += I.S.size();
            c1 += I.S.size();
        }
        else {
            int i = I.i, j = I.j;
            swap(s[i-1], s[j-1]);
            cost += j-i;
            c2 += j-i;
        }
        if (s.size() > MAX_S) {
            cout << "size of s exceeds 200,000!" << endl;
            return;
        }
    }
    cout << "s: " << s << endl;
    cout << "t: " << t << endl;
    cout << "s == t: " << (s == t ? "true" : "false") << endl;
    cout << "cost  : " << cost << " (" << c1 << ", " << c2 << ")" << endl;
}
