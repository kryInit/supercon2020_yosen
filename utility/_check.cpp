// _output_sampleの方のprocedureを使っている

const int MAX_N = 500000;
const int MAX_S = 200000;

void check(string s, string t, int n, procedure *c) {
    if (n > MAX_N) {
        cout << "n is too large!" << endl;
        return;
    }
    for(procedure *proc = c; proc < (c+n); ++proc) {
        int type = proc->type;
        if (type == 1) {
            char c = proc->S[0];
            string S = proc->S.substr(1);
            string tmps;
            for(auto i : s) {
                if (i == c) tmps += S;
                else tmps.push_back(i);
            }
            s = tmps;
        }
        else {
            int i = proc->i, j = proc->j;
            swap(s[i-1], s[j-1]);
        }
        if (s.size() > MAX_S) {
            cout << "size of s exceeds 200,000!" << endl;
            return;
        }
    }
    cout << "s: " << s << endl;
    cout << "t: " << t << endl;
    cout << "s == t: " << (s == t ? "true" : "false") << endl;
}
