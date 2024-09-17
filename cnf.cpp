#define _CRT_SECURE_NO_WARNINGS
#include "myheader.h"
extern int arg;
extern int vac;
extern int finalsudoku[9][9];
head* cnfparser(string &filename){//cnf文件to链表结构
	string in = filename;
	ifstream input(in);
	if (!input) {
		printf("文件打开错误！\n");
		return NULL;
	}
	char c;//用于存放cnf文件中的注释标志c
	char exp[105];//用于存放cnf文件中的注释（整行）,注释的英文explaination
	input >> c;//跳过第一个c
	while (c != 'p') {
		input.getline(exp, 100);//跳过注释
		input >> c;//跳过后续的c
	}
	string cnf;//存放“cnf”这三个字符
	int clause=0;
	input >> cnf >> arg >> clause;
	head* R = new head;
	R->num = clause;
	head* H = new head;
	R->nextcol = H;
	H->nextcol = nullptr;
	for (int i = 1; i <= clause; i++) {
		int tmp;//临时变量
		input >> tmp;
		node* N = new node;
		N->value = tmp;
		H->firstnode = N;
		H->num++;
		N->nextnode = nullptr;
		input >> tmp;
		while (tmp) {//头插法
			N = new node;
			N->value = tmp;
			N->nextnode = H->firstnode;
			H->firstnode = N;
			H->num++;
			input >> tmp;
		}
		if (i != clause ) {//下一行
			H = new head;
			H->nextcol = R->nextcol;
			R->nextcol = H;
		}
	}
	input.close();//文件关闭
	return R;
}

void cnfsolver(head* R, int arg, int command, string& filename, int board[9][9]) {
	string outfilename = filename.replace(filename.end() - 4, filename.end(), ".res");
	long starttime1= 0,starttime2=0,endtime1 = 0,endtime2=0,t1=0,t2=0;//测试时间
	bool issat;//是否可满足
	if (command == 1) {
		int* solutions = new int[arg];//解集
		memset(solutions, 0, sizeof(int) * arg);
		head* R2 = copy(R);
		starttime1 = clock();
		DPLL(R, solutions);
		endtime1 = clock();056
		t1 = endtime1 - starttime1;
		printf("DPLL部分运行时间为：%dms\n", t1);
		starttime2 = clock();
		issat=DPLLplus(R2, solutions);
		if (issat)
			printf("可满足\n");
		else printf("不可满足\n");
		endtime2 = clock();
		t2 = endtime2 - starttime2;
		printf("DPLLplus部分运行时间为：%dms\n", t2);
		double x = 1.0000*(t1 - t2) / t1;
		printf("优化率为%.2lf%%\n", x * 100);
		outfile(issat, t1, t2, solutions, arg, command, outfilename);
	}
	else if (command == 2) {
		int solutions[5000] = {0};
		issat = DPLL(R, solutions);
		printf("数独游戏开始，输入0终止游戏\n");
		while (vac) {
			printf("请输入您要填入的行数、列数、数字（例如xyz表示在第x行第y列填入数字z）:\n");
			int n;
			int x, y, z;
			cin >> n;
			if (!n) exit(0);
			x = n / 100 - 1;
			y = (n / 10) % 10 - 1;
			z = n % 10;
			if (solutions[n] > 0 && board[x][y] == 0) {
				board[x][y] = z;
				vac--;
				print(board);
				printf("填入成功，请填写下一个：\n");
			}
			else {
				printf("输入的数字不正确！\n");
				continue;
			}
		}
		printf("游戏结束，恭喜您成功过关！\n");
		outfile(issat, t1, t2, solutions, arg, command, outfilename);
	}
}

void outfile(bool issat, int t1,int t2, int* solutions, int arg, int command,string& outfilename) {//输出文件
	ofstream output(outfilename);
	if (!output) {
		printf("文件打开失败！\n");
		exit(0);
	}
	output << "s" << issat << endl;
	if (issat) {//如果结果正确
		output << "v";
		for (int i = 1, j = 1; i <= arg; i++) {
			output << " " << solutions[i] * i;
			j++;
			if (j == 10) {
				output << endl; 
				j = 1; 
			}
		}
		output << endl;
	}
	if (command == 1) output <<"t"<<" "<< t1 << " " << t2;
	output.close();//文件关闭
}
