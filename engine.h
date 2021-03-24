#include <iostream>
#include <string>
#include "othero.h"
using namespace std;

// 基底クラス
class Engine{
  public:
    string recieve();
    bool run(); // エンジン実行コマンド

  private:
    BitBoard board;
    bool isReady();
    bool gameover();
    bool go();
    bool position();
    bool woi();
    bool woiNewGame();
    bool stop();
    bool quit();
};