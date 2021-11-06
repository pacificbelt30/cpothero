#include<iostream>
#include<stdio.h>
#include<stdint.h>
#include<stdlib.h>
#include<time.h>
#include <unistd.h>
#include "./env.h"
//#include"./eval.h"
#include"./othero.h"
//#include"./learn.h"
#include"./engine/rand_ai.h"
#include"./engine/one.h"
#include"./engine/static.h"
#include"./solve.h"
#include"./test/aitest.h"
using namespace std;

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


int main(int argc,char *argv[]){
  /*
  int te;
  cout << "先後を選択(先手は-1，後手は1)" << endl;
  fflush(stdout);
  scanf("%d",&te);
  //randAi(te);
  one(te);
  */
  /*
  Othero othero;
  BitBoard board;
  othero.init(&board);
  */
  /*
  board.black=BLACK;
  board.white=WHITE;
  board.teban=GOTE;
  board.teban=SENTE;
  */
  /*
  cout << othero.checkLeagal(board) << endl;
  cout << othero.canReverse(&board) << endl;
  cout << "初手合法手" << othero.bitCount(othero.canReverse(&board)) << endl;
  othero.inverseTEBAN(&board);
  cout << "TEBAN" << board.teban << endl;
  othero.inverseTEBAN(&board);
  cout << "TEBAN" << board.teban << endl;
  */
  //cout << "結果" << solver(board) << endl;
  //cout << "結果" << solverMM(board) << endl;
  //cout << "結果" << (int)(BLACK&WHITE) << endl;
  //openeval();
  srand((unsigned int)time(NULL)); // 現在時刻の情報で初期化
  
  if(argc != 1){
    if(argv[1][0] == 'l'){
      Learn l;
      l.generateKif(KYOKUSU);//引数に作成する対局数
      l.learning();//学習用
    }
    if(argv[1][0] == 's'){
      BitBoard board;
      Static solverEngine;
      board.black = DEBUG_SOLVE_BLACK;
      board.white = DEBUG_SOLVE_WHITE;
      board.teban = DEBUG_SOLVE_TEBAN;
      cout << board.black << " " << board.white << " " << board.teban << endl;
      cout << "solver 結果" << Solve::solver(board) << endl;
      cout << "solver 結果" << Solve::solver(board) << endl;
      int temp = Solve::solver(board);
      cout << "solver 結果" << temp << endl;
      cout << "solverMM 結果" << Solve::solverMM(board) << endl;
      /*??????*/
      cout << "solverEngine 結果" << solverEngine.solver_nega(board).eval << endl;
      cout << "solverEngine 結果" << solverEngine.solver_nega(board).eval << endl;
      BInfo binfo = solverEngine.solver_nega(board);
      int eval = solverEngine.solver_nega(board).eval;
      //binfo.eval = solverEngine.solver_nega(board).eval;
      cout << "solverEngine 結果" << binfo.eval << endl;
      cout << "solverEngine 結果" << binfo.pos  << endl;
      cout << "solverEngine 結果" << eval  << endl;
      cout << "solverEngine 結果" << solverEngine.solver_nega(board).eval << endl;
      cout << board.black << " " << board.white << " " << board.teban << endl;
      //cout << "空きます" << (int)(BLACK&WHITE) << endl;
    }
  }
  else{
    // ENGINE e = new ENGINE();
    if(DEBUG_MODE){
      AITest t;
      t.staticAITest();
      t.recieve();
    }
    else{
      ENGINE e;
      e.recieve();
    }
  }
  return 0;
}
