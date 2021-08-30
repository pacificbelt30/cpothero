#include<filesystem>
#include<fstream>
#include<stdio.h>
#include<stdint.h>
#include<stdlib.h>
#include<time.h>
#include <unistd.h>
#include"./othero.h"
#ifndef _EVAL_
#define _EVAL_

typedef struct _BestPosition{
  uint64_t pos;
  int eval;
}BestPosition;

class Eval{
public:
Eval();
~Eval();
//パターンの値を計算する
int sumhor2(BitBoard *board,int direction);//3^8 = 6561
int sumhor3(BitBoard *board,int direction);//3^8 = 6561
int sumhor4(BitBoard *board,int direction);//3^8 = 6561
int sumdir4(BitBoard *board,int direction);//3^4 = 81
int sumdir5(BitBoard *board,int direction);//3^5 = 243
int sumdir6(BitBoard *board,int direction);//3^6 = 729
int sumdir7(BitBoard *board,int direction);//3^7 = 2187
int sumdir8(BitBoard *board,int direction);//3^8 = 6561
int sumcor(BitBoard *board,int direction);//3^8 = 6561
int sumedg(BitBoard *board,int direction);//3^8 = 6561
int sumeval(BitBoard *board);//*

void initArray(int n,int *eval);//m要素の配列0埋め
//DEBUG用
void showBitboard(BitBoard *board);
//評価関数を開く開けない場合は全部0
void openeval();

//パターンの配列
//配列でかすぎる
int *hor2; 
int *hor3; 
int *hor4; 
int *dir4; 
int *dir5; 
int *dir6; 
int *dir7; 
int *dir8; 
int *cor; 
int *edg; 

uint64_t evalPos(uint64_t legalboard,BitBoard *board);
uint64_t evalPos_s(BitBoard *board);
/*
static int hor2l[6561]; 
static int hor3l[6561]; 
static int hor4l[6561]; 
static int dir4l[81]; 
static int dir5l[243]; 
static int dir6l[729]; 
static int dir7l[2187]; 
static int dir8l[6561]; 
static int corl[6561]; 
static int edgl[6561]; 
*/
};
#endif
