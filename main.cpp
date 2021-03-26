#include<stdio.h>
#include<stdint.h>
#include<stdlib.h>
#include<time.h>
#include <unistd.h>
//#include"./eval.h"
#include"./othero.h"
//#include"./learn.h"
#include"./engine/rand_ai.h"
//#include"./engine/one.h"
//#include"./solve.h"

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
  printf("先後を選択(先手は-1，後手は1)\n");
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
  printf("%d\n",othero.checkLeagal(board));
  printf("%llu\n",othero.canReverse(&board));
  printf("初手合法手%llu\n",othero.bitCount(othero.canReverse(&board)));
  othero.inverseTEBAN(&board);
  printf("%d\n",board.teban);
  othero.inverseTEBAN(&board);
  printf("%d\n",board.teban);
  */
  //printf("結果%d\n",solver(board));  
  //printf("結果%d\n",solverMM(board));  
  //printf("結果%d\n",(int)(BLACK&WHITE));
  /*
  openeval();
  srand((unsigned int)time(NULL)); // 現在時刻の情報で初期化
  generateKif(100);//引数に作成する対局数
  learning();//学習用
  */
  RandAIEngine *e = new RandAIEngine();
  e->recieve();
  return 0;
}
