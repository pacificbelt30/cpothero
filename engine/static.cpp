#include<iostream>
#include<stdio.h>
#include<stdint.h>
#include<stdlib.h>
#include<time.h>
#include <unistd.h>
#include"../eval.h"
#include"../othero.h"
#include"../learn.h"
#include"./one.h"
#include"./static.h"
using namespace std;

//一手読み
//te 先手，後手
void Static::one(int te)
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
  cout << "試合開始" << endl;
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
        cout << "sente" << endl;
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
          //cout << "evalPos=" << pos << endl;
          if (!pos) return;
          Othero::put(pos,&board);
        }
        Othero::show(&board);
        board.teban=GOTE;
      }
      else
      {
        //人側
        cout << "gote" << endl;
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
      cout << count << "手" << endl;
    }
  }
  else if(te==1)//(te=1)の処理
  {
    while(Othero::checkGameover(&board)!=GAME_OVER&&count<100)
    {
      if (board.teban==SENTE)
      {
        //人側
        cout << "sente" << endl;
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
        cout << "gote" << endl;
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
          //cout << "evalPos=" << pos << endl;
          if (!pos) return;
          Othero::put(pos,&board);
        }
        Othero::show(&board);
        board.teban=SENTE;
      }

  
      count++;
      cout << count << "手" << endl;
    }
  }
  else if(te==99)
  {
    
    while(Othero::checkGameover(&board)!=GAME_OVER&&count<100)
    {      
      if (board.teban==SENTE)
      {
        //ai側
        cout << "sente" << endl;
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
        cout << "gote" << endl;
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
      cout << count << "手" << endl;
    }
  }
  
  cout << count << "手" << endl;
  Othero::result(&board);
  return;
}

string Static::woi(){
  //return "ittedakeyomu";
  return to_string(eval.dir4[0]);
}

uint64_t Static::go(){
  BitBoard tmp = this->getboard();
  return bestPos(tmp);
  // return true;
}

uint64_t Static::bestPos(BitBoard board){
  BitBoard tmp = board;
  uint64_t legal = Othero::canReverse(&board);
  int legalnum = Othero::bitCount(legal);

  uint64_t pos = eval.evalPos(legal,&board);
  return pos;
}

uint64_t Static::search(BitBoard board,int depth){
  int best = (-1)*INFINITY;
  uint64_t bestPos;
  int val = 0;
  BitBoard tmp = board;
  uint64_t legal = Othero::canReverse(&board);
  int legalnum = Othero::bitCount(legal);
  for(i=0; i<64; i++)
  {
    if(legalnum<=count) break;
    else if((legal&((uint64_t)(1)<<i))!=0)
    {
      val = eval(Othero::vput((uint64_t)(1) << i,&board),DEPTH);
      if(best<val) {best = val; bestPos = (uint64_t)(1) << i}
    }
  }
  return bestPos;
}

int eval(BitBoard board,int depth){
  int best = (-1)*INFINITY;
  BitBoard tmp = board;
  uint64_t legal = Othero::canReverse(&board);
  int legalnum = Othero::bitCount(legal);
  if(depth == 0){
  }
  if(Othero::checkGameover(&board) =- GAME_OVER){
  }
  if(legalnum == 0){
    Othero::inverseTEBAN(&tmp);
    eval(-1,tmp,depth-1);
    Othero::inverseTEBAN(&tmp);
    if(best<val) best = val;
    return best;
  }
  for(i=0; i<64; i++)
  {
    if(legalnum<=count) break;
    else if((legal&((uint64_t)(1)<<i))!=0)
    {
      count++;
      //BitBoard temp = board;
      tmp = Othero::vput((uint64_t)(1)<<i,&board);
      tmp.teban = board.teban;
      Othero::inverseTEBAN(&temp);
      Othero::inverseTEBAN(&temp);
      if(best<val) best = val;
    }
  }
}
