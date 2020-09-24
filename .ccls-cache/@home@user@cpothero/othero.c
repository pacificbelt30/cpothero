/*
オセロの機能
ひっくり返す処理
ビットボード
置く処理
*/
#include<stdio.h>
#include<stdint.h>
#include<stdlib.h>
#include<time.h>
#include <unistd.h>
#include"./othero.h"

//bitcount
//詳しくは分割統治法でggr
int bitCount(uint64_t board){
  board = (board & 0x5555555555555555) + ( (board>> 1) & 0x5555555555555555);
  board = (board & 0x3333333333333333) + ( (board>> 2) & 0x3333333333333333);
  board = (board & 0xF0F0F0F0F0F0F0F ) + ( (board>> 4) & 0xF0F0F0F0F0F0F0F);
  board = (board & 0xFF00FF00FF00FF  ) + ( (board>> 8) & 0xFF00FF00FF00FF);
  board = (board & 0xFFFF0000FFFF    ) + ( (board>>16) & 0xFFFF0000FFFF);
  return  (board & 0xFFFFFFFF        ) + ( (board>>32) & 0xFFFFFFFF);
}

//初期化
void init(BitBoard *board){
	board->black = 0x1008000000;
	board->white = 0x810000000;
	board->teban = SENTE;
}


void show(BitBoard *board){
  int i,j;
  printf(" a b c d e f g h\n");
  printf(" ＿＿＿＿＿＿＿＿\n");
  for ( i = 0; i < 8; i++)
  {
    printf("|");
    //一列出力
    for ( j = 0; j < 8; j++)
    {
      if (((uint64_t)1<<(63-(i*8+j))& board->black)!=0)
      {
        printf("o ");
      }
      else if (((uint64_t)1<<(63-(i*8+j))& board->white)!=0)
      {
        printf("x ");
      }
      else
      {
        printf("- ");
      }
       
    }
    printf("| %d\n",i+1);
  }
  
  printf(" ＿＿＿＿＿＿＿＿\n");
  fflush(stdout);
}


//与えられたposから裏返る石の座標を返す
uint64_t reverse(uint64_t pos, BitBoard *board){
  int i;
  uint64_t revd_board=0;
  uint64_t enemy,me,tmp=0;

  //手番判定
  if ( board->teban == SENTE )
  {
    enemy=board->white;
    me = board->black;
  }
  else
  {
    enemy=board->black;
    me = board->white;
  }

  //右
  tmp=0;
  for ( i = 1; ((pos >> i) & MASK_HORIZONTAL & enemy) !=0 ; i++)
  {
    tmp = tmp|(pos>>i);
  }
  if (((pos>>i)&me) != 0)
  {
    revd_board = revd_board | tmp; 
  }
  
  //左
  tmp=0;
  for ( i = 1; ((pos << i) & MASK_HORIZONTAL & enemy) !=0 ; i++)
  {
    tmp = tmp|(pos<<i);
  }
  if (((pos<<i)&me) != 0)
  {
    revd_board = revd_board | tmp; 
  }
  
  //上
  tmp=0;
  for ( i = 1; ((pos << (i*8)) & MASK_VERTICAL & enemy) !=0 ; i++)
  {
    tmp = tmp|(pos<<(i*8));
  }
  if ((pos<<(i*8)&me) != 0)
  {
    revd_board = revd_board | tmp; 
  }

  //下
  tmp=0;
  for ( i = 1; ((pos >> (i*8)) & MASK_VERTICAL & enemy) !=0 ; i++)
  {
    tmp = tmp|(pos>>(i*8));
  }
  if ((pos>>(i*8)&me) != 0)
  {
    revd_board = revd_board | tmp; 
  }
  
  //右上
  tmp=0;
  for ( i = 1; ((pos << (i*7)) & MASK_DIAGONAL & enemy) !=0 ; i++)
  {
    tmp = tmp|(pos<<(i*7));
  }
  if ((pos<<(i*7)&me) != 0)
  {
    revd_board = revd_board | tmp; 
  }
  
  //左上
  tmp=0;
  for ( i = 1; ((pos << (i*9)) & MASK_DIAGONAL & enemy) !=0 ; i++)
  {
    tmp = tmp|(pos<<(i*9));
  }
  if ((pos<<(i*9)&me) != 0)
  {
    revd_board = revd_board | tmp; 
  }
  
  //右下
  tmp=0;
  for ( i = 1; ((pos >> (i*9)) & MASK_DIAGONAL & enemy) !=0 ; i++)
  {
    tmp = tmp|(pos>>(i*9));
  }
  if ((pos>>(i*9)&me) != 0)
  {
    revd_board = revd_board | tmp; 
  }

  //左下
  tmp=0;
  for ( i = 1; ((pos >> (i*7)) & MASK_DIAGONAL & enemy) !=0 ; i++)
  {
    tmp = tmp|(pos>>(i*7));
  }
  if ((pos>>(i*7)&me) != 0)
  {
    revd_board = revd_board | tmp; 
  }

  return revd_board;
}

//合法手の数を返す関数
uint64_t canReverse(BitBoard *board)
{
  int i;
  uint64_t legalboard=0,empty,temp;
  empty = ~(board->black | board->white);//空きマス
  for ( i = 0; i < 64; i++)
  {
    temp = (empty&((uint64_t)1<<i));//i番目のマスが空いているかどうか(空いている場合≠0)
    if (temp!=0)//空いている場合の処理
    {
      if (reverse(temp,board )!=0)//そのマスに置いたとき裏返せるなら
      {
        legalboard = legalboard | temp;//合法手に追加
      }
    }
    
  }
  //学習時に邪魔なので切る
  //printf("legel = %llu\n",bitCount(legalboard));//合法手の盤面
  return legalboard;
}

int checkGameover(BitBoard *board)
{
  //PASS:1
  //GAMEOVER:2
  //埋まったマス
  uint64_t vdmasu=board->black|board->white;
  if(~(vdmasu)==0)//空きマスがない場合
  {
    return GAME_OVER;
  }
  //else if(~(vdmasu&board->black )==0 | ~(vdmasu&board->white)==0)//片方の色で埋まっている
  else if((vdmasu&~(board->black) )==0 | (vdmasu&~(board->white))==0)//片方の色で埋まっている
  {
    return GAME_OVER;
  }
  else if(canReverse(board)==0)
  { 
      board->teban*=(-1);
      //inverseTEBAN(board);
      if(canReverse(board)==0)
      {
          board->teban*=(-1);
          //inverseTEBAN(board);
          return GAME_OVER;
      }
      board->teban*=(-1);
      //inverseTEBAN(board);
  }
  return 0;
}

void inverseTEBAN(BitBoard *board)
{
  if(board->teban == SENTE) board->teban = GOTE;
  else board->teban = SENTE;
  return ;
}

//置く処理
//reverseと同様の処理をしている場所を書き直す
uint64_t put(uint64_t pos,BitBoard *board)
{
  int i;
  uint64_t revd_board;//裏返った盤面
  uint64_t *enemy;
  uint64_t *me;

  if ( board->teban == SENTE )
  {
    enemy=&board->white;
    me = &board->black;
  }
  else
  {
    enemy=&board->black;
    me = &board->white;
  }

  revd_board = reverse(pos,board);//裏返った盤面
  *me ^= pos | revd_board;//自分盤面更新
  *enemy ^= revd_board;//相手盤面更新
  
  return 0;
}

//結果表示
void result(BitBoard *board)
{
  printf("黒石%d個\n",bitCount(board->black));
  printf("白石%d個\n",bitCount(board->white));
  //printf("黒石%d個\n",bitCount(board->black)-bitCount(board->white));
  if (bitCount(board->black) >= bitCount(board->white) )
  {
    printf("黒番 %d石差で勝ち\n",bitCount(board->black)-bitCount(board->white));
  }
  else
  {
    printf("白番 %d石差で勝ち\n",bitCount(board->white)-bitCount(board->black));
  }
  
  
}
uint64_t inputPos() {
  char retu;
  int gyo;
  printf("座標の入力(例：e3)\n");
  fflush(stdout);
  scanf("%c%d",&retu,&gyo);
  if('a' > retu || retu > 'h') return inputPos();
  else if(1 > gyo || gyo > 8) return inputPos();
  //printf("%llu\n",((uint64_t)1<<((7-(int)(retu-'a'))+8*(8-gyo))));
  return ((uint64_t)1<<((7-(int)(retu-'a'))+8*(8-gyo)));
}

int checkLeagal(BitBoard board)
{
  if((board.black&board.white)==0) return 1;
  else return 0;
}
