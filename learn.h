#include<iostream>
#include<fstream>
#include<stdio.h>
#include<stdint.h>
#include<stdlib.h>
#include<time.h>
#include <unistd.h>
#include "./othero.h"
#include "./eval.h"
using namespace std;

#ifndef _LEARN_
#define _LEARN_

#define SOLVE_DEPTH 10
#define KYOKUSU 1000 // 対局数

class Learn:public Eval{
    public:
//int **kif;
using Eval::Eval;
//using Eval::~Eval;
Learn(){cout << "LEARNc"<<endl;}
~Learn(){cout << "LEARNd"<<endl;}
//追加
void generateKif(int n);//棋譜を作成する関数
//getPosnum posの座標をint型で取得
int getPosnum(uint64_t pos);
//学習
void updateeval(int point,BitBoard *board);
void learning();
void writeeval(ofstream *fp,int n,int pat[]);
int **makeKifArray(int n);//kifのポインタ
};
#endif
