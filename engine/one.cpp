#include<stdio.h>
#include<stdint.h>
#include<stdlib.h>
#include<time.h>
#include <unistd.h>
#include"../eval.h"
#include"../othero.h"
#include"../learn.h"
#include"./one.h"

//一手読み
//te 先手，後手
void One::one(int te)
{
  //手順：　先手で
  //指す->入力される->合法手判断->適用(再入力)->繰り返し
  int i,count=1;
  int gyo;
  char retu;
  uint64_t pos,legal;
  BitBoard board;
  //Eval eval;
  Othero::init(&board);
  Othero::show(&board);
  printf("試合開始\n");
  fflush(stdout);
  //eval.openeval();
  //対戦部分(人間先手1，後手-1)
  if(te==-1)
  {
    while(Othero::checkGameover(&board)!=GAME_OVER&&count<100)
    {
      if (board.teban==SENTE)
      {
        //ai側
        printf("sente\n");
        fflush(stdout);
        legal=Othero::canReverse(&board);
        if(!legal){
          board.teban = GOTE;
          //count++;
          continue;
        }
        else
        {
          pos = eval.evalPos(legal,&board);
          //printf("evalPos=%d\n",pos);
          if (!pos) return;
          Othero::put(pos,&board);
        }
        Othero::show(&board);
        board.teban=GOTE;
      }
      else
      {
        //人側
        printf("gote\n");
        fflush(stdout);
        pos = Othero::canReverse(&board) & Othero::inputPos();
        legal=Othero::canReverse(&board);
        if(!legal){
          board.teban = SENTE;
          //count++;
          continue;
        }
        if(!pos)
        {
          continue;
        }

        Othero::put(pos,&board);
        Othero::show(&board);
        board.teban = SENTE;
      }

      count++;
      printf("%d手\n",count);
    }
  }
  else if(te==1)//(te=1)の処理
  {
    while(Othero::checkGameover(&board)!=GAME_OVER&&count<100)
    {
      if (board.teban==SENTE)
      {
        //人側
        printf("sente\n");
        fflush(stdout);
        pos = Othero::canReverse(&board) & Othero::inputPos();
        legal=Othero::canReverse(&board);
        if(!legal){
          board.teban = GOTE;
          //count++;
          continue;
        }
        if(!pos)
        {
          continue;
        }
  
        Othero::put(pos,&board);
        Othero::show(&board);
        board.teban = GOTE;
      }
      else
      {
        //ai側
        printf("gote\n");
        fflush(stdout);
        legal=Othero::canReverse(&board);
        if(!legal){
          board.teban = SENTE;
          //count++;
          continue;
        }
        else
        {
          pos = eval.evalPos(legal,&board);
          //printf("evalPos=%d\n",pos);
          if (!pos) return;
          Othero::put(pos,&board);
        }
        Othero::show(&board);
        board.teban=SENTE;
      }

  
      count++;
      printf("%d手\n",count);
    }
  }
  else if(te==99)
  {
    
    while(Othero::checkGameover(&board)!=GAME_OVER&&count<100)
    {      
      if (board.teban==SENTE)
      {
        //ai側
        printf("sente\n");
        fflush(stdout);
        legal=Othero::canReverse(&board);
        if(!legal){
          board.teban = GOTE;
          //count++;
          continue;
        }
        else
        {
          pos = eval.evalPos(legal,&board);
          if (!pos) return;
          Othero::put(pos,&board);
        }
        Othero::show(&board);
        board.teban=GOTE;
      }
      else
      {
        //ai側
        printf("gote\n");
        fflush(stdout);
        legal=Othero::canReverse(&board);
        if(!legal){
          board.teban = SENTE;
          //count++;
          continue;
        }
        else
        {
          pos = eval.evalPos(legal,&board);
          if (!pos) return;
          Othero::put(pos,&board);
        }
        Othero::show(&board);
        board.teban=SENTE;
      }

  
      count++;
      printf("%d手\n",count);
    }
  }
  
  printf("%d手\n",count);
  Othero::result(&board);
  return;
}

//置く処理
//reverseと同様の処理をしている場所を書き直す
BitBoard vput(uint64_t pos,BitBoard *board)
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
  
  return vi;
}

int max(int array[],int n)
{
  int i,nummax=0;
  int maxvalue=array[0];
  for(i=1;i<n;i++)
  {
    if(maxvalue < array[i]) {
      maxvalue = array[i];
      nummax = i;
    }
  }
  return nummax;
}

int min(int array[],int n)
{
  int i,nummax=0;
  int maxvalue=array[0];
  for(i=1;i<n;i++)
  {
    if(maxvalue > array[i]) {
      maxvalue = array[i];
      nummax = i;
    }
  }
  return nummax;
}

string One::woi(){
  return "ittedakeyomu";
}

uint64_t One::go(){
  BitBoard tmp = this->getboard();
  return bestPos(tmp);
  // return true;
}

uint64_t One::bestPos(BitBoard board){
  uint64_t legal = Othero::canReverse(&board);
  uint64_t pos = eval.evalPos(legal,&board);
  return pos;
}
