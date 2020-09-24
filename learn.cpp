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


//NULLチェック済み
void writeeval(FILE *fp,int n,int pat[])
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
void generateKif(int n)
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
  printf("ファイルを開く\n");
  //createtable();//最右端ビット用テーブルの初期化
  if (fp==NULL)
  {
    printf("ファイルを開けませんでした\n");
    return ;
  }
  printf("対局数書き込み\n");
  fprintf(fp,"%d \n",n);
  //fprintf(fp,"対局数 %d \n",n);
  //n回対局
  for(taikyoku = 0;taikyoku<n;taikyoku++)
  {
    //usleep(1);
    printf("初期化\n");
    init(&board);
    count=0;
    //fprintf(fp,"第%d局目",taikyoku+1);
    printf("第%d局",taikyoku+1);
    for ( i = 0; i < 64; i++)
    {
      kif[i]=0;
    }
    while(bitCount(~(board.black|board.white))>=SOLVE_DEPTH&&checkGameover(&board)!=GAME_OVER&&count<64)
    {      
      //最終完全よみ（仮）
      /*
      if(bitCount(~(board.black|board.white))<=(SOLVE_DEPTH))
      {
        resu=(board.teban)*solver(board);
        fprintf(fp,"%d ",resu);
        //fprintf(fp,"石差 %d ",resu);
        printf("石差%d\n",resu);
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
        printf("sente\n");
        fflush(stdout);
        legal=canReverse(&board);
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
          put(pos,&board);
          //kif[count] = getRMB(pos);//kif保存
          kif[count] = getPosnum(pos);//kif保存
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
          count++;
          continue;
        }
        else
        {
          //pos = randPos(legal);
          if(count<10)pos = randPos(legal);
          else pos = evalPos(legal,&board);
          if (!pos) return;
          put(pos,&board);
          //kif[count] = getRMB(pos);//kif保存
          kif[count] = getPosnum(pos);//kif保存
        }
        show(&board);
        board.teban=SENTE;
      }
    
      count++;
      printf("%d手\n",count);
    }
    resu=(board.teban)*solver(board);
    //resu = bitCount( board.black) - bitCount(board.white);
    fprintf(fp,"%d ",resu);
    //fprintf(fp,"石差 %d ",resu);
    printf("石差%d\n",resu);
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
int getPosnum(uint64_t pos)
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
int **makeKifArray(int n)
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
void learning()
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

  printf("評価関数初期化\n");
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

  printf("ファイルを開いた\n");
  fflush(stdout);
  fscanf(fp,"%d",&taikyoku);
  printf("対局数を読み取る\n");
  fflush(stdout);
  kif = makeKifArray(taikyoku);
  printf("kifポインタ\n");
  fflush(stdout);
  sekisa = (int *)malloc((sizeof(int)*taikyoku));
  printf("このあと入力\n");
  fflush(stdout);
  for(i=0;i<taikyoku;i++)
  {
    fscanf(fp,"%d",&sekisa[i]);
    for(j=0;j<64;j++)
    {
      fscanf(fp,"%d",&kif[i][j]);
    }
  }

  printf("入力終了\n");
  fflush(stdout);
  init(&board);

  printf("board初期化\n");
  fflush(stdout);
//segmentaion fault
  for ( i = 0; i < taikyoku; i++)
  {
    printf("第%d局\n",i);
    init(&board);
    for (j = 0; j < 64; j++)
    {
      if(j%2==0)
      {//SENTE
        //put(kif[i][j],&board);
        if(kif[i][j]!=0) put((uint64_t)1<<(kif[i][j]-1),&board);//uint64_tじゃない
        printf("DEBUG BEFORE UPDATE\n");
        updateeval((int)(rate*((double)(-sumeval(&board)+1000*sekisa[i]))),&board);

        printf("SEKISA = %d : SUMEVAL =  %d \n",sekisa[i],sumeval(&board));
        board.teban=GOTE;
        showBitboard(&board);
        fflush(stdout);
      }
      else
      {//GOTE
        //put(kif[i][j],&board);
        if(kif[i][j]!=0) put((uint64_t)1<<(kif[i][j]-1),&board);//uint64_tじゃない
        printf("DEBUG BEFORE UPDATE\n");
        //updateeval((int)((-1)*rate*((double)(-sumeval(&board)+1000*sekisa[i]))),&board);
        updateeval((int)(rate*((double)(-sumeval(&board)+1000*sekisa[i]))),&board);
        board.teban=SENTE;
        printf("SEKISA = %d : SUMEVAL =  %d\n",sekisa[i],sumeval(&board));
        showBitboard(&board);
        fflush(stdout);
      }
      
    }
    
  }
  

  printf("解析終了\n");
  fflush(stdout);
  free(sekisa);
  printf("free 石差終わり\n");
  fflush(stdout);
  for ( i = 0; i < taikyoku; i++)
  {
    free(kif[i]);
  }
  printf("free終わり\n");
  fflush(stdout);
  free(kif);
  printf("書き込み開始終わり\n");
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


void updateeval(int point,BitBoard *board)
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
