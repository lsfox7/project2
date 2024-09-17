#define _CRT_SECURE_NO_WARNINGS
#include"myheader.h"
extern int arg;

int check(string& filename) {//验证根据DPLL算法生成的文件是否正确
    head* R = cnfparser(filename);
    if (!R)
        return -1;
    head* Rcopy = copy(R);
    int _FunNum = arg > 1000 ? arg : 1000;
    int* book = new int[_FunNum];
    memset(book, 0, sizeof(int) * _FunNum);

    int isTrue = DPLL(R, book);
    if (!isTrue) { 
        cout << ".cnf文件不可满足"; 
        exit(0); 
    }
    else {
        if (verification(Rcopy, book)) 
            return 1;
        else 
            return 0;
    }
}

int verification(head* Rcopy, int* solutions) {
    head* pH = Rcopy->nextcol;
    while (pH) {
        node* p = pH->firstnode;
        while (p) {
            if (solutions[abs(p->value)] * p->value > 0) 
                break;
            p = p->nextnode;
        }
        if (!p) 
            return 0;
        pH = pH->nextcol;
    }
    return 1;
}

