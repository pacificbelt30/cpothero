/*
オセロの機能
ひっくり返す処理
ビットボード
置く処理
*/
#include<iostream>
#include<stdio.h>
#include<stdint.h>
#include<stdlib.h>
#include<time.h>
#include <unistd.h>
#include"./othero.h"
#include "./env.h"
using namespace std;

//Othero::Othero(){};
//Othero::~Othero(){};

//bitcount
//詳しくは分割統治法でggr
int Othero::bitCount(uint64_t board){
  board = (board & 0x5555555555555555) + ( (board>> 1) & 0x5555555555555555);
  board = (board & 0x3333333333333333) + ( (board>> 2) & 0x3333333333333333);
  board = (board & 0xF0F0F0F0F0F0F0F ) + ( (board>> 4) & 0xF0F0F0F0F0F0F0F);
  board = (board & 0xFF00FF00FF00FF  ) + ( (board>> 8) & 0xFF00FF00FF00FF);
  board = (board & 0xFFFF0000FFFF    ) + ( (board>>16) & 0xFFFF0000FFFF);
  return  (board & 0xFFFFFFFF        ) + ( (board>>32) & 0xFFFFFFFF);
}

//初期化
void Othero::init(BitBoard *board){
//  board->black = 0x1008000000;
//  board->white = 0x810000000;
  board->black = INITBLACK;
  board->white = INITWHITE;
  board->teban = INITTEBAN;
}


void Othero::show(BitBoard *board){
  int i,j;
  cout << " a b c d e f g h" << endl;
  cout << " ＿＿＿＿＿＿＿＿" << endl;
  for ( i = 0; i < 8; i++)
  {
    cout << "|";
    //一列出力
    for ( j = 0; j < 8; j++)
    {
      if (((uint64_t)1<<(63-(i*8+j))& board->black)!=0)
      {
        cout << "o ";
      }
      else if (((uint64_t)1<<(63-(i*8+j))& board->white)!=0)
      {
        cout << "x ";
      }
      else
      {
        cout << "- ";
      }
       
    }
    cout << "| " << i+1 << endl;
  }
  
  cout << " ＿＿＿＿＿＿＿＿" << endl;
  fflush(stdout);
}


//与えられたposから裏返る石の座標を返す
uint64_t Othero::reverse(uint64_t pos, BitBoard *board){
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
uint64_t Othero::canReverse(BitBoard *board)
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
  //cout << "legel = " << bitCount(legalboard) << endl;//合法手の盤面
  return legalboard;
}

int Othero::checkGameover(BitBoard *board)
{
  //PASS:1
  //GAMEOVER:2
  //埋まったマス
  BitBoard temp = *board;
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
  //else if(Othero::bitCount(canReverse(board))==0)
  else if(Othero::bitCount(canReverse(&temp))==0)
  { 
    //board->teban*=(-1);
    //inverseTEBAN(board);
    inverseTEBAN(&temp);
    
    //if(Othero::bitCount(canReverse(board))==0)
    if(Othero::bitCount(canReverse(&temp))==0)
    {
      //board->teban*=(-1);
      //inverseTEBAN(board);
      inverseTEBAN(&temp);
      return GAME_OVER;
    }
    //board->teban*=(-1);
    //inverseTEBAN(board);
    inverseTEBAN(&temp);
    return PASS;
  }
  return 0;
}

void Othero::inverseTEBAN(BitBoard *board)
{
  if(board->teban == SENTE) board->teban = GOTE;
  else board->teban = SENTE;
  return ;
}

//置く処理
//reverseと同様の処理をしている場所を書き直す
uint64_t Othero::put(uint64_t pos,BitBoard *board)
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
void Othero::result(BitBoard *board)
{
  cout << "黒石 " << bitCount(board->black) << "個" << endl;
  cout << "白石 " << bitCount(board->white) << "個" << endl;
  //printf("黒石%d個\n",bitCount(board->black)-bitCount(board->white));
  if (bitCount(board->black) >= bitCount(board->white) )
  {
    cout << "黒番 " << bitCount(board->black)-bitCount(board->white) << "石差で勝ち" << endl;
  }
  else
  {
    cout << "白番 " << bitCount(board->white)-bitCount(board->black) << "石差で勝ち" << endl;
  }
  
  
}
uint64_t Othero::inputPos() {
  char retu;
  int gyo;
  cout << "座標の入力(例：e3)" << endl;
  fflush(stdout);
  scanf("%c%d",&retu,&gyo);
  if('a' > retu || retu > 'h') return inputPos();
  else if(1 > gyo || gyo > 8) return inputPos();
  //cout << ((uint64_t)1<<((7-(int)(retu-'a'))+8*(8-gyo))) << endl;
  return ((uint64_t)1<<((7-(int)(retu-'a'))+8*(8-gyo)));
}

int Othero::checkLeagal(BitBoard board)
{
  if((board.black&board.white)==0) return 1;
  else return 0;
}

//置く処理
//reverseと同様の処理をしている場所を書き直す
BitBoard Othero::vput(uint64_t pos,BitBoard *board)
{
  int i;
  BitBoard vi;//teban関係なし
  uint64_t revd_board;//裏返った盤面
  uint64_t *enemy;
  uint64_t *me;
  vi.white = board->white;
  vi.black = board->black;

  if ( board->teban == SENTE )
  {
    enemy=&vi.white;
    me = &vi.black;
  }
  else
  {
    enemy=&vi.black;
    me = &vi.white;
  }

  revd_board = Othero::reverse(pos,board);//裏返った盤面
  *me ^= (pos | revd_board);//自分盤面更新
  *enemy ^= revd_board;//相手盤面更新
  //cerr << "DEBUG_inverseTEBAN before"<< " teban:" << (int)(vi.teban) <<endl;
  inverseTEBAN(&vi);//手番逆転
  //cerr << "DEBUG_inverseTEBAN after"<< " teban:" << (int)(vi.teban) <<endl;
  return vi;
}
