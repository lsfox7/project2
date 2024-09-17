#define _CRT_SECURE_NO_WARNINGS
#include "myheader.h"
static int T = 0;
string board2cnf(int holes, int board[9][9]) {
    int startsudoku[9][9] = { 0 };//数独初盘
    int finalsudoku[9][9] = { 0 };//数独终盘
    int i, j;//随机生成第一行
    srand((unsigned)time(nullptr));
    for (i = 0; i < 9; i++) {
        finalsudoku[0][i] = rand() % 9 + 1;
        j = 0;
        while (j < i) {//从头开始检查前面的数字有没有和新的数字一样的
            if (finalsudoku[0][i] == finalsudoku[0][j]) {//如果重复了就重新生成
                finalsudoku[0][i] = rand() % 9 + 1;
                j = 0;
            }
            else j++;//没重复，检查下一个
        }
    }
    create(finalsudoku, 1, 0);//递归生成后8行
    dig(finalsudoku, startsudoku, holes);//生成初盘
    memcpy(board, startsudoku, 9 * 9 * sizeof(int));
    printf("初始化后数独初盘为：\n");
    print(startsudoku);//输出初盘
    string filename = sudoku2cnf(startsudoku, holes);
    return filename;
}

int create(int a[][9], int i, int j) {//递归填充
    if (i < 9 && j < 9) {
        int x, y, k;
        int used[10] = { 0 };//用于排除已经使用过的的数字。0是没用过，1是用过。
        for (x = 0; x < i; x++)
            used[a[x][j]] = 1;//同列已使用的数字置为1
        for (x = 0; x < j; x++)
            used[a[i][x]] = 1;//同行已使用的数字置为1

        //同方块已使用的数字置为0
        for (x = i / 3 * 3; x <= i; x++) {//下边为空，不用检查
            if (x == i) {
                for (y = j / 3 * 3; y < j; y++)//右边为空，不用检查
                    used[a[x][y]] = 1;
            }
            else {
                for (y = j / 3 * 3; y < j / 3 * 3 + 3; y++)
                    used[a[x][y]] = 1;
            }
        }
        //对角线
        if (i == j) {
            for (x = 0; x < i; x++) {
                used[a[x][x]] = 1;
            }
        }
        if (i + j == 8) {
            for (x = 0; x < i; x++) {
                used[a[x][8 - x]]=1;
            }
        }

        int flag = 0;
        for (k = 1; k <= 9 && flag == 0; k++) {
            if (used[k] == 0) {
                flag = 1;
                a[i][j] = k;
                if (j == 8 && i != 8) {//递归生成
                    if (create(a, i + 1, 0) == 0)
                        return 0;
                    else flag = 0;
                }
                else if (j != 8) {//递归生成
                    if (create(a, i, j + 1) == 0)
                        return 0;
                    else flag = 0;
                }
            }
        }
        if (flag == 0) {//出错
            a[i][j] = 0;
            return 1;
        }
    }
    return 0;
}

void dig(const int a[][9], int b[][9], int holenum) {//随机生成初盘（也就是挖洞）
    int i, j, k;
    srand((unsigned)time(nullptr));
    for (i = 0; i < 9; i++)
        for (j = 0; j < 9; j++)
            b[i][j] = a[i][j];
    int** holes = new int* [holenum];
    for (int k = 0; k < holenum; k++) {
        holes[k] = new int[2];//一个放行序，一个放列序
    }
    int flag = 0;
    for (k = 0; k < holenum; k++) {
        i = rand() % 9;
        j = rand() % 9;
        flag = 0;
        for (int m = 0; m < k; m++) {//检查有没有和之前挖的洞重复
            if (i == holes[m][0] && j == holes[m][1])
                flag = 1;
        }
        if (flag == 0) {//没有重复，可以挖
            b[i][j] = 0;
            holes[k][0] = i;
            holes[k][1] = j;
        }
        else  i--;
    }
}

void print(const int a[][9]) {//打印数独数组
    int i, j;
    for (i = 0; i < 9; i++) {
        for (j = 0; j < 9; j++)
            printf("%d ", a[i][j]);
        cout << endl;
    }
}

string sudoku2cnf(int a[][9], int holes) {
    ofstream output("sudoku.cnf");//定义输入文件
    if (!output.is_open())
        cout << "can't open!\n";
    output << "p" << " " << "cnf" << " " << 729 << " " << 8829 + 81 - holes+72 << " " << endl;

    //单子句，xyz表示在第x行第y列填入数字z
    for (int x = 0; x < 9; ++x) {
        for (int y = 0; y < 9; ++y)
            if (a[x][y] != 0)
                output << (x + 1) * 100 + (y + 1) * 10 + a[x][y] << " " << 0 << endl;
    }

    //每个位置：1-9
    for (int x = 1; x <= 9; ++x) {
        for (int y = 1; y <= 9; ++y) {
            for (int z = 1; z <= 9; ++z)
                output << x * 100 + y * 10 + z << " ";
            output << 0;
            output << endl;
        }
    }
    //行
    for (int y = 1; y <= 9; ++y) {
        for (int z = 1; z <= 9; ++z)
            for (int x = 1; x <= 8; ++x)
                for (int i = x + 1; i <= 9; ++i)
                    output << 0 - (x * 100 + y * 10 + z) << " "<< 0 - (i * 100 + y * 10 + z) << " " << 0 << endl;
    }
    //列
    for (int x = 1; x <= 9; ++x) {
        for (int z = 1; z <= 9; ++z)
            for (int y = 1; y <= 8; ++y)
                for (int i = y + 1; i <= 9; ++i)
                    output << 0 - (x * 100 + y * 10 + z) << " "<< 0 - (x * 100 + i * 10 + z) << " " << 0 << endl;
    }
    //3*3 sub-grids
    for (int z = 1; z <= 9; ++z) {
        for (int i = 0; i <= 2; ++i)
            for (int j = 0; j <= 2; ++j)
                for (int x = 1; x <= 3; ++x)
                    for (int y = 1; y <= 3; ++y)
                        for (int k = y + 1; k <= 3; ++k)
                            output << 0 - ((3 * i + x) * 100 + (3 * j + y) * 10 + z) << " "
                            << 0 - ((3 * i + x) * 100 + (3 * j + k) * 10 + z) << " " << 0 << endl;
    }
    for (int z = 1; z <= 9; z++) {
        for (int i = 0; i <= 2; i++)
            for (int j = 0; j <= 2; j++)
                for (int x = 1; x <= 3; x++)
                    for (int y = 1; y <= 3; y++)
                        for (int k = x + 1; k <= 3; k++)
                            for (int l = 1; l <= 3; l++)
                                output << 0 - ((3 * i + x) * 100 + (3 * j + y) * 10 + z) << ' '
                                << 0 - ((3 * i + k) * 100 + (3 * j + l) * 10 + z) << ' ' << 0 << endl;
    }
    //11，22，33，44，55，66，77，88，99
    for (int x = 0; x < 9; ++x) {
        for (int y = x+1; y < 9; ++y)
            output << 0 - (x * 100 + x * 10 + a[x][x]) << " " << 0 - (y * 100 + y * 10 + a[y][y]) << " " << 0 << endl;
    }
    //19，28，37，46，55，64，73，82，91
    for (int x = 0; x < 9; ++x) {
        for(int y=x+1;y< 9;++y)
            output << 0 - (x * 100 + (10-x) * 10 + a[x][8-x]) << " " << 0 - (y * 100 + (10-y) * 10 + a[y][8-y]) << " " << 0 << endl;
    }
    output.close();
    return "sudoku.cnf";//返回文件名
}
