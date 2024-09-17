#define _CRT_SECURE_NO_WARNINGS
#include "myheader.h"
int arg;//cnf中变元个数，变元英文argument
int vac;//空位个数，空位英文vacancy
int main() {
	display();
	string filename;//文件名
	int command = 0;//用户的数字指令
	cin >> command;
	while (command) {
		if (command == 1) {//cnf求解
			printf("请输入后缀为.cnf的文件名：\n");
			cin >> filename;//此处用c语言读入只能存放在数组中，不方便
			head* R = cnfparser(filename);
			if (!R) {//尽量提高容错性，输入文件名错误后还可以继续交互
				cout << endl;
				display();
				cin >> command;
				continue;
			}
			int fakeboard[9][9];//此处的二维数组并无实际用处，只是为了保证cnfsolver函数传参的一致性
			cnfsolver(R, arg, command, filename, fakeboard);
		}

		else if (command == 2) {//数独问题
			printf("请输入数独初始数字个数（17—80）：\n");
			int initial;//数独初始数字个数
			cin >> initial;
			vac = 81 - initial;
			int board[9][9] = { 0 };
			filename = board2cnf(vac, board);
			head* R = cnfparser(filename);
			if (!R) {
				printf("错误！\n");
				display();
				cin >> command;
				continue;
			}
			cnfsolver(R, 10000, command, filename, board);
		}
		else if (command == 3) {//正确性检验
			printf("请输入后缀为.cnf的文件名：\n");
			cin >> filename;
			int result = check(filename);
			if (result == -1) {
				cout << endl;
				display();
				cin >> command;
				continue;
			}
			else if (result == 1)
				printf("输出正确\n");
			else printf("输出错误\n");
		}
		display();
		cin >> command;
	}
}

void display() {
	printf("欢迎体验SAT问题，请输入指令数字：\n");
	printf("    1——cnf求解\n");
	printf("    2——数独问题\n");
	printf("    0——退出程序\n");
}