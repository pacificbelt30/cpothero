#include<iostream>
#include<stdio.h>
#include<stdint.h>
#include<stdlib.h>
#include<time.h>
#include <unistd.h>
#include"../eval.h"
#include"../othero.h"
#include"../learn.h"
#include"./rand_ai.h"
using namespace std;

void randAi(int te)
{
  //手順：　先手で
  //指す->入力される->合法手判断->適用(再入力)->繰り返し
  int i,count=1;
  int gyo;
  char retu;
  uint64_t pos,legal;
  BitBoard board;
  Othero::init(&board);
  Othero::show(&board);
  cout << "試合開始" << endl;
  fflush(stdout);
  //対戦部分(人間先手1，後手-1)
  if(te==1)
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
          pos = randPos(legal);
          if (!pos) return;
          Othero::put(pos,&board);
        }
        Othero::show(&board);
        board.teban=GOTE;
      }
      /*
      else
      {
        //ai側
        cout << "gote" << endl;
        fflush(stdout);
        legal=canReverse(&board);
        if(!legal){
          board.teban = SENTE;
          //count++;
          continue;
        }
        else
        {
          pos = randPos(legal);
          if (!pos) return;
          put(pos,&board);
        }
        show(&board);
        board.teban=SENTE;
      }*/
      
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
          pos = randPos(legal);
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
          pos = randPos(legal);
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
          pos = randPos(legal);
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


uint64_t randPos(uint64_t legalboard)
{
  int index,count=0,i,num;
  num = Othero::bitCount(legalboard);
  //srand((unsigned int)time(NULL)); // 現在時刻の情報で初期化
  index = rand() % num;//1から合法手の数までの乱数
  for ( i = 0; i < 64; i++)
  {
    if(legalboard&((uint64_t)1<<i))
    {
      if(count==index) return ((uint64_t)1<<i);//index番目の合法手を返す
      else count++;
    }
  }
  return 0;
}

uint64_t RandAIEngine::randPos(uint64_t legalboard)
{
  int index,count=0,i,num;
  num = Othero::bitCount(legalboard);
  //srand((unsigned int)time(NULL)); // 現在時刻の情報で初期化
  index = rand() % num;//1から合法手の数までの乱数
  for ( i = 0; i < 64; i++)
  {
    if(legalboard&((uint64_t)1<<i))
    {
      if(count==index) return ((uint64_t)1<<i);//index番目の合法手を返す
      else count++;
    }
  }
  return 0;
}

uint64_t RandAIEngine::go(){
  BitBoard tmp = this->getboard();
  return randPos(Othero::canReverse(&tmp));
  // return true;
}

string RandAIEngine::woi(){
  return "RandAI";
}

