#include<stdio.h>
#include<stdint.h>
#include<stdlib.h>
#include<time.h>
#include <unistd.h>
#include"../eval.h"
#include"../othero.h"
#include"../learn.h"
#include"./rand_ai.h"

void randAi(int te)
{
  //手順：　先手で
  //指す->入力される->合法手判断->適用(再入力)->繰り返し
  int i,count=1;
  int gyo;
  char retu;
  uint64_t pos,legal;
  BitBoard board;
  init(&board);
  show(&board);
  printf("試合開始\n");
  fflush(stdout);
  //対戦部分(人間先手1，後手-1)
  if(te==1)
  {
    while(checkGameover(&board)!=GAME_OVER&&count<100)
    {
      if (board.teban==SENTE)
      {
        //ai側
        printf("sente\n");
        fflush(stdout);
        legal=canReverse(&board);
        if(!legal){
          board.teban = GOTE;
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
        board.teban=GOTE;
      }
      /*
      else
      {
        //ai側
        printf("gote\n");
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
        printf("gote\n");
        fflush(stdout);
        pos = canReverse(&board) & inputPos();
        legal=canReverse(&board);
        if(!legal){
          board.teban = SENTE;
          //count++;
          continue;
        }
        if(!pos)
        {
          continue;
        }

        put(pos,&board);
        show(&board);
        board.teban = SENTE;
      }

      count++;
      printf("%d手\n",count);
    }
  }
  else if(te==1)//(te=1)の処理
  {
    while(checkGameover(&board)!=GAME_OVER&&count<100)
    {
      if (board.teban==SENTE)
      {
        //人側
        printf("sente\n");
        fflush(stdout);
        pos = canReverse(&board) & inputPos();
        legal=canReverse(&board);
        if(!legal){
          board.teban = GOTE;
          //count++;
          continue;
        }
        if(!pos)
        {
          continue;
        }
  
        put(pos,&board);
        show(&board);
        board.teban = GOTE;
      }
      else
      {
        //ai側
        printf("gote\n");
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
      }

  
      count++;
      printf("%d手\n",count);
    }
  }
  else if(te==99)
  {
    
    while(checkGameover(&board)!=GAME_OVER&&count<100)
    {      
      if (board.teban==SENTE)
      {
        //ai側
        printf("sente\n");
        fflush(stdout);
        legal=canReverse(&board);
        if(!legal){
          board.teban = GOTE;
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
        board.teban=GOTE;
      }
      else
      {
        //ai側
        printf("gote\n");
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
      }

  
      count++;
      printf("%d手\n",count);
    }
  }
  
  printf("%d手\n",count);
  result(&board);
  return;
}


uint64_t randPos(uint64_t legalboard)
{
  int index,count=0,i,num;
  num = bitCount(legalboard);
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

