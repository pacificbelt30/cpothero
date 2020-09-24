#include<stdio.h>
#include<stdint.h>
#include<stdlib.h>
#include<time.h>
#include <unistd.h>
#include "./othero.h"
#include "./eval.h"

#ifndef _LEARN_
#define _LEARN_

#define SOLVE_DEPTH 10

//追加
void generateKif(int n);//棋譜を作成する関数
//getPosnum posの座標をint型で取得
int getPosnum(uint64_t pos);
//学習
void updateeval(int point,BitBoard *board);
void learning();
void writeeval(FILE *fp,int n,int pat[]);
int **makeKifArray(int n);//kifのポインタ
#endif
