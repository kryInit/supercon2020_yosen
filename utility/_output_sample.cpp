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
