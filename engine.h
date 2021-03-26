#include <iostream>
#include <string>
#include "othero.h"
using namespace std;

// 基底クラス
class Engine{
  public:
    string recieve();
    bool run(); // エンジン実行コマンド
    BitBoard getboard();

  private:
    BitBoard board;

  protected:
    virtual bool isReady();
    virtual bool gameover();
    virtual uint64_t go();
    virtual bool position();
    virtual string woi();
    virtual bool woiNewGame();
    virtual bool stop();
    virtual bool quit();
};