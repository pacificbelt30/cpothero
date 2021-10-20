#include<iostream>
#include<stdio.h>
#include<stdint.h>
#include<stdlib.h>
#include<time.h>
#include <unistd.h>
#include"../eval.h"
#include"../othero.h"
#include"../learn.h"
#include"./nteyomi.h"
using namespace std;

string Nte::woi(){
  return "ntedakeyomu";
  //return to_string(eval.dir4[0]);
}

uint64_t Nte::go(){
  BitBoard tmp = this->getboard();
  return bestPos(tmp);
  // return true;
}

uint64_t Nte::bestPos(BitBoard board){
  BitBoard nowBoard = board;//現在の局面
  uint64_t tmp2;//
  //tmp2 = this->bestPos(tmp);
  BInfo best;//
  if(Othero::bitCount(~(nowBoard.black | nowBoard.white)) <= SOLVERDEPTH) {
    best=this->solver_nega(nowBoard);
    cerr <<"sovler: "<< best.eval<<" value:" << best.pos <<" yomikazu:"<<best.yomikazu<<endl;
  }
  else {
    best = this->negaMax(nowBoard, ENGINEDEPTH);
    cerr <<"eval: "<< best.eval<<" value:" << best.pos <<" yomikazu:"<<best.yomikazu<<endl;
  }
  return best.pos;
}

// 評価値と指し手の構造体を返す
// gameoverのときは石差に重みを付けてを返す
// passのときはpos=0にする
BInfo Nte::negaMax(BitBoard board,unsigned int depth){
  int i,j;
  int tmpArray[64];
  int val=0;
  int legalnum;
  int count=0;
  BInfo best;
  best.eval = (-1)*INFINITY;
  best.yomikazu = 0;
  BitBoard temp;
  uint64_t legal = Othero::canReverse(&board);
  legalnum=Othero::bitCount(legal);

  //DEBUG
  //cout << "TEBAN=" << board.teban << endl;
  //show(&board);

  // depth = 0
  /* 葉の場合、評価値を返す */
  if(depth<=0){
    //best.eval = ;
    //Othero::inverseTEBAN(&temp);//手番をもとに戻す この時点で入力の手番と同じになる
    //if(board.teban==SENTE) {val *= -1;cerr << "SENTE" <<endl;}//後手番の場合評価値逆転
    val = eval.sumeval(&board);
    //if(board.teban==SENTE)val *= -1;//後手番の場合評価値逆転 直後に-1倍される
    if(board.teban==GOTE)val *= -1;//後手番の場合評価値逆転 直後に-1倍される
    //cerr << "DEBUG_leaf depth:" << depth << " teban:" << (int)(board.teban) <<endl;
    best.eval = val;
    best.yomikazu = 0;
    return best;
  }

  /* gameoverのときは石差に重みを付けてを返す */
  if(Othero::checkGameover(&board)==GAME_OVER)
  {
    //if(board.teban == SENTE)//後手が最後に指した
    if(board.teban == GOTE)//後手が最後に指した
    {
      best.eval = 1000*(Othero::bitCount(board.white) - Othero::bitCount(board.black));
      best.yomikazu = legalnum;
      return best;
    }
    else//先手が最後に指した
    {
      best.eval = 1000*(Othero::bitCount(board.black) - Othero::bitCount(board.white));
      best.yomikazu = legalnum;
      return best;
    }
  } //return eval();

  /* 現在の局面から1手進めた状態をa1,a2,a3・・akとする */
  //expand_node(node, 次の turn);


  //pass
  if(legalnum == 0)
  {
    temp = board;
    Othero::inverseTEBAN(&temp);
    //put(1<<i,&temp);
    BInfo binfo = this->negaMax(temp, depth-1);
    binfo.eval = (-1)*binfo.eval;
    if(best.eval<binfo.eval) best = binfo;
    best.yomikazu += legalnum;
    return best;
  }
  
  //なんでもない時
  for(i=0; i<64; i++)
  {
    if(legalnum<=count) break;
    else if((legal&((uint64_t)(1)<<i))!=0) // 指せるとき
    {
      uint64_t pos = (uint64_t)(1)<<i;
      TEBAN b,a;
      count++;
      //BitBoard temp = board;
      temp = board;
      // 一手後の盤面を生成し，その盤面で再帰
      temp = Othero::vput(pos,&board);//この時点で手番が変わってはいる
      temp.teban = board.teban;
      b = temp.teban;
      //if(depth==1) cerr << "DEBUG_depth 1 teban:" << (int)(temp.teban) <<endl;
      Othero::inverseTEBAN(&temp);
      a = temp.teban;
      //cerr << "before teban:" << b << " after teban:" << a <<endl;
      if(b==a){cerr<<"ilegal teban"<<endl;}
      BInfo binfo = this->negaMax(temp, depth-1);
      binfo.eval = (-1)*binfo.eval;
      binfo.pos = pos;
      best.yomikazu += binfo.yomikazu; // ?
      if(best.eval<binfo.eval) best = binfo;
    }
    //val = - Negamax(ai, 次の turn, depth-1);
    //if(best < val)   best= val;
  }

  return best;
}

// 最終完全読み
BInfo Nte::solver_nega(BitBoard board){
  int i,j;
  int tmpArray[64];
  int val=0;
  int legalnum;
  int count=0;
  BInfo best;
  best.eval = (-1)*INFINITY;
  best.yomikazu = 0;
  BitBoard temp;
  uint64_t legal = Othero::canReverse(&board);
  legalnum=Othero::bitCount(legal);

  /* 葉の場合、評価値を返す */
  if(Othero::checkGameover(&board)==GAME_OVER)
  {
    best.pos = 0;
    if(board.teban == GOTE)//後手が最後に指した
    {
      best.eval = Othero::bitCount(board.white) - Othero::bitCount(board.black);
    }
    else//先手が最後に指した
    {
      best.eval = Othero::bitCount(board.black) - Othero::bitCount(board.white);
    }
    best.yomikazu = 0;
    return best;
  } //return eval();

  //pass
  if(legalnum == 0)
  {
    temp = board;
    Othero::inverseTEBAN(&temp);
    //put(1<<i,&temp);
    BInfo binfo = this->solver_nega(temp);
    binfo.eval = (-1)*binfo.eval;
    if(best.eval<binfo.eval) best = binfo;
    best.yomikazu = legalnum;
    return best;
  }
  
  //なんでもない時
  for(i=0; i<64; i++)
  {
    if(legalnum<=count) break;
    else if((legal&((uint64_t)(1)<<i))!=0) // 指せるとき
    {
      uint64_t pos = (uint64_t)(1)<<i;
      count++;
      //BitBoard temp = board;
      // 一手後の盤面を生成し，その盤面で再帰
      temp = Othero::vput(pos,&board);
      //temp.teban = board.teban;
      //Othero::inverseTEBAN(&temp);
      BInfo binfo = this->solver_nega(temp);
      binfo.eval = (-1)*binfo.eval;
      binfo.pos = pos;
      best.yomikazu += binfo.yomikazu;
      if(best.eval<binfo.eval) best = binfo;
    }
    //val = - Negamax(ai, 次の turn, depth-1);
    //if(best < val)   best= val;
  }

  return best;
}
