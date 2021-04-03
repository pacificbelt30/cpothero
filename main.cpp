#include<iostream>
#include<stdio.h>
#include<stdint.h>
#include<stdlib.h>
#include<time.h>
#include <unistd.h>
//#include"./eval.h"
#include"./othero.h"
//#include"./learn.h"
#include"./engine/rand_ai.h"
#include"./engine/one.h"
//#include"./solve.h"
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


int main(){
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
  /*
  generateKif(100);//引数に作成する対局数
  learning();//学習用
  */
  //RandAIEngine *e = new RandAIEngine();
  One *e = new One();
  e->recieve();
  return 0;
}
