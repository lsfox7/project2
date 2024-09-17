#define _CRT_SECURE_NO_WARNINGS
#include "myheader.h"
extern int arg;

int DPLL(head* R, int* solutions) {
    //单子句规则：化简
	int singleclause;
	while (singleclause = issingleclauseexist(R)) {
		if (singleclause > 0)
			solutions[singleclause] = 1;
		else if (singleclause < 0)
			solutions[0 - singleclause] = -1;
		simplifycnf(R, singleclause);
		if (iscnfempty(R)) //cnf为空
			return 1;
		else if (isemptyclauseexist(R))//存在空子句
			return 0;
	}

    //分裂策略：赋值
	int nextarg = whatisnextplus(R);
	head* Rcopy = copy(R);
	if (DPLL(combine(R, nextarg), solutions))
		return 1;
	int x= DPLL(combine(Rcopy, 0 - nextarg), solutions);
	deletecopy(Rcopy);
	return x;
}

int DPLLplus(head* L, int* solutions) {//非递归
    stack<head*> S;
    head* ListL[10000] = { nullptr };//记录链表
    int i = 0;
    head* temL = nullptr;
    int v[10000];//记录赋值
    int val = 0;//结果
    S.push(L);
    while (!S.empty()) {
        while ((L = S.top()) && (val = singlespread(L, solutions)) == -1) {//单位传播
            temL = copy(L);
            ListL[i] = temL;
            v[i] = whatisnextplus(L);
            L = combine(L, v[i]);//分裂策略1
            S.push(L);
            i++;
        }
        if (val == 1)
            return 1;
        S.pop();i--;
        deletecopy(L);
        if (!S.empty()) {
            L = ListL[i];
            S.pop();
            S.push(combine(L, -v[i]));//分裂策略2
        }
    }
    return 0;
}

int issingleclauseexist(head* R) {//是否含有单子句
	head* pH = R->nextcol;
	while (pH) {//遍历链表
		if (pH->num == 1) 
            return pH->firstnode->value;
		pH = pH->nextcol;
	}
	return 0;
}
int isemptyclauseexist(head* R) {//判断范式中是否有空子句
    head* pH = R->nextcol;
    while (pH) {
        if (pH->num == 0) return 1;
        pH = pH->nextcol;
    }
    return 0;
}
int iscnfempty(head* R) {//L中存储的范式是否为空范式
    if (R->num)
        return 0;
    else
        return 1;
}

void simplifycnf(head* R, int singleclause) {
    head* pH = R->nextcol;
    head* fpH = R;
    int flag;
    while (pH) {
        node* p = pH->firstnode;
        node* fp = p;
        flag = 0;
        while (p) {
            flag = 0;
            if (p->value == singleclause) {//去掉这个子句（删除一行）
                fpH->nextcol = pH->nextcol;
                p = pH->firstnode;
                while (p) {
                    fp = p;
                    p = p->nextnode;
                    delete fp;
                }
                R->num--;
                flag = 1;
                break;//跳过这一行
            }
            else if (p->value == 0 - singleclause) {//去掉这个文字（删除一个结点）
                if (pH->firstnode == p) {
                    flag = 2;//第一个结点删除
                    pH->firstnode = p->nextnode;
                    pH->num--;
                }
                else {
                    flag = 3;
                    fp->nextnode = p->nextnode;
                    pH->num--;
                }
            }
            if (!flag) {//没有删除的节点
                if (fp != p) fp = p;
                p = p->nextnode;
            }
            else if (flag == 2) {//删除的为第一个结点
                p = pH->firstnode;
                fp = p;
            }
            else if (flag == 3) {//删除的不是第一个结点
                delete p;
                p = fp->nextnode;
            }
        }
        if (!flag) {
            fpH = pH;
            pH = pH->nextcol;
        }
        else if (flag == 1) {
            delete pH;
            pH = fpH->nextcol;
        }
    }
}
int singlespread(head* R, int* solutions) {//单位传播
    int singleclause;
    while (singleclause = issingleclauseexist(R)) {
        if (singleclause > 0)
            solutions[singleclause] = 1;
        else if (singleclause < 0)
            solutions[0 - singleclause] = -1;//例如：book[-(-2)] = -1表示2代表的变元取负值，即-2
        simplifycnf(R, singleclause);//根据单子句传播策略简化单子句
        if (iscnfempty(R))
            return 1;
        else if (isemptyclauseexist(R))
            return 0;
    }
    return -1;
}

head* copy(head* L) {//将L中全部内容都拷贝到temL中，函数结构与cnfparser函数基本相同
    head* temL = new head;
    temL->num = L->num;
    head* pH = L->nextcol;
    head* _pH = new head;
    temL->nextcol = _pH;
    _pH->nextcol = nullptr;
    for (int i = 1; i <= L->num; i++) {
        _pH->num = pH->num;
        int tep = pH->firstnode->value;
        node* p = pH->firstnode;
        node* _p = new node;
        _p->value = tep;
        _pH->firstnode = _p;
        _p->nextnode = nullptr;
        while (p->nextnode) {
            p = p->nextnode;
            tep = p->value;
            _p = new node;
            _p->value = tep;//采用头插法建立链表
            _p->nextnode = _pH->firstnode;
            _pH->firstnode = _p;
        }
        if (i != L->num) {
            _pH = new head;
            _pH->nextcol = temL->nextcol;
            temL->nextcol = _pH;
        }
        pH = pH->nextcol;
    }
    return temL;
}
void deletecopy(head* Rcopy) {//释放掉L里面的所有内存空间
    head* _pH = Rcopy->nextcol;
    while (_pH) {
        node* _p = _pH->firstnode;
        while (_p) {
            node* _fp = _p;
            _p = _p->nextnode;
            delete _fp;
        }
        head* _fpH = _pH;
        _pH = _pH->nextcol;
        delete _fpH;
    }
    delete Rcopy;
}

head* combine(head* R, int v) {//将v作为一个单子句合并到范式L中，将合并后的范式作为返回值
    head* pH = new head; 
    pH->num++;
    pH->nextcol = R->nextcol;
    R->nextcol = pH;
    node* p = new node; 
    p->value = v;
    pH->firstnode = p; 
    p->nextnode = nullptr;
    R->num++;
    return R;
}

int whatisnextplus(head* R) {//最短子句的首变元
    head* pH = R->nextcol;
    int min, ans = 0;
    if (!pH) { 
        cout << "范式为空集，无法选择文字返回" << endl; 
        return 0; 
    }
    min = pH->num; 
    ans = R->nextcol->firstnode->value;
    while (pH) {
        if (pH->num != 0 && pH->num < min) 
            ans = pH->firstnode->value;
        pH = pH->nextcol;
    }
    return ans;
}

void printList(head* R) {//仅用于测试使用
    head* _pH = R->nextcol;
    int i = 1;
    while (_pH) {
        cout << "第" << i++ << "行：" << _pH->num;
        node* _p = _pH->firstnode;
        while (_p) {
            cout << "->" << _p->value;
            _p = _p->nextnode;
        }
        cout << endl;
        _pH = _pH->nextcol;
    }
}