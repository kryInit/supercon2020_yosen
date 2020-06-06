#include<bits/stdc++.h>
using namespace std;

long long checkAndCalcCost(string filename_in, string filename_out);

int main(int argc, char *argv[]) {
    if (argc < 2) {
        cout << "few argument!" << endl;
        return 0;
    }
    string filename_in[10], filename_out[10];
    for(int i=1; i<=10; ++i) {
        stringstream ss;
        ss << setw(2) << setfill('0') << i;
        string filename = "02_random_" + ss.str();
        filename_in[i-1] = filename + ".in";
        filename_out[i-1] = filename + ".out";
    }

    system("./gen");

    int idx_min_cost_cmd = 0;
    long long min_cost = LONG_LONG_MAX;

    for(int k=1; k<argc; ++k) {
        string cmd = string(argv[1]);
        long long sum_cost = 0;
        
        cout << cmd << " is started!" << endl;

        for(int i=0; i<10; ++i) {
            string ith_cmd = "./" + cmd + " < " + filename_in[i] + " > " + filename_out[i];
            cout << " > " << ith_cmd << endl;
            system(ith_cmd.c_str());
            cout << "   ---- checking... ----" << endl;
            long long cost = 0;
            cost = checkAndCalcCost(filename_in[i], filename_out[i]);
            if (cost == -1) {
                cout << "we have a bad news...\n your proguram is something wrong." << endl;
                continue;
            }
            else cout << "OK!\ncost: " << cost << endl;
            cout << "   ---- finished! ----" << endl;
            sum_cost += cost;
        }
        cout << "sum cost: " << sum_cost << endl;
        cout << "\n\n";
        if (min_cost > sum_cost) {
            min_cost = sum_cost;
            idx_min_cost_cmd = k;
        }
    }
    if (idx_min_cost_cmd == 0) {
        cout << "all proguram is something wrong..." << endl;
    }
    else {
        cout << "minimum cost command is " + string(argv[idx_min_cost_cmd]) << endl;
        cout << "cost: " << min_cost << endl;
    }
}

const int MAX_N = 500000;
const int MAX_S = 200000;

long long checkAndCalcCost(string filename_in, string filename_out) {
    ifstream ifs_in(filename_in);
    ifstream ifs_out(filename_out);

    string s,t,yn;
    int n;
    ifs_in >> s >> t;
    ifs_out >> yn;
    if (yn == "NO") return 0;

    ifs_out >> n;

    if (n > MAX_N) {
        cout << "n is too large." << endl;
        return -1;
    }
    long long cost=0;
    for(int i=0; i<n; ++i) {
        int type;
        ifs_out >> type;
        if (type == 1) {
            string ss;
            ifs_out >> ss;
            char c = ss.front();
            string S = ss.substr(1);
            string tmps;
            for(auto i : s) {
                if (i == c) tmps += S;
                else tmps.push_back(i);
            }
            s = tmps;
            cost += S.size()+1;
        }
        else {
            int i,j;
            ifs_out >> i >> j;
            swap(s[i-1], s[j-1]);
            cost += j-i;
        }
        if (s.size() > 200000) {
            cout << "size of s exceeds 200,000." << endl;
            return - 1;
        }
    }
    if (s != t) {
        cout << "after operation s is not equal to t." << endl;
        return -1;
    }
    return cost;
}
