#pragma once
#include <stdio.h>
#include<iostream>
#include<fstream>
#include<ctime>
#include<cmath>
#include<stack>
#include<string>
#include<cstring>
#define _CRT_SECURE_NO_WARNINGS
using namespace std;

//定义结点类型
typedef struct node {
    int value = 0;//变元的值
    node* nextnode;//下一个变元
}node;

//定义根结点/头结点数据类型
typedef struct head {
    int num = 0;//子句个数/变元个数
    head * nextcol;//第一行/下一行的头结点
    node * firstnode;//第一个变元
}head;


//函数声明
//main.cpp
void display();
//cnf.cpp
head* cnfparser(string& filename);
void cnfsolver(head* R, int arg, int command, string& filename, int board[9][9]);
void outfile(bool issat, int t1,int t2, int* solutions, int arg, int command, string& outfilename);
//DPLL.cpp
int DPLL(head* R, int* solutions);
void printList(head* R);
int issingleclauseexist(head* R);
void simplifycnf(head* R, int singleclause);
int iscnfempty(head* R);
int isemptyclauseexist(head* R);
void deletecopy(head* Rcopy);
head* combine(head* R, int v);
int whatisnextplus(head* R);
head* copy(head* L);
int whatisnext(head* R);
int DPLLplus(head* L, int* book);
int singlespread(head* R, int* solutions);
//check.cpp
int check(string& filename);
int verification(head* ordL, int* solutions);
//sudoku.cpp
string board2cnf(int holes, int array[9][9]);
int create(int a[][9], int i, int j);
void dig(const int a[][9], int b[][9], int numDigits);
void print(const int a[][9]);
string sudoku2cnf(int a[][9], int holes);
