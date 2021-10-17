#include<stdint.h>
#include"../eval.h"
#include"../othero.h"
#include"../learn.h"
#include"../engine.h"

#ifndef _STATIC_
#define _STATIC_
#define ENGINEDEPTH 5
#define SOLVERDEPTH 7
#define INFINITY 1.0e9

// 指し手に対する評価値の構造体
typedef struct _BInfo {
  uint64_t pos; // 指し手
  int eval; // 評価値
  int yomikazu; // 読んだ手の数
}BInfo;

//一手読み
//te 先手，後手
class Static : public Engine{
  public:
    Static();
    ~Static();
    void one(int te);
  private:
  protected:
    //valiable
    double staticBoard[64];
    // function
    //int evalPos(BitBoard board,int depth);
    uint64_t evalPos(uint64_t legal,BitBoard board);
    void bitboardToArray(BitBoard board,int* array);
    void openEval();
    uint64_t bestPos(BitBoard board);
    uint64_t go() override;
    string woi() override;
    uint64_t search(BitBoard board,int depth);//depth手探索
    BInfo negaMax(BitBoard board,unsigned int depth);
    BInfo solver_nega(BitBoard board);
};
#endif
