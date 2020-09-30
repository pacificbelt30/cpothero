#include<stdio.h>
#include<stdint.h>
#include<stdlib.h>
#include<time.h>
#include <unistd.h>
#include"./othero.h"
#include"./eval.h"
#include "./engine/one.h"
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
    printf("ERROR sumhor2");
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
    printf("ERROR sumhor3");
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
    printf("ERROR sumhor4");
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
    printf("ERROR sumdir4");
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
    printf("ERROR sumdir5");
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
    printf("ERROR sumdir6");
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
    printf("ERROR sumdir7");
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
    printf("ERROR sumdir8");
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
    printf("ERROR sumedg");
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
    printf("ERROR sumcor");
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
  FILE *d4,*d5,*d6,*d7,*d8,*h2,*h3,*h4,*cr,*eg;

  d4 = fopen("eval/d4.txt","r");
  if(d4 == NULL) {
      initArray(81,dir4);
      printf("eval/d4.txt を開けませんでした\n");
  } 
  else
  {
      for(i=0;i<81;i++)
      {
        fscanf(d4,"%d",&dir4[i]);
      }
  }

  d5 = fopen("eval/d5.txt","r");
  if(d5 == NULL) {
      initArray(243,dir5); 
      printf("eval/d5.txt を開けませんでした\n");
  }
  else
  {
      for(i=0;i<243;i++)
      {
        fscanf(d5,"%d",&dir5[i]);
      }
  }

  d6 = fopen("eval/d6.txt","r");
  if(d6 == NULL) {
      initArray(729,dir6); 
      printf("eval/d6.txt を開けませんでした\n");
  }
  else
  {
      for(i=0;i<729;i++)
      {
        fscanf(d6,"%d",&dir6[i]);
      }
  }

  d7 = fopen("eval/d7.txt","r");
  if(d7 == NULL) {
      initArray(2187,dir7); 
      printf("eval/d7.txt を開けませんでした\n");
  }
  else
  {
      for(i=0;i<2187;i++)
      {
        fscanf(d7,"%d",&dir7[i]);
      }
  }

  d8 = fopen("eval/d8.txt","r");
  if(d8 == NULL) {
      initArray(6561,dir8); 
      printf("eval/d8.txt を開けませんでした\n");
  }
  else
  {
      for(i=0;i<6561;i++)
      {
        fscanf(d8,"%d",&dir8[i]);
      }
  }

  h2 = fopen("eval/h2.txt","r");
  if(h2 == NULL) {
      initArray(6561,hor2); 
      printf("eval/h2.txt を開けませんでした\n");
  }
  else
  {
      for(i=0;i<6561;i++)
      {
        fscanf(h2,"%d",&hor2[i]);
      }
  }

  h3 = fopen("eval/h3.txt","r");
  if(h3 == NULL) {
      initArray(6561,hor3); 
      printf("eval/h3.txt を開けませんでした\n");
  }
  else
  {
      for(i=0;i<6561;i++)
      {
        fscanf(h3,"%d",&hor3[i]);
      }
  }

  h4 = fopen("eval/h4.txt","r");
  if(h4 == NULL) {
      initArray(6561,hor4); 
      printf("eval/h4.txt を開けませんでした\n");
  }
  else
  {
      for(i=0;i<6561;i++)
      {
        fscanf(h4,"%d",&hor4[i]);
      }
  }

  cr = fopen("eval/cr.txt","r");
  if(cr == NULL) {
      initArray(6561,cor); 
      printf("eval/cr.txt を開けませんでした\n");
  }
  else
  {
      for(i=0;i<6561;i++)
      {
        fscanf(cr,"%d",&cor[i]);
      }
  }

  eg = fopen("eval/eg.txt","r");
  if(eg == NULL) {
      initArray(6561,edg); 
      printf("eval/eg.txt を開けませんでした\n");
  }
  else
  {
      for(i=0;i<6561;i++)
      {
        fscanf(eg,"%d",&edg[i]);
      }
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
    int i;
    printf("SENTE\n");
    for(i=0;i<64;i++)
    {
        if((board->black>>(63-i)&1) ==1) printf("o");
        else printf("-");
    }
    printf("\n");
    printf("GOTE\n");
    for(i=0;i<64;i++)
    {
        if((board->white>>(63-i)&1) ==1) printf("o");
        else printf("-");
    }
    printf("\n");
}

uint64_t Eval::evalPos(uint64_t legalboard,BitBoard *board)
{
  BitBoard temp;
  int i;
  int index,num,count=0;
  int *sum;
  num = Othero::bitCount(legalboard);
  printf("num=%d\n",num);
  sum = (int *)malloc(sizeof(int)*num);
  printf("legalboard=%llu\n",legalboard);
  for ( i = 0; i < 64; i++)
  {
    if(legalboard&((uint64_t)(1)<<i))
    {
      temp = vput(legalboard&((uint64_t)(1)<<i),board);
      sum[count] = sumeval(&temp);
      //printf("評価値=%d\n",sum[count]);
      count++;
    }
  }
  if (board->teban == SENTE) index = max(sum,num);
  else index = min(sum,num);
  count=0;
  //printf("index=%d\n",index);
  printf("評価値(先手視点) = %d\n",sum[index]);
  free(sum);
  for(i=0;i<64;i++)
  {
    if(legalboard&((uint64_t)(1)<<i))
    {
      //printf("i = %d\n",i);
      if(index==count) return (uint64_t)(1)<<i;
      else count++;
    }
  }
  return 0;
}


