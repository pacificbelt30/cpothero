#include<iostream>
#include<fstream>
#include<stdio.h>
#include<stdint.h>
#include<stdlib.h>
#include<time.h>
#include <unistd.h>
#include "./othero.h"
#include "./learn.h"
#include "./solve.h"
#include "./eval.h"
#include "./engine/rand_ai.h"
#include "./engine/one.h"
using namespace std;


//追加
//NULLチェック済み
void Learn::writeeval(FILE *fp,int n,int pat[])
{
  int i;
  for ( i = 0; i < n; i++)
  {
    fprintf(fp,"%d ",pat[i]);
  }
  return;
  
}

//棋譜を作る
//上限64手
void Learn::generateKif(int n)
{
  //int kif[64];
  int kif[64];
  int i,taikyoku;
  int count=0;
  int resu;
  uint64_t pos,legal;
  BitBoard board;
  FILE *fp;
  fp = fopen("kif/100.txt","w");
  cout << "ファイルを開く" << endl;
  //createtable();//最右端ビット用テーブルの初期化
  if (fp==NULL)
  {
    cout << "ファイルを開けませんでした" << endl;
    return ;
  }
  cout << "対局数書き込み" << endl;
  fprintf(fp,"%d \n",n);
  //fprintf(fp,"対局数 %d \n",n);
  //n回対局
  for(taikyoku = 0;taikyoku<n;taikyoku++)
  {
    //usleep(1);
    cout << "初期化" << endl;
    Othero::init(&board);
    count=0;
    //fprintf(fp,"第%d局目",taikyoku+1);
    cout << "第"<< taikyoku + 1 << "局" << endl;
    for ( i = 0; i < 64; i++)
    {
      kif[i]=0;
    }
    while(Othero::bitCount(~(board.black|board.white))>=SOLVE_DEPTH&&Othero::checkGameover(&board)!=GAME_OVER&&count<64)
    {      
      //最終完全よみ（仮）
      /*
      if(bitCount(~(board.black|board.white))<=(SOLVE_DEPTH))
      {
        resu=(board.teban)*solver(board);
        fprintf(fp,"%d ",resu);
        //fprintf(fp,"石差 %d ",resu);
        cout << "石差" << resu << endl;
        for(i=0;i<64;i++)
        {
          fprintf(fp,"%d ",kif[i]);
        }
        fprintf(fp,"\n");
        break;
      }*/

      if (board.teban==SENTE)
      {
        //ai側
        cout << "sente" << endl;
        fflush(stdout);
        legal=Othero::canReverse(&board);
        if(!legal){
          board.teban = GOTE;
          count++;
          continue;
        }
        else
        {
          //pos = randPos(legal);
          if(count<10)pos = randPos(legal);
          else pos = evalPos(legal,&board);
          if (!pos) return;
          Othero::put(pos,&board);
          //kif[count] = getRMB(pos);//kif保存
          kif[count] = getPosnum(pos);//kif保存
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
          count++;
          continue;
        }
        else
        {
          //pos = randPos(legal);
          if(count<10)pos = randPos(legal);
          else pos = evalPos(legal,&board);
          if (!pos) return;
          Othero::put(pos,&board);
          //kif[count] = getRMB(pos);//kif保存
          kif[count] = getPosnum(pos);//kif保存
        }
        Othero::show(&board);
        board.teban=SENTE;
      }
    
      count++;
      cout << count << "手" << endl;
    }
    resu=(board.teban)*Solve::solver(board);
    //resu = bitCount( board.black) - bitCount(board.white);
    fprintf(fp,"%d ",resu);
    //fprintf(fp,"石差 %d ",resu);
    cout << "石差" << resu << endl;
    for(i=0;i<64;i++)
    {
      fprintf(fp,"%d ",kif[i]);
    }
    fprintf(fp,"\n");
  }
  fclose(fp);
  return;
}


//posが何番目のビットに立っているか
int Learn::getPosnum(uint64_t pos)
{
    int count=0;
    if((pos>>63)==1) return 64;
    while((pos>>count)!=0)
    {
        ++count;
    }
    return count;
}


//棋譜を読み込むよう
int **Learn::makeKifArray(int n)
{
  int **kif;
  int i;
  kif = (int **)malloc(sizeof(int *)*n);
  for ( i = 0; i < n; i++)
  {
    kif[i] = (int *)malloc(sizeof(int)*64);
  }
  return kif;
}


/*
棋譜FILEを開く
評価関数の初期化
最初なので全部０
*/
void Learn::learning()
{
  double rate = 0.0003;
  BitBoard board;
  int i,j;
  int taikyoku;
  int **kif;
  int *sekisa;
  FILE *fp;
  FILE *d4,*d5,*d6,*d7,*d8,*h2,*h3,*h4,*cr,*eg;

  //読み込むkifファイル
  fp = fopen("kif/100.txt","r");
/*
  initArray(6561,hor2); 
  initArray(6561,hor3); 
  initArray(6561,hor4); 
  initArray(81,dir4); 
  initArray(243,dir5); 
  initArray(729,dir6); 
  initArray(2187,dir7); 
  initArray(6561,dir8); 
  initArray(6561,cor); 
  initArray(6561,edg); 
  */
  openeval();

  cout << "評価関数初期化" << endl;
  fflush(stdout);


  d4 = fopen("eval/d4.txt","w");
  if(d4 == NULL) return ;
  d5 = fopen("eval/d5.txt","w");
  if(d5 == NULL) return ;
  d6 = fopen("eval/d6.txt","w");
  if(d6 == NULL) return ;
  d7 = fopen("eval/d7.txt","w");
  if(d7 == NULL) return ;
  d8 = fopen("eval/d8.txt","w");
  if(d8 == NULL) return ;
  h2 = fopen("eval/h2.txt","w");
  if(h2 == NULL) return ;
  h3 = fopen("eval/h3.txt","w");
  if(h3 == NULL) return ;
  h4 = fopen("eval/h4.txt","w");
  if(h4 == NULL) return ;
  cr = fopen("eval/cr.txt","w");
  if(cr == NULL) return ;
  eg = fopen("eval/eg.txt","w");
  if(eg == NULL) return ;

  cout << "ファイルを開いた" << endl;
  fflush(stdout);
  fscanf(fp,"%d",&taikyoku);
  cout << "対局数を読み取る" << endl;
  fflush(stdout);
  kif = makeKifArray(taikyoku);
  cout << "kifポインタ" << endl;
  fflush(stdout);
  sekisa = (int *)malloc((sizeof(int)*taikyoku));
  cout << "このあと入力" << endl;
  fflush(stdout);
  for(i=0;i<taikyoku;i++)
  {
    fscanf(fp,"%d",&sekisa[i]);
    for(j=0;j<64;j++)
    {
      fscanf(fp,"%d",&kif[i][j]);
    }
  }

  cout << "入力終了" << endl;
  fflush(stdout);
  Othero::init(&board);

  cout << "board初期化" << endl;
  fflush(stdout);
//segmentaion fault
  for ( i = 0; i < taikyoku; i++)
  {
    cout << "第" << i << "局" << endl;
    Othero::init(&board);
    for (j = 0; j < 64; j++)
    {
      if(j%2==0)
      {//SENTE
        //put(kif[i][j],&board);
        if(kif[i][j]!=0) Othero::put((uint64_t)1<<(kif[i][j]-1),&board);//uint64_tじゃない
        cout << "DEBUG BEFORE UPDATE" << endl;
        updateeval((int)(rate*((double)(-sumeval(&board)+1000*sekisa[i]))),&board);

        cout << "SEKISA = " << sekisa[i] << " : SUMEVAL =  " << sumeval(&board) << endl;
        board.teban=GOTE;
        showBitboard(&board);
        fflush(stdout);
      }
      else
      {//GOTE
        //put(kif[i][j],&board);
        if(kif[i][j]!=0) Othero::put((uint64_t)1<<(kif[i][j]-1),&board);//uint64_tじゃない
        cout << "DEBUG BEFORE UPDATE" << endl;
        //updateeval((int)((-1)*rate*((double)(-sumeval(&board)+1000*sekisa[i]))),&board);
        updateeval((int)(rate*((double)(-sumeval(&board)+1000*sekisa[i]))),&board);
        board.teban=SENTE;
        cout << "SEKISA = " << sekisa[i] << " : SUMEVAL =  " << sumeval(&board) << endl;
        showBitboard(&board);
        fflush(stdout);
      }
      
    }
    
  }
  

  cout << "解析終了" << endl;
  fflush(stdout);
  free(sekisa);
  cout << "free 石差終わり" << endl;
  fflush(stdout);
  for ( i = 0; i < taikyoku; i++)
  {
    free(kif[i]);
  }
  cout << "free終わり" << endl;
  fflush(stdout);
  free(kif);
  cout << "書き込み開始終わり" << endl;
  fflush(stdout);
  
  writeeval(h2,6561,hor2); 
  writeeval(h3,6561,hor3); 
  writeeval(h4,6561,hor4); 
  writeeval(d4,81,dir4); 
  writeeval(d5,243,dir5); 
  writeeval(d6,729,dir6); 
  writeeval(d7,2187,dir7); 
  writeeval(d8,6561,dir8); 
  writeeval(cr,6561,cor); 
  writeeval(eg,6561,edg); 

  return;
}


void Learn::updateeval(int point,BitBoard *board)
{
  hor2[sumhor2(board,1)]+=point;//3^8 = 6561
  hor2[sumhor2(board,2)]+=point;//3^8 = 6561
  hor2[sumhor2(board,3)]+=point;//3^8 = 6561
  hor2[sumhor2(board,4)]+=point;//3^8 = 6561

  hor3[sumhor3(board,1)]+=point;//3^8 = 6561
  hor3[sumhor3(board,2)]+=point;//3^8 = 6561
  hor3[sumhor3(board,3)]+=point;//3^8 = 6561
  hor3[sumhor3(board,4)]+=point;//3^8 = 6561

  hor4[sumhor4(board,1)]+=point;//3^8 = 6561
  hor4[sumhor4(board,2)]+=point;//3^8 = 6561
  hor4[sumhor4(board,3)]+=point;//3^8 = 6561
  hor4[sumhor4(board,4)]+=point;//3^8 = 6561

  dir4[sumdir4(board,1)]+=point;//3^4 = 81
  dir4[sumdir4(board,2)]+=point;//3^4 = 81
  dir4[sumdir4(board,3)]+=point;//3^4 = 81
  dir4[sumdir4(board,4)]+=point;//3^4 = 81

  dir5[sumdir5(board,1)]+=point;//3^5 = 243
  dir5[sumdir5(board,2)]+=point;//3^5 = 243
  dir5[sumdir5(board,3)]+=point;//3^5 = 243
  dir5[sumdir5(board,4)]+=point;//3^5 = 243

  dir6[sumdir6(board,1)]+=point;//3^6 = 729
  dir6[sumdir6(board,2)]+=point;//3^6 = 729
  dir6[sumdir6(board,3)]+=point;//3^6 = 729
  dir6[sumdir6(board,4)]+=point;//3^6 = 729

  dir7[sumdir7(board,1)]+=point;//3^7 = 2187
  dir7[sumdir7(board,2)]+=point;//3^7 = 2187
  dir7[sumdir7(board,3)]+=point;//3^7 = 2187
  dir7[sumdir7(board,4)]+=point;//3^7 = 2187

  dir8[sumdir8(board,1)]+=point;//3^8 = 6561
  dir8[sumdir8(board,2)]+=point;//3^8 = 6561

  cor[sumcor(board,1)]+=point;//3^8 = 6561
  cor[sumcor(board,2)]+=point;//3^8 = 6561
  cor[sumcor(board,3)]+=point;//3^8 = 6561
  cor[sumcor(board,4)]+=point;//3^8 = 6561

  edg[sumedg(board,1)]+=point;//3^8 = 6561
  edg[sumedg(board,2)]+=point;//3^8 = 6561
  edg[sumedg(board,3)]+=point;//3^8 = 6561
  edg[sumedg(board,4)]+=point;//3^8 = 6561

  return;
}
