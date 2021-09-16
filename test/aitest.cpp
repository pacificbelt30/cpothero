#include<iostream>
#include "../env.h"
#include "./aitest.h"
#include "../engine/static.h"
#include "../othero.h"

using namespace std;

void AITest::staticAITest(){
  //Static e;
  //e.recieve();
  //e.board.black = INITBLACK;
  //e.board.white = INITWHITE;
  //e.board.teban = -1;
  Othero::init(&board);
  cout << Othero::canReverse(&board)<<endl;;
  cout << this->go();
  cout << this->evalPos(Othero::canReverse(&board),board) << endl;
  //cout << << endl;
}
