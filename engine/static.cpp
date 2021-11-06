#include<iostream>
#include<stdio.h>
#include<stdint.h>
#include<stdlib.h>
#include<time.h>
#include <unistd.h>
#include"../eval.h"
#include"../othero.h"
#include"../learn.h"
#include"./one.h"
#include"./static.h"
#include"../env.h"
#include<fstream>
using namespace std;

//一手読み
//te 先手，後手
void Static::one(int te)
{
  //手順：　先手で
  //指す->入力される->合法手判断->適用(再入力)->繰り返し
  int i,count=1;
  int gyo;
  char retu;
  uint64_t pos,legal;
  BitBoard board;
  //Eval eval;
  Othero::init(&board);
  Othero::show(&board);
  cout << "試合開始" << endl;
  fflush(stdout);
  //eval.openeval();
  //対戦部分(人間先手1，後手-1)
  if(te==-1)
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
          pos = this->evalPos(legal,board);
          //cout << "evalPos=" << pos << endl;
          if (!pos) return;
          Othero::put(pos,&board);
        }
        Othero::show(&board);
        board.teban=GOTE;
      }
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
          pos = this->evalPos(legal,board);
          //cout << "evalPos=" << pos << endl;
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
          pos = this->evalPos(legal,board);
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
          pos = this->evalPos(legal,board);
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

string Static::woi(){
  return "静的評価関数";
  //return to_string(eval.dir4[0]);
}

uint64_t Static::go(){
  BitBoard nowBoard = this->getboard();//現在の局面
  return this->bestPos(nowBoard);
}

//現在の局面を評価し最善手を返す
uint64_t Static::bestPos(BitBoard board){
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
  //best = this->solver_nega(tmp);
  //if(DEBUG_MODE) cout <<"bestpos "<< tmp2 << endl;
  return best.pos;
  // return true;
}

//探索 最善手を返す
uint64_t Static::search(BitBoard board,int depth){
  int best = (-1)*INFINITY;
  uint64_t bestPos;
  int val = 0;
  BitBoard tmp = board;
  uint64_t legal = Othero::canReverse(&board);
  int legalnum = Othero::bitCount(legal);
  int count = 0;
  for(int i=0; i<64; i++)
  {
    if(legalnum<=count) break;
    else if((legal&((uint64_t)(1)<<i))!=0)
    {
      //val = eval(Othero::vput((uint64_t)(1) << i,&board),ENGINEDEPTH);
      if(best<val) {best = val; bestPos = (uint64_t)(1) << i;}
      count++;
    }
  }
  return bestPos;
}

//評価値
uint64_t Static::evalPos(uint64_t legal,BitBoard board){
  int best = (-1)*INFINITY;
  int tmpArray[64];
  BitBoard tmp = board;
  //uint64_t legal = Othero::canReverse(&board);
  int legalnum = Othero::bitCount(legal);
  int count = 0;
  int val;
  int index=0;
  //if(depth == 0){
  //}
  if(Othero::checkGameover(&board) == GAME_OVER){
  }
  if(legalnum == 0){
    //Othero::inverseTEBAN(&tmp);
    //this->evalPos(-1,tmp,depth-1);
    //Othero::inverseTEBAN(&tmp);
    //if(best<val) best = val;
    //return best;
    return -1;
  }

  for(int i=0; i<64; i++){
    val = 0;
    if(legalnum<=count) break;
    else if((legal&((uint64_t)(1)<<i))!=0){
      count++;
      //BitBoard temp = board;
      tmp = Othero::vput((uint64_t)(1)<<i,&board);
      tmp.teban = board.teban;
      Othero::inverseTEBAN(&tmp);
      this->bitboardToArray(tmp,tmpArray);
      //val = this->eval();
      for (int j = 0; j < 64; j++) {
        val += tmpArray[j] * this->staticBoard[j];
      }
      if(DEBUG_MODE) cout<< "now val:" << val <<" now best:" << best << endl;
      Othero::inverseTEBAN(&tmp);//手番をもとに戻す この時点で入力の手番と同じになる
      if(tmp.teban==GOTE)val *= -1;//後手番の場合評価値逆転
      if(DEBUG_MODE)cout << "現在の評価値" << val << " value:"<<((uint64_t)(1)<<i)<< endl;
      if(best<val) {best = val;index=i;if(DEBUG_MODE)cout<<index<<endl;}
    }
  }
  if(DEBUG_MODE)cout << "index :" << index << " value:" << ((uint64_t)(1)<<index) <<endl;
  cerr << "index :" << index << " value:" << ((uint64_t)(1)<<index) <<endl;
  return (uint64_t)(1)<<index;
}

// ビットボードの情報を配列に
void Static::bitboardToArray(BitBoard board,int* array){
  for (int i = 0; i < 64; i++) {
    if((board.black&(uint64_t(1)<<i))!=0){
      array[63-i] = 1;
    }
    else if((board.white&(uint64_t(1)<<i))!=0){
      array[63-i] = -1;
    }
    else{
      array[63-i] = 0;
    }
  }
  if(DEBUG_MODE){
    for (int i = 0; i < 8; i++) {
      for (int j = 0; j < 8; j++) {
        cout << setw(3) <<array[i*8+j];
      }
      cout << endl;
    }
    cout << "end" << endl;
  }
  return ;
}

// 評価用ファイルを開く
void Static::openEval(){
  //std::ifstream ifs("~/work/cpothero/staticEvalBoard2.txt");
  std::ifstream ifs("/home/user/work/cpothero/staticEvalBoard2.txt");
  //std::ifstream ifs("/home/user/work/cpothero/staticEvalBoard1.txt");
  if(!ifs){
    if(DEBUG_MODE)std::cout << "ファイルが開けませんでした．" << endl;
    return;
  }
  if(DEBUG_MODE)std::cout << "ファイルを開けたよ" << endl;
  for (int i = 0; i < 64; i++) {
    ifs >> this->staticBoard[i];
  }
  for (int i = 0; i < 8; i++) {
    for (int j = 0; j < 8; j++) {
      if(DEBUG_MODE)std::cout << " " <<std::setw(4)<<this->staticBoard[i*8+j];
    }
    if(DEBUG_MODE)std::cout << std::endl;
    
  }
  return;
}

// 評価値と指し手の構造体を返す
// gameoverのときは石差に重みを付けてを返す
// passのときはpos=0にする
BInfo Static::negaMax(BitBoard board,unsigned int depth){
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
    this->bitboardToArray(board, tmpArray);
    for (int j = 0; j < 64; j++) {
      val += tmpArray[j] * this->staticBoard[j];
    }
    //Othero::inverseTEBAN(&temp);//手番をもとに戻す この時点で入力の手番と同じになる
    //if(board.teban==SENTE) {val *= -1;cerr << "SENTE" <<endl;}//後手番の場合評価値逆転
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
    if(board.teban == GOTE)//先手が最後に指した(-1倍されるのでwhite-black)
    {
      best.eval = 1000*(Othero::bitCount(board.white) - Othero::bitCount(board.black));
      best.yomikazu = legalnum;
      return best;
    }
    else//後手が最後に指した
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
      //temp = board;
      // 一手後の盤面を生成し，その盤面で再帰
      temp = Othero::vput(pos,&board);//この時点で手番が変わってはいる
      temp.teban = board.teban;
      b = temp.teban;
      //if(depth==1) cerr << "DEBUG_depth 1 teban:" << (int)(temp.teban) <<endl;
      Othero::inverseTEBAN(&temp);
      a = temp.teban;
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
BInfo Static::solver_nega(BitBoard board){
  int i,j;
  //int tmpArray[64];
  int val=0;
  int legalnum;
  int count=0;
  BInfo best;
  best.pos = 0ULL;
  best.eval = (-1)*INFINITY;
  best.yomikazu = 0;
  BitBoard temp;
  uint64_t legal = Othero::canReverse(&board);
  legalnum=Othero::bitCount(legal);

  /* 葉の場合、評価値を返す */
  if(Othero::checkGameover(&board)==GAME_OVER)
  {
    if(board.teban == GOTE)//先手が最後に指した(-1倍されるのでwhite-black)
    {
      best.eval = Othero::bitCount(board.white) - Othero::bitCount(board.black);
    }
    else//先手が最後に指した
    {
      best.eval = Othero::bitCount(board.black) - Othero::bitCount(board.white);
    }
    best.pos = 0ULL;
    best.yomikazu = 0;
    //cout << "static solver gameover" << endl;
    return best;
  } //return eval();

  //pass
  if(legalnum == 0)
  {
    //temp = board;
    //Othero::inverseTEBAN(&temp);
    TEBAN fat = board.teban;
    Othero::inverseTEBAN(&board);
    if(fat==board.teban) std::cout << "error" << std::endl;
    //put(1<<i,&temp);
    //BInfo binfo = this->solver_nega(temp);
    BInfo binfo = this->solver_nega(board);
    //binfo.eval = (-1)*binfo.eval;
    //if(best.eval<binfo.eval) best = binfo;
    //best = binfo;
    best.eval = (-1)*binfo.eval;
    best.pos = 0ULL;
    best.yomikazu = legalnum;
    return best;
  }
  
  //なんでもない時
  count = 0;
  for(i=0; i<64; i++)
  {
    if(legalnum<=count) break;
    else if((legal&((uint64_t)(1)<<i))!=0) // 指せるとき
    {
      uint64_t pos = (uint64_t)(1)<<i;
      count++;
      //BitBoard temp = board;
      // 一手後の盤面を生成し，その盤面で再帰
      //BitBoard temp = Othero::vput(pos,&board);
      temp = Othero::vput(pos,&board); /* これを使うとバグる? */ 
      //temp.teban = board.teban;

      //Othero::inverseTEBAN(&temp);
      BInfo binfo = this->solver_nega(Othero::vput(pos, &board));
      binfo.eval = (-1)*binfo.eval;
      binfo.pos = (uint64_t)pos;
      best.yomikazu += binfo.yomikazu;
      if(best.eval<binfo.eval) best = binfo;
    }
    //val = - Negamax(ai, 次の turn, depth-1);
    //if(best < val)   best= val;
  }

  return best;
}

Static::Static(){
  this->openEval();
}

Static::~Static(){

}
