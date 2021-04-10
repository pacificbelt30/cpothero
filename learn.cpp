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
#include "./othero.h"
#include "./learn.h"
#include "./solve.h"
#include "./eval.h"
#include "./engine/rand_ai.h"
#include "./engine/one.h"
using namespace std;


//追加
//NULLチェック済み
void Learn::writeeval(ofstream *fp,int n,int pat[])
{
  int i;
  for ( i = 0; i < n; i++)
  {
    *fp << pat[i] << " ";
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
  ofstream fp;
  string dirpath = getExecDirectoryName() + "eval/";

  //読み込むkifファイル
  fp.open((getExecDirectoryName()+"kif/100.txt").c_str());

  cout << "ファイルを開く" << endl;
  //createtable();//最右端ビット用テーブルの初期化
  if (!fp)
  {
    cout << "ファイルを開けませんでした" << endl;
    return ;
  }
  cout << "対局数書き込み" << endl;
  fp << n << " \n";
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
    fp << resu << " ";
    //fprintf(fp,"石差 %d ",resu);
    cout << "石差" << resu << endl;
    for(i=0;i<64;i++)
    {
      fp << kif[i] << " ";
    }
    fp << "\n";
  }
  fp.close();
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
  // FILE *fp;
  // FILE *d4,*d5,*d6,*d7,*d8,*h2,*h3,*h4,*cr,*eg;
  ifstream fp;
  ofstream d4,d5,d6,d7,d8,h2,h3,h4,cr,eg;
  string dirpath = getExecDirectoryName() + "eval/";

  //読み込むkifファイル
  fp.open((getExecDirectoryName()+"kif/100.txt").c_str());
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

  d4.open((dirpath+"d4.txt").c_str());
  if(!d4) return;
  d5.open((dirpath+"d5.txt").c_str());
  if(!d5) return;
  d6.open((dirpath+"d6.txt").c_str());
  if(!d6) return;
  d7.open((dirpath+"d7.txt").c_str());
  if(!d7) return;
  d8.open((dirpath+"d8.txt").c_str());
  if(!d8) return;
  h2.open((dirpath+"h2.txt").c_str());
  if(!h2) return;
  h3.open((dirpath+"h3.txt").c_str());
  if(!h3) return;
  h4.open((dirpath+"h4.txt").c_str());
  if(!h4) return;
  cr.open((dirpath+"cr.txt").c_str());
  if(!cr) return;
  eg.open((dirpath+"eg.txt").c_str());
  if(!eg) return;

  cout << "ファイルを開いた" << endl;
  fp >> taikyoku;
  cout << "対局数を読み取る" << endl;
  kif = makeKifArray(taikyoku);
  cout << "kifポインタ" << endl;
  sekisa = (int *)malloc((sizeof(int)*taikyoku));
  cout << "このあと入力" << endl;
  for(i=0;i<taikyoku;i++)
  {
    fp >> sekisa[i] ;
    for(j=0;j<64;j++)
    {
      fp >> kif[i][j];
    }
  }

  cout << "入力終了" << endl;
  Othero::init(&board);

  cout << "board初期化" << endl;
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
      }
      
    }
    
  }
  

  cout << "解析終了" << endl;
  free(sekisa);
  cout << "free 石差終わり" << endl;
  for ( i = 0; i < taikyoku; i++)
  {
    free(kif[i]);
  }
  cout << "free終わり" << endl;
  free(kif);
  cout << "書き込み開始終わり" << endl;
  
  writeeval(&h2,6561,hor2); 
  writeeval(&h3,6561,hor3); 
  writeeval(&h4,6561,hor4); 
  writeeval(&d4,81,dir4); 
  writeeval(&d5,243,dir5); 
  writeeval(&d6,729,dir6); 
  writeeval(&d7,2187,dir7); 
  writeeval(&d8,6561,dir8); 
  writeeval(&cr,6561,cor); 
  writeeval(&eg,6561,edg); 

  fp.close();
  h2.close();
  h3.close();
  h4.close();
  d4.close();
  d5.close();
  d6.close();
  d7.close();
  d8.close();
  cr.close();
  eg.close();
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
