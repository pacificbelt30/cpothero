#include <iostream>
#include <string>
#include "engine.h"
using namespace std;

string Engine::recieve(){
  while(true){
  string command;
  cin >> command;
  if(command=="woi"){
    cout << "id " << woi() << endl;
  }
  else if(command=="woinewgame"){
    cout << woiNewGame() << endl;
  }
  else if(command=="isready"){
    cout << isReady() << endl;
  }
  else if(command=="go"){
    cout << "bestmove " << go() << endl;
    // cout << go() << endl;
  }
  else if(command=="position"){
    if(position()){
      cout << "posok" << endl;
    }
    else{
      cout << "posfailed" << endl;
    }
  }
  else if(command=="quit"){
    cout << quit() << endl;
    exit(0);
  }
  else if(command=="stop"){
    if(stop()){
      cout << "OK" << endl;
      break;
    }
    else{
      cout << "NO" << endl;
    }
  }
  else if(command=="gameover"){
    cout << gameover() << endl;
  }
  else{
    cout << "DEBUG:"<< command << endl;
  }
  }
  return "";
}

string Engine::woi(){
  return "test";
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
  return 0;
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
