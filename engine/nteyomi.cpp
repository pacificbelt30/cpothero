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
  uint64_t legal = Othero::canReverse(&board);
  uint64_t pos = eval.evalPos(legal,&board);
  return pos;
}
