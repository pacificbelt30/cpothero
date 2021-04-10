#include<iostream>
#include<fstream>
#ifdef __linux__
#include<filesystem>
#define getExecDirectoryName() std::filesystem::read_symlink("/proc/self/exe").remove_filename().string()
#endif
#ifdef _WIN64
#include<windows.h>
auto f = [](){
  char Path[MAX_PATH+1]; 
  if(0!=GetModuleFileName( NULL, Path, MAX_PATH )){// 実行ファイルの完全パスを取得
   char drive[MAX_PATH+1],dir[MAX_PATH+1],fname[MAX_PATH+1],ext[MAX_PATH+1];
   _splitpath(Path,drive,dir,fname,ext);//パス名を構成要素に分解します
   return string(drive)+string(dir);
  }
  return string("");
}
#define getExecDirectoryName() f()
#endif
#include<stdio.h>
#include<stdint.h>
#include<stdlib.h>
#include<time.h>
#include <unistd.h>
#include<string>
#include"./othero.h"
#include"./eval.h"
#include"./env.h"
#include "./engine/one.h"
using namespace std;
/*
棋譜のフォーマット
間は空白or改行
最初に手数?
石差: 先手番から見た石差
番地:1~64,0はパス扱い
*/

/*
 * 評価関数の値は予想石差の1000倍になるように調整
 *評価関数の値の合計は予想石差に1000をかけたもの
 * */

/*
http://www.es-cube.net/es-cube/reversi/sample/html/3_2.html
パターンのとり方
white = 2, black = 1, 空き = 0;
最大8個の石のパターン(6561通り)それ以上は多すぎる
diag4,diag5,diag6,diag7,diag8, 斜めのパターン
hor2,hor3,hor4, 縦横のパターン
edge 
  ********_
  _*_______
corner
  ***_____
  ***_____
  **______
  ________
*/

/*
//パターンの配列
//配列でかすぎる
int hor2[6561]; 
int hor3[6561]; 
int hor4[6561]; 
int dir4[81]; 
int dir5[243]; 
int dir6[729]; 
int dir7[2187]; 
int dir8[6561]; 
int cor[6561]; 
int edg[6561]; 
*/

//関数のプロトタイプ宣言
//ジャンプ用 prototype dec
//学習時にいらない関数

Eval::Eval()
{
    hor2 = (int*)malloc(6561*sizeof(int)); 
    hor3 = (int*)malloc(6561*sizeof(int)); 
    hor4 = (int*)malloc(6561*sizeof(int)); 
    dir4 = (int*)malloc(81  *sizeof(int)); 
    dir5 = (int*)malloc(243 *sizeof(int)); 
    dir6 = (int*)malloc(729 *sizeof(int)); 
    dir7 = (int*)malloc(2187*sizeof(int)); 
    dir8 = (int*)malloc(6561*sizeof(int)); 
    cor  = (int*)malloc(6561*sizeof(int)); 
    edg  = (int*)malloc(6561*sizeof(int)); 
    openeval();
}
Eval::~Eval()
{
    free(hor2); 
    free(hor3); 
    free(hor4); 
    free(dir4); 
    free(dir5); 
    free(dir6); 
    free(dir7); 
    free(dir8); 
    free(cor); 
    free(edg); 
}

//パターン
//direction 
//1=2行目
//2=7行目
//3=2列目
//4=7列目
//他は-1
int Eval::sumhor2(BitBoard *board,int direction)
{
  int index;
  switch (direction)
  {
  case 1:
    index = ((board->white>>55)&1)*2+((board->black>>55)&1);
    index = index * 3 + ((board->white>>54)&1)*2+((board->black>>54)&1);
    index = index * 3 + ((board->white>>53)&1)*2+((board->black>>53)&1);
    index = index * 3 + ((board->white>>52)&1)*2+((board->black>>52)&1);
    index = index * 3 + ((board->white>>51)&1)*2+((board->black>>51)&1);
    index = index * 3 + ((board->white>>50)&1)*2+((board->black>>50)&1);
    index = index * 3 + ((board->white>>49)&1)*2+((board->black>>49)&1);
    return  index * 3 + ((board->white>>48)&1)*2+((board->black>>48)&1);
    break;
  
  case 2:
    index = ((board->white>>15)&1)*2+((board->black>>15)&1);
    index = index * 3 + ((board->white>>14)&1)*2+((board->black>>14)&1);
    index = index * 3 + ((board->white>>13)&1)*2+((board->black>>13)&1);
    index = index * 3 + ((board->white>>12)&1)*2+((board->black>>12)&1);
    index = index * 3 + ((board->white>>11)&1)*2+((board->black>>11)&1);
    index = index * 3 + ((board->white>>10)&1)*2+((board->black>>10)&1);
    index = index * 3 + ((board->white>> 9)&1)*2+((board->black>> 9)&1);
    return  index * 3 + ((board->white>> 8)&1)*2+((board->black>> 8)&1);
    break;

  case 3:
    index = ((board->white>>62)&1)*2+((board->black>>62)&1);
    index = index * 3 + ((board->white>>54)&1)*2+((board->black>>54)&1);
    index = index * 3 + ((board->white>>46)&1)*2+((board->black>>46)&1);
    index = index * 3 + ((board->white>>38)&1)*2+((board->black>>38)&1);
    index = index * 3 + ((board->white>>30)&1)*2+((board->black>>30)&1);
    index = index * 3 + ((board->white>>22)&1)*2+((board->black>>22)&1);
    index = index * 3 + ((board->white>>14)&1)*2+((board->black>>14)&1);
    return  index * 3 + ((board->white>> 6)&1)*2+((board->black>> 6)&1);
    break;

  case 4:
    index = ((board->white>>57)&1)*2+((board->black>>57)&1);
    index = index * 3 + ((board->white>>49)&1)*2+((board->black>>49)&1);
    index = index * 3 + ((board->white>>41)&1)*2+((board->black>>41)&1);
    index = index * 3 + ((board->white>>33)&1)*2+((board->black>>33)&1);
    index = index * 3 + ((board->white>>25)&1)*2+((board->black>>25)&1);
    index = index * 3 + ((board->white>>17)&1)*2+((board->black>>17)&1);
    index = index * 3 + ((board->white>> 9)&1)*2+((board->black>> 9)&1);
    return  index * 3 + ((board->white>> 1)&1)*2+((board->black>> 1)&1);
    break;

  default:
    if(DEBUG_MODE) cout << "ERROR sumhor2" << endl;
    return -1;
    break;
  }

  
}

int Eval::sumhor3(BitBoard *board,int direction)
{
  int index;
  switch (direction)
  {
  case 1:
    index = ((board->white>>47)&1)*2+((board->black>>47)&1);
    index = index * 3 + ((board->white>>46)&1)*2+((board->black>>46)&1);
    index = index * 3 + ((board->white>>45)&1)*2+((board->black>>45)&1);
    index = index * 3 + ((board->white>>44)&1)*2+((board->black>>44)&1);
    index = index * 3 + ((board->white>>43)&1)*2+((board->black>>43)&1);
    index = index * 3 + ((board->white>>42)&1)*2+((board->black>>42)&1);
    index = index * 3 + ((board->white>>41)&1)*2+((board->black>>41)&1);
    return  index * 3 + ((board->white>>40)&1)*2+((board->black>>40)&1);
    break;
  
  case 2:
    index = ((board->white>>23)&1)*2+((board->black>>23)&1);
    index = index * 3 + ((board->white>>22)&1)*2+((board->black>>22)&1);
    index = index * 3 + ((board->white>>21)&1)*2+((board->black>>21)&1);
    index = index * 3 + ((board->white>>20)&1)*2+((board->black>>20)&1);
    index = index * 3 + ((board->white>>19)&1)*2+((board->black>>19)&1);
    index = index * 3 + ((board->white>>18)&1)*2+((board->black>>18)&1);
    index = index * 3 + ((board->white>>17)&1)*2+((board->black>>17)&1);
    return  index * 3 + ((board->white>>16)&1)*2+((board->black>>16)&1);
    break;

  case 3:
    index = ((board->white>>61)&1)*2+((board->black>>61)&1);
    index = index * 3 + ((board->white>>53)&1)*2+((board->black>>53)&1);
    index = index * 3 + ((board->white>>45)&1)*2+((board->black>>45)&1);
    index = index * 3 + ((board->white>>37)&1)*2+((board->black>>37)&1);
    index = index * 3 + ((board->white>>29)&1)*2+((board->black>>29)&1);
    index = index * 3 + ((board->white>>21)&1)*2+((board->black>>21)&1);
    index = index * 3 + ((board->white>>13)&1)*2+((board->black>>13)&1);
    return  index * 3 + ((board->white>> 5)&1)*2+((board->black>> 5)&1);
    break;

  case 4:
    index = ((board->white>>58)&1)*2+((board->black>>58)&1);
    index = index * 3 + ((board->white>>50)&1)*2+((board->black>>50)&1);
    index = index * 3 + ((board->white>>42)&1)*2+((board->black>>42)&1);
    index = index * 3 + ((board->white>>34)&1)*2+((board->black>>34)&1);
    index = index * 3 + ((board->white>>26)&1)*2+((board->black>>26)&1);
    index = index * 3 + ((board->white>>18)&1)*2+((board->black>>18)&1);
    index = index * 3 + ((board->white>>10)&1)*2+((board->black>>10)&1);
    return  index * 3 + ((board->white>> 2)&1)*2+((board->black>> 2)&1);
    break;

  default:
    if(DEBUG_MODE) cout << "ERROR sumhor3" << endl;
    return -1;
    break;
  }

  
}

int Eval::sumhor4(BitBoard *board,int direction)
{
  int index;
  switch (direction)
  {
  case 1:
    index = ((board->white>>39)&1)*2+((board->black>>39)&1);
    index = index * 3 + ((board->white>>38)&1)*2+((board->black>>38)&1);
    index = index * 3 + ((board->white>>37)&1)*2+((board->black>>37)&1);
    index = index * 3 + ((board->white>>36)&1)*2+((board->black>>36)&1);
    index = index * 3 + ((board->white>>35)&1)*2+((board->black>>35)&1);
    index = index * 3 + ((board->white>>34)&1)*2+((board->black>>34)&1);
    index = index * 3 + ((board->white>>33)&1)*2+((board->black>>33)&1);
    return  index * 3 + ((board->white>>32)&1)*2+((board->black>>32)&1);
    break;
  
  case 2:
    index = ((board->white>>31)&1)*2+((board->black>>31)&1);
    index = index * 3 + ((board->white>>30)&1)*2+((board->black>>30)&1);
    index = index * 3 + ((board->white>>29)&1)*2+((board->black>>29)&1);
    index = index * 3 + ((board->white>>28)&1)*2+((board->black>>28)&1);
    index = index * 3 + ((board->white>>27)&1)*2+((board->black>>27)&1);
    index = index * 3 + ((board->white>>26)&1)*2+((board->black>>26)&1);
    index = index * 3 + ((board->white>>25)&1)*2+((board->black>>25)&1);
    return  index * 3 + ((board->white>>24)&1)*2+((board->black>>24)&1);
    break;

  case 3:
    index = ((board->white>>62)&1)*2+((board->black>>62)&1);
    index = index * 3 + ((board->white>>54)&1)*2+((board->black>>54)&1);
    index = index * 3 + ((board->white>>46)&1)*2+((board->black>>46)&1);
    index = index * 3 + ((board->white>>38)&1)*2+((board->black>>38)&1);
    index = index * 3 + ((board->white>>30)&1)*2+((board->black>>30)&1);
    index = index * 3 + ((board->white>>22)&1)*2+((board->black>>22)&1);
    index = index * 3 + ((board->white>>14)&1)*2+((board->black>>14)&1);
    return  index * 3 + ((board->white>> 6)&1)*2+((board->black>> 6)&1);
    break;

  case 4:
    index = ((board->white>>57)&1)*2+((board->black>>57)&1);
    index = index * 3 + ((board->white>>49)&1)*2+((board->black>>49)&1);
    index = index * 3 + ((board->white>>41)&1)*2+((board->black>>41)&1);
    index = index * 3 + ((board->white>>33)&1)*2+((board->black>>33)&1);
    index = index * 3 + ((board->white>>25)&1)*2+((board->black>>25)&1);
    index = index * 3 + ((board->white>>17)&1)*2+((board->black>>17)&1);
    index = index * 3 + ((board->white>> 9)&1)*2+((board->black>> 9)&1);
    return  index * 3 + ((board->white>> 1)&1)*2+((board->black>> 1)&1);
    break;

  default:
    if(DEBUG_MODE) cout << "ERROR sumhor4" << endl;
    return -1;
    break;
  }

  
}

int Eval::sumdir4(BitBoard *board,int direction)
{
  int index;
  switch (direction)
  {
  case 1:
    index = ((board->white>>60)&1)*2+((board->black>>60)&1);
    index = index * 3 + ((board->white>>53)&1)*2+((board->black>>53)&1);
    index = index * 3 + ((board->white>>46)&1)*2+((board->black>>46)&1);
    return  index * 3 + ((board->white>>39)&1)*2+((board->black>>39)&1);
    break;
  
  case 2:
    index = ((board->white>>59)&1)*2+((board->black>>15)&1);
    index = index * 3 + ((board->white>>50)&1)*2+((board->black>>50)&1);
    index = index * 3 + ((board->white>>41)&1)*2+((board->black>>41)&1);
    return  index * 3 + ((board->white>>32)&1)*2+((board->black>>32)&1);
    break;

  case 3:
    index = ((board->white>>31)&1)*2+((board->black>>31)&1);
    index = index * 3 + ((board->white>>32)&1)*2+((board->black>>32)&1);
    index = index * 3 + ((board->white>>13)&1)*2+((board->black>>13)&1);
    return  index * 3 + ((board->white>> 4)&1)*2+((board->black>> 4)&1);
    break;

  case 4:
    index = ((board->white>>24)&1)*2+((board->black>>57)&1);
    index = index * 3 + ((board->white>>17)&1)*2+((board->black>>17)&1);
    index = index * 3 + ((board->white>>10)&1)*2+((board->black>>10)&1);
    return  index * 3 + ((board->white>> 3)&1)*2+((board->black>> 3)&1);
    break;

  default:
    if(DEBUG_MODE) cout << "ERROR sumdir4" << endl;
    return -1;
    break;
  }

  
}

int Eval::sumdir5(BitBoard *board,int direction)
{
  int index;
  switch (direction)
  {
  case 1:
    index = ((board->white>>59)&1)*2+((board->black>>59)&1);
    index = index * 3 + ((board->white>>52)&1)*2+((board->black>>52)&1);
    index = index * 3 + ((board->white>>45)&1)*2+((board->black>>45)&1);
    index = index * 3 + ((board->white>>38)&1)*2+((board->black>>38)&1);
    return  index * 3 + ((board->white>>31)&1)*2+((board->black>>31)&1);
    break;
  
  case 2:
    index = ((board->white>>60)&1)*2+((board->black>>60)&1);
    index = index * 3 + ((board->white>>51)&1)*2+((board->black>>51)&1);
    index = index * 3 + ((board->white>>42)&1)*2+((board->black>>42)&1);
    index = index * 3 + ((board->white>>33)&1)*2+((board->black>>33)&1);
    return  index * 3 + ((board->white>>24)&1)*2+((board->black>>24)&1);
    break;

  case 3:
    index = ((board->white>>39)&1)*2+((board->black>>39)&1);
    index = index * 3 + ((board->white>>30)&1)*2+((board->black>>30)&1);
    index = index * 3 + ((board->white>>21)&1)*2+((board->black>>21)&1);
    index = index * 3 + ((board->white>>12)&1)*2+((board->black>>12)&1);
    return  index * 3 + ((board->white>> 3)&1)*2+((board->black>> 3)&1);
    break;

  case 4:
    index = ((board->white>>32)&1)*2+((board->black>>32)&1);
    index = index * 3 + ((board->white>>25)&1)*2+((board->black>>25)&1);
    index = index * 3 + ((board->white>>18)&1)*2+((board->black>>18)&1);
    index = index * 3 + ((board->white>>11)&1)*2+((board->black>>11)&1);
    return  index * 3 + ((board->white>> 4)&1)*2+((board->black>> 4)&1);
    break;

  default:
    if(DEBUG_MODE) cout << "ERROR sumdir5" << endl;
    return -1;
    break;
  }

  
}

int Eval::sumdir6(BitBoard *board,int direction)
{
  int index;
  switch (direction)
  {
  case 1:
    index = ((board->white>>58)&1)*2+((board->black>>58)&1);
    index = index * 3 + ((board->white>>51)&1)*2+((board->black>>51)&1);
    index = index * 3 + ((board->white>>44)&1)*2+((board->black>>44)&1);
    index = index * 3 + ((board->white>>27)&1)*2+((board->black>>27)&1);
    index = index * 3 + ((board->white>>30)&1)*2+((board->black>>30)&1);
    return  index * 3 + ((board->white>>23)&1)*2+((board->black>>23)&1);
    break;
  
  case 2:
    index = ((board->white>>61)&1)*2+((board->black>>61)&1);
    index = index * 3 + ((board->white>>52)&1)*2+((board->black>>52)&1);
    index = index * 3 + ((board->white>>43)&1)*2+((board->black>>43)&1);
    index = index * 3 + ((board->white>>34)&1)*2+((board->black>>34)&1);
    index = index * 3 + ((board->white>>25)&1)*2+((board->black>>25)&1);
    return  index * 3 + ((board->white>>16)&1)*2+((board->black>>16)&1);
    break;

  case 3:
    index = ((board->white>>47)&1)*2+((board->black>>47)&1);
    index = index * 3 + ((board->white>>38)&1)*2+((board->black>>38)&1);
    index = index * 3 + ((board->white>>29)&1)*2+((board->black>>29)&1);
    index = index * 3 + ((board->white>>20)&1)*2+((board->black>>20)&1);
    index = index * 3 + ((board->white>>11)&1)*2+((board->black>>11)&1);
    return  index * 3 + ((board->white>> 2)&1)*2+((board->black>> 2)&1);
    break;

  case 4:
    index = ((board->white>>40)&1)*2+((board->black>>40)&1);
    index = index * 3 + ((board->white>>33)&1)*2+((board->black>>33)&1);
    index = index * 3 + ((board->white>>26)&1)*2+((board->black>>26)&1);
    index = index * 3 + ((board->white>>19)&1)*2+((board->black>>19)&1);
    index = index * 3 + ((board->white>>12)&1)*2+((board->black>>12)&1);
    return  index * 3 + ((board->white>> 5)&1)*2+((board->black>> 5)&1);
    break;

  default:
    if(DEBUG_MODE) cout << "ERROR sumdir6" << endl;
    return -1;
    break;
  }

  
}

int Eval::sumdir7(BitBoard *board,int direction)
{
  int index;
  switch (direction)
  {
  case 2:
    index = ((board->white>>62)&1)*2+((board->black>>62)&1);
    index = index * 3 + ((board->white>>53)&1)*2+((board->black>>53)&1);
    index = index * 3 + ((board->white>>44)&1)*2+((board->black>>44)&1);
    index = index * 3 + ((board->white>>35)&1)*2+((board->black>>35)&1);
    index = index * 3 + ((board->white>>26)&1)*2+((board->black>>26)&1);
    index = index * 3 + ((board->white>>17)&1)*2+((board->black>>17)&1);
    return  index * 3 + ((board->white>> 8)&1)*2+((board->black>> 8)&1);
    break;
  
  case 1:
    index = ((board->white>>57)&1)*2+((board->black>>57)&1);
    index = index * 3 + ((board->white>>50)&1)*2+((board->black>>50)&1);
    index = index * 3 + ((board->white>>43)&1)*2+((board->black>>43)&1);
    index = index * 3 + ((board->white>>36)&1)*2+((board->black>>36)&1);
    index = index * 3 + ((board->white>>29)&1)*2+((board->black>>29)&1);
    index = index * 3 + ((board->white>>22)&1)*2+((board->black>>22)&1);
    return  index * 3 + ((board->white>>15)&1)*2+((board->black>>15)&1);
    break;

  case 3:
    index = ((board->white>>55)&1)*2+((board->black>>55)&1);
    index = index * 3 + ((board->white>>46)&1)*2+((board->black>>46)&1);
    index = index * 3 + ((board->white>>37)&1)*2+((board->black>>37)&1);
    index = index * 3 + ((board->white>>28)&1)*2+((board->black>>28)&1);
    index = index * 3 + ((board->white>>19)&1)*2+((board->black>>19)&1);
    index = index * 3 + ((board->white>>10)&1)*2+((board->black>>10)&1);
    return  index * 3 + ((board->white>> 1)&1)*2+((board->black>> 1)&1);
    break;

  case 4:
    index = ((board->white>>48)&1)*2+((board->black>>48)&1);
    index = index * 3 + ((board->white>>41)&1)*2+((board->black>>41)&1);
    index = index * 3 + ((board->white>>34)&1)*2+((board->black>>34)&1);
    index = index * 3 + ((board->white>>27)&1)*2+((board->black>>27)&1);
    index = index * 3 + ((board->white>>20)&1)*2+((board->black>>20)&1);
    index = index * 3 + ((board->white>>13)&1)*2+((board->black>>13)&1);
    return  index * 3 + ((board->white>> 6)&1)*2+((board->black>> 6)&1);
    break;

  default:
    if(DEBUG_MODE) cout << "ERROR sumdir7" << endl;
    return -1;
    break;
  }

  
}

int Eval::sumdir8(BitBoard *board,int direction)
{
  int index;
  switch (direction)
  {
  case 2:
    index = ((board->white>>63)&1)*2+((board->black>>63)&1);
    index = index * 3 + ((board->white>>54)&1)*2+((board->black>>54)&1);
    index = index * 3 + ((board->white>>45)&1)*2+((board->black>>45)&1);
    index = index * 3 + ((board->white>>36)&1)*2+((board->black>>36)&1);
    index = index * 3 + ((board->white>>27)&1)*2+((board->black>>27)&1);
    index = index * 3 + ((board->white>>18)&1)*2+((board->black>>18)&1);
    index = index * 3 + ((board->white>> 9)&1)*2+((board->black>> 9)&1);
    return  index * 3 + ((board->white>> 0)&1)*2+((board->black>>0)&1);
    break;
  
  case 1:
    index = ((board->white>>56)&1)*2+((board->black>>56)&1);
    index = index * 3 + ((board->white>>49)&1)*2+((board->black>>49)&1);
    index = index * 3 + ((board->white>>42)&1)*2+((board->black>>42)&1);
    index = index * 3 + ((board->white>>35)&1)*2+((board->black>>35)&1);
    index = index * 3 + ((board->white>>28)&1)*2+((board->black>>28)&1);
    index = index * 3 + ((board->white>>21)&1)*2+((board->black>>21)&1);
    index = index * 3 + ((board->white>>14)&1)*2+((board->black>>14)&1);
    return  index * 3 + ((board->white>> 7)&1)*2+((board->black>> 7)&1);
    break;

  default:
    if(DEBUG_MODE) cout << "ERROR sumdir8" << endl;
    return -1;
    break;
  }

  
}

int Eval::sumedg(BitBoard *board,int direction)
{
  int index;
  switch (direction)
  {
  case 1:
    index = ((board->white>>63)&1)*2+((board->black>>63)&1);
    index = index * 3 + ((board->white>>62)&1)*2+((board->black>>62)&1);
    index = index * 3 + ((board->white>>61)&1)*2+((board->black>>61)&1);
    index = index * 3 + ((board->white>>60)&1)*2+((board->black>>60)&1);
    index = index * 3 + ((board->white>>59)&1)*2+((board->black>>59)&1);
    index = index * 3 + ((board->white>>58)&1)*2+((board->black>>58)&1);
    index = index * 3 + ((board->white>>57)&1)*2+((board->black>>57)&1);
    return  index * 3 + ((board->white>>54)&1)*2+((board->black>>54)&1);
    break;
  
  case 2:
    index = ((board->white>>56)&1)*2+((board->black>>56)&1);
    index = index * 3 + ((board->white>>48)&1)*2+((board->black>>48)&1);
    index = index * 3 + ((board->white>>40)&1)*2+((board->black>>40)&1);
    index = index * 3 + ((board->white>>32)&1)*2+((board->black>>32)&1);
    index = index * 3 + ((board->white>>24)&1)*2+((board->black>>24)&1);
    index = index * 3 + ((board->white>>16)&1)*2+((board->black>>16)&1);
    index = index * 3 + ((board->white>> 8)&1)*2+((board->black>> 8)&1);
    return  index * 3 + ((board->white>>49)&1)*2+((board->black>>49)&1);
    break;

  case 3:
    index = ((board->white>>7)&1)*2+((board->black>>7)&1);
    index = index * 3 + ((board->white>>15)&1)*2+((board->black>>15)&1);
    index = index * 3 + ((board->white>>23)&1)*2+((board->black>>23)&1);
    index = index * 3 + ((board->white>>31)&1)*2+((board->black>>31)&1);
    index = index * 3 + ((board->white>>39)&1)*2+((board->black>>39)&1);
    index = index * 3 + ((board->white>>47)&1)*2+((board->black>>47)&1);
    index = index * 3 + ((board->white>>55)&1)*2+((board->black>>55)&1);
    return  index * 3 + ((board->white>>14)&1)*2+((board->black>>14)&1);
    break;

  case 4:
    index = ((board->white>>0)&1)*2+((board->black>>0)&1);
    index = index * 3 + ((board->white>>1)&1)*2+((board->black>>1)&1);
    index = index * 3 + ((board->white>>2)&1)*2+((board->black>>2)&1);
    index = index * 3 + ((board->white>>3)&1)*2+((board->black>>3)&1);
    index = index * 3 + ((board->white>>4)&1)*2+((board->black>>4)&1);
    index = index * 3 + ((board->white>>5)&1)*2+((board->black>>5)&1);
    index = index * 3 + ((board->white>>6)&1)*2+((board->black>>6)&1);
    return  index * 3 + ((board->white>>9)&1)*2+((board->black>>9)&1);
    break;

  default:
    if(DEBUG_MODE) cout << "ERROR sumedg" << endl;
    return -1;
    break;
  }

  
}

int Eval::sumcor(BitBoard *board,int direction)
{
  int index;
  switch (direction)
  {
  case 1:
    index = ((board->white>>63)&1)*2+((board->black>>63)&1);
    index = index * 3 + ((board->white>>62)&1)*2+((board->black>>62)&1);
    index = index * 3 + ((board->white>>61)&1)*2+((board->black>>61)&1);
    index = index * 3 + ((board->white>>55)&1)*2+((board->black>>55)&1);
    index = index * 3 + ((board->white>>54)&1)*2+((board->black>>54)&1);
    index = index * 3 + ((board->white>>53)&1)*2+((board->black>>53)&1);
    index = index * 3 + ((board->white>>47)&1)*2+((board->black>>47)&1);
    return  index * 3 + ((board->white>>46)&1)*2+((board->black>>46)&1);
    break;
  
  case 2:
    index = ((board->white>>56)&1)*2+((board->black>>56)&1);
    index = index * 3 + ((board->white>>48)&1)*2+((board->black>>48)&1);
    index = index * 3 + ((board->white>>40)&1)*2+((board->black>>40)&1);
    index = index * 3 + ((board->white>>57)&1)*2+((board->black>>57)&1);
    index = index * 3 + ((board->white>>49)&1)*2+((board->black>>49)&1);
    index = index * 3 + ((board->white>>41)&1)*2+((board->black>>41)&1);
    index = index * 3 + ((board->white>>58)&1)*2+((board->black>>58)&1);
    return  index * 3 + ((board->white>>50)&1)*2+((board->black>>50)&1);
    break;

  case 3:
    index = ((board->white>>0)&1)*2+((board->black>>0)&1);
    index = index * 3 + ((board->white>> 1)&1)*2+((board->black>> 1)&1);
    index = index * 3 + ((board->white>> 2)&1)*2+((board->black>> 2)&1);
    index = index * 3 + ((board->white>> 8)&1)*2+((board->black>> 8)&1);
    index = index * 3 + ((board->white>> 9)&1)*2+((board->black>> 9)&1);
    index = index * 3 + ((board->white>>10)&1)*2+((board->black>>10)&1);
    index = index * 3 + ((board->white>>16)&1)*2+((board->black>>16)&1);
    return  index * 3 + ((board->white>>17)&1)*2+((board->black>>17)&1);
    break;

  case 4:
    index = ((board->white>> 7)&1)*2+((board->black>> 7)&1);
    index = index * 3 + ((board->white>>15)&1)*2+((board->black>>15)&1);
    index = index * 3 + ((board->white>>23)&1)*2+((board->black>>23)&1);
    index = index * 3 + ((board->white>> 6)&1)*2+((board->black>> 6)&1);
    index = index * 3 + ((board->white>>14)&1)*2+((board->black>>14)&1);
    index = index * 3 + ((board->white>>22)&1)*2+((board->black>>22)&1);
    index = index * 3 + ((board->white>> 5)&1)*2+((board->black>> 5)&1);
    return  index * 3 + ((board->white>>13)&1)*2+((board->black>>13)&1);
    break;

  default:
    if(DEBUG_MODE) cout << "ERROR sumcor" << endl;
    return 0;
    break;
  }
}

int Eval::sumeval(BitBoard *board)
{
  int sum=0;
  sum += hor2[sumhor2(board,1)];//3^8 = 6561
  sum += hor2[sumhor2(board,2)];//3^8 = 6561
  sum += hor2[sumhor2(board,3)];//3^8 = 6561
  sum += hor2[sumhor2(board,4)];//3^8 = 6561

  sum += hor3[sumhor3(board,1)];//3^8 = 6561
  sum += hor3[sumhor3(board,2)];//3^8 = 6561
  sum += hor3[sumhor3(board,3)];//3^8 = 6561
  sum += hor3[sumhor3(board,4)];//3^8 = 6561

  sum += hor4[sumhor4(board,1)];//3^8 = 6561
  sum += hor4[sumhor4(board,2)];//3^8 = 6561
  sum += hor4[sumhor4(board,3)];//3^8 = 6561
  sum += hor4[sumhor4(board,4)];//3^8 = 6561

  sum += dir4[sumdir4(board,1)];//3^4 = 81
  sum += dir4[sumdir4(board,2)];//3^4 = 81
  sum += dir4[sumdir4(board,3)];//3^4 = 81
  sum += dir4[sumdir4(board,4)];//3^4 = 81

  sum += dir5[sumdir5(board,1)];//3^5 = 243
  sum += dir5[sumdir5(board,2)];//3^5 = 243
  sum += dir5[sumdir5(board,3)];//3^5 = 243
  sum += dir5[sumdir5(board,4)];//3^5 = 243

  sum += dir6[sumdir6(board,1)];//3^6 = 729
  sum += dir6[sumdir6(board,2)];//3^6 = 729
  sum += dir6[sumdir6(board,3)];//3^6 = 729
  sum += dir6[sumdir6(board,4)];//3^6 = 729

  sum += dir7[sumdir7(board,1)];//3^7 = 2187
  sum += dir7[sumdir7(board,2)];//3^7 = 2187
  sum += dir7[sumdir7(board,3)];//3^7 = 2187
  sum += dir7[sumdir7(board,4)];//3^7 = 2187

  sum += dir8[sumdir8(board,1)];//3^8 = 6561
  sum += dir8[sumdir8(board,2)];//3^8 = 6561

  sum += cor[sumcor(board,1)];//3^8 = 6561
  sum += cor[sumcor(board,2)];//3^8 = 6561
  sum += cor[sumcor(board,3)];//3^8 = 6561
  sum += cor[sumcor(board,4)];//3^8 = 6561

  sum += edg[sumedg(board,1)];//3^8 = 6561
  sum += edg[sumedg(board,2)];//3^8 = 6561
  sum += edg[sumedg(board,3)];//3^8 = 6561
  sum += edg[sumedg(board,4)];//3^8 = 6561

  fflush(stdout);
  return sum;
}

//評価関数を開いて
//void openeval(int *eval,int num)
void Eval::openeval()
{
  int i;
  //FILE *d4,*d5,*d6,*d7,*d8,*h2,*h3,*h4,*cr,*eg;
  ifstream d4,d5,d6,d7,d8,h2,h3,h4,cr,eg;

  string dirpath = getExecDirectoryName() + "eval/";

  d4.open((dirpath+"d4.txt").c_str());
  if(!d4) {
      initArray(81,dir4);
      if(DEBUG_MODE) cout << "eval/d4.txt を開けませんでした" << endl;
  } 
  else
  {
      for(i=0;i<81;i++)
      {
        d4 >> dir4[i];
        if(DEBUG_MODE) cout << dir4[i] << endl;
      }
      d4.close();
  }

  d5.open((dirpath+"d5.txt").c_str());
  if(!d5) {
      initArray(243,dir5); 
      if(DEBUG_MODE) cout << "eval/d5.txt を開けませんでした" << endl;
  }
  else
  {
      for(i=0;i<243;i++)
      {
        d5 >> dir5[i];
        if(DEBUG_MODE) cout << dir5[i] << endl;
      }
      d5.close();
  }

  d6.open((dirpath+"d6.txt").c_str());
  if(!d6) {
      initArray(729,dir6); 
      if(DEBUG_MODE) cout << "eval/d6.txt を開けませんでした" << endl;
  }
  else
  {
      for(i=0;i<729;i++)
      {
        d6 >> dir6[i];
        if(DEBUG_MODE) cout << dir6[i] << endl;
      }
      d6.close();
  }

  d7.open((dirpath+"d7.txt").c_str());
  if(!d7) {
      initArray(2187,dir7); 
      if(DEBUG_MODE) cout << "eval/d7.txt を開けませんでした" << endl;
  }
  else
  {
      for(i=0;i<2187;i++)
      {
        d7 >> dir7[i];
        if(DEBUG_MODE) cout << dir7[i] << endl;
      }
      d7.close();
  }

  d8.open((dirpath+"d8.txt").c_str());
  if(!d8) {
      initArray(6561,dir8); 
      if(DEBUG_MODE) cout << "eval/d8.txt を開けませんでした" << endl;
  }
  else
  {
      for(i=0;i<6561;i++)
      {
        d8 >> dir8[i];
        if(DEBUG_MODE) cout << dir8[i] << endl;
      }
      d8.close();
  }

  h2.open((dirpath+"h2.txt").c_str());
  if(!h2) {
      initArray(6561,hor2); 
      if(DEBUG_MODE) cout << "eval/h2.txt を開けませんでした" << endl;
  }
  else
  {
      for(i=0;i<6561;i++)
      {
        h2 >> hor2[i];
        if(DEBUG_MODE) cout << hor2[i] << endl;
      }
      h2.close();
  }

  h3.open((dirpath+"h3.txt").c_str());
  if(!h3) {
      initArray(6561,hor3); 
      if(DEBUG_MODE) cout << "eval/h3.txt を開けませんでした" << endl;
  }
  else
  {
      for(i=0;i<6561;i++)
      {
        h3 >> hor3[i];
        if(DEBUG_MODE) cout << hor3[i] << endl;
      }
      h3.close();
  }

  h4.open((dirpath+"h4.txt").c_str());
  if(!h4) {
      initArray(6561,hor4); 
      if(DEBUG_MODE) cout << "eval/h4.txt を開けませんでした" << endl;
  }
  else
  {
      for(i=0;i<6561;i++)
      {
        h4 >> hor4[i];
        if(DEBUG_MODE) cout << hor4[i] << endl;
      }
      h4.close();
  }

  cr.open((dirpath+"cr.txt").c_str());
  if(!cr) {
      initArray(6561,cor); 
      if(DEBUG_MODE) cout << "eval/cr.txt を開けませんでした" << endl;
  }
  else
  {
      for(i=0;i<6561;i++)
      {
        cr >> cor[i];
        if(DEBUG_MODE) cout << cor[i] << endl;
      }
      cr.close();
  }

  eg.open((dirpath+"eg.txt").c_str());
  if(!eg) {
      initArray(6561,edg); 
      if(DEBUG_MODE) cout << "eval/eg.txt を開けませんでした" << endl;
  }
  else
  {
      for(i=0;i<6561;i++)
      {
        eg >> edg[i];
        if(DEBUG_MODE) cout << edg[i] << endl;
      }
      eg.close();
  }

  return ;

}


void Eval::initArray(int n,int *eval)
{
  int i;
  for ( i = 0; i < n; i++)
  {
    eval[i]=0;
  }
  return ;
}

//Debug用
void Eval::showBitboard(BitBoard *board)
{
  if(DEBUG_MODE) return;
  int i;
  cout << "SENTE" << endl;
  for(i=0;i<64;i++)
  {
      if((board->black>>(63-i)&1) ==1) cout << "o";
      else cout << "-";
  }
  cout << endl;
  cout << "GOTE" << endl;
  for(i=0;i<64;i++)
  {
      if((board->white>>(63-i)&1) ==1) cout << "o";
      else cout << "-";
  }
  cout << endl;
}

uint64_t Eval::evalPos(uint64_t legalboard,BitBoard *board)
{
  BitBoard temp;
  int i;
  int index,num,count=0;
  int *sum;
  num = Othero::bitCount(legalboard);
  if(DEBUG_MODE) cout << "num=" << num << endl;
  sum = (int *)malloc(sizeof(int)*num);
  if(DEBUG_MODE) cout << "legalboard=" << legalboard << endl;
  for ( i = 0; i < 64; i++)
  {
    if(legalboard&((uint64_t)(1)<<i))
    {
      temp = Othero::vput(legalboard&((uint64_t)(1)<<i),board);
      sum[count] = sumeval(&temp);
      //if(DEBUG_MODE) cout << "評価値=" << sum[count] << endl;
      count++;
    }
  }
  if (board->teban == SENTE) index = max(sum,num);
  else index = min(sum,num);
  count=0;
  //if(DEBUG_MODE) cout << "index=" << index << endl;
  if(DEBUG_MODE) cout << "評価値(先手視点) = " << sum[index] << endl;
  free(sum);
  for(i=0;i<64;i++)
  {
    if(legalboard&((uint64_t)(1)<<i))
    {
      //if(DEBUG_MODE) cout << "i = " << i endl;
      if(index==count) return (uint64_t)(1)<<i;
      else count++;
    }
  }
  return 0;
}


