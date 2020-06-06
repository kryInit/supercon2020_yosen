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
    void init_type1(string _S) { type = 1; S = _S; }
    void init_type2(int _i, int _j) { type = 2; i = _i; j = _j; }
};

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
void simplySolve(string s, string t, int& n, procedure *c);
vector<procedure> getSortingProcedure(string s);
vector<procedure> lsort(string s);
vector<procedure> rsort(string s);


procedure proc[MAX_N];

int main() {
    string s,t;
    cin >> s >> t;
    if(isNoCase(s,t)) {
        output(0,0,NULL);
        return 0;
    }
    int n=0;
    simplySolve(s,t,n,proc);
//    check(s,t,n,proc);
    output(1,n,proc);
}

void simplySolve(string s, string t, int& n, procedure *c) {
    vector<procedure> v;
    int scnt[A2Z_NUM]={}, tcnt[A2Z_NUM]={};
    for(auto i : s) scnt[i-'a']++;
    for(auto i : t) tcnt[i-'a']++;
    vector<vector<int>> dp(MAX_T+1, vector<int>(A2Z_NUM+1, 0));
    dp[0][A2Z_NUM] = 1;
    for(int i=0; i<A2Z_NUM; ++i) {
        int ith_scnt = scnt[i];
        if (!ith_scnt) continue;
        for(int j=ith_scnt; j<=MAX_T; ++j) {
            if (!dp[j-ith_scnt][A2Z_NUM]) continue;
            for(int k=0; k<A2Z_NUM; ++k) {
                dp[j][k] = dp[j-ith_scnt][k];
            }
            dp[j][i]++;
            dp[j][A2Z_NUM] = 1;
        }
    }
    string S[26];
    for(int i=0; i<A2Z_NUM; ++i) S[i].push_back((char)(i+'a'));
    for(int i=0; i<A2Z_NUM; ++i) {
        if (!tcnt[i]) continue;
        for(int j=0; j<A2Z_NUM; ++j) for(int k=0; k<dp[tcnt[i]][j]; ++k) S[j].push_back((char)(i+'A'));
    }
    for(int i=0; i<A2Z_NUM; ++i) {
        if (!scnt[i]) continue;
        procedure tmp;
        tmp.init_type1(S[i]);
        v.push_back(tmp);
        char c = S[i].front();
        string ss = S[i].substr(1);
        string tmps;
        for(auto j : s) {
            if (j == c) tmps += ss;
            else tmps.push_back(j);
        }
        s = tmps;
    }
    {
        vector<procedure> tmpv = getSortingProcedure(s);
        for(auto i : tmpv) v.push_back(i);
        sort(s.begin(), s.end(), [](char l, char r){ return tolower(l) < tolower(r); });
    }
    for(int i=0; i<A2Z_NUM; ++i) if (tcnt[i]) {
            procedure tmp;
            string tmps;
            tmps.push_back((char)(i+'A'));
            tmps.push_back((char)(i+'a'));
            tmp.init_type1(tmps);
            v.push_back(tmp);
        }
    n = v.size();
    for(int i=0; i<n; ++i) *(c+i) = v[i];
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
    int end_point[A2Z_NUM] = {};
    for(int i=0; i<s.size(); ++i) next_idx[tolower(s[i])-'a'].push(i);
    for(int i=0; i<A2Z_NUM; ++i) end_point[i] = next_idx[i].size() + (i ? end_point[i-1] : -1);
    for(int i=0; i<s.size(); ++i) {
        char si = tolower(s[i]), ti = tolower(t[i]);
        if (ti == si) {
            next_idx[si-'a'].pop();
            continue;
        }
        int j = next_idx[ti-'a'].top();
        char sj = tolower(s[j]), tj = tolower(t[j]);
        if (si >= tj) {
            swap(s[i], s[j]);
            procedure tmp;
            tmp.init_type2(i+1,j+1);
            result.push_back(tmp);
            next_idx[si-'a'].pop();
            next_idx[sj-'a'].pop();
            next_idx[si-'a'].push(j);
        } else {
            bool found = false;
            for(int k=end_point[si-'a']; k>i; --k) {
                int sk = tolower(s[k]);
                if (sk >= tj) {
                    found = true;
                    swap(s[k], s[j]);
                    swap(s[i], s[k]);
                    procedure tmp;
                    tmp.init_type2(k+1,j+1);
                    result.push_back(tmp);
                    tmp.init_type2(i+1,k+1);
                    result.push_back(tmp);
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
                procedure tmp;
                tmp.init_type2(i+1,j+1);
                result.push_back(tmp);
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
    int start_point[A2Z_NUM] = {};
    for(int i=0; i<s.size(); ++i) prev_idx[tolower(s[i])-'a'].push(i);
    for(int i=1; i<A2Z_NUM; ++i) start_point[i] = start_point[i-1] + prev_idx[i-1].size();
    for(int j=s.size()-1; j>=0; --j) {
        char sj = tolower(s[j]), tj = tolower(t[j]);
        if (tj == sj) {
            prev_idx[sj-'a'].pop();
            continue;
        }
        int i = prev_idx[tj-'a'].top();
        char si = tolower(s[i]), ti = tolower(t[i]);
        if (ti >= sj) {
            swap(s[i], s[j]);
            procedure tmp;
            tmp.init_type2(i+1,j+1);
            result.push_back(tmp);
            prev_idx[si-'a'].pop();
            prev_idx[sj-'a'].pop();
            prev_idx[sj-'a'].push(i);
        } else {
            bool found = false;
            for(int k=start_point[sj-'a']; k<j; ++k) {
                int sk = tolower(s[k]);
                if (ti >= sk) {
                    found = true;
                    swap(s[i], s[k]);
                    swap(s[k], s[j]);
                    procedure tmp;
                    tmp.init_type2(i+1,k+1);
                    result.push_back(tmp);
                    tmp.init_type2(k+1,j+1);
                    result.push_back(tmp);
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
                procedure tmp;
                tmp.init_type2(i+1,j+1);
                result.push_back(tmp);
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
    int cost=0;
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
            cost += S.size()+1;
        }
        else {
            int i = proc->i, j = proc->j;
            swap(s[i-1], s[j-1]);
            cost += j-i;
        }
        if (s.size() > 200000) {
            cout << "size of s exceeds 200,000!" << endl;
            return;
        }
    }
    cout << "s: " << s << endl;
    cout << "t: " << t << endl;
    cout << "s == t: " << (s == t ? "true" : "false") << endl;
    cout << "cost  : " << cost << endl;
}
