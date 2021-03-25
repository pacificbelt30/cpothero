#include <iostream>
#include <string>
#include "engine.h"
using namespace std;

string Engine::recieve(){
  string command;
  cin >> command;
  if(command=="woi"){
    cout << woi() << endl;
  }
  else if(command=="woinewgame"){
    cout << woiNewGame() << endl;
  }
  else if(command=="isready"){
    cout << isReady() << endl;
  }
  else if(command=="go"){
    cout << go() << endl;
  }
  else if(command=="position"){
    cout << position() << endl;
  }
  else if(command=="quit"){
    cout << quit() << endl;
    exit(0);
  }
  else if(command=="stop"){
    cout << stop() << endl;
  }
  else if(command=="gameover"){
    cout << gameover() << endl;
  }
  else{
    cout << "False" << endl;
  }
  return "";
}

bool Engine::woi(){
  return true;
}

bool Engine::woiNewGame(){
  return true;
}

bool Engine::position(){
  uint64_t black;
  uint64_t white;
  int teban;
  cin >> black >> white >> teban;
  board.black = black;
  board.white = white;
  board.teban = TEBAN(teban);
  return true;
}

uint64_t Engine::go(){
  return true;
}

bool Engine::isReady(){
  return true;
}

bool Engine::quit(){
  return true;
}

bool Engine::gameover(){
  return true;
}

bool Engine::stop(){
  return true;
}

BitBoard Engine::getboard(){
  return this->board;
}
