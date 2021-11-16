#include<stdint.h>
#include"../eval.h"
#include"../othero.h"
#include"../learn.h"
#include"../engine.h"

#ifndef _STATIC_
#define _STATIC_
#define ENGINEDEPTH 4
#define SOLVERDEPTH 9
#define INFINITY 1.0e9

// 指し手に対する評価値の構造体
typedef struct _BInfo {
  int eval; // 評価値
  int yomikazu; // 読んだ手の数
  uint64_t pos; // 指し手
} BInfo;

//一手読み
//te 先手，後手
class Static : public Engine{
  public:
    Static();
    ~Static();
    void one(int te);
    uint64_t bestPos(BitBoard board);
    BInfo solver_nega(BitBoard board,bool flag);
  private:
  protected:
    //valiable
    double staticBoard[64];
    // function
    //int evalPos(BitBoard board,int depth);
    uint64_t evalPos(uint64_t legal,BitBoard board);
    void bitboardToArray(BitBoard board,int* array);
    void openEval();
    uint64_t go() override;
    string woi() override;
    uint64_t search(BitBoard board,int depth);//depth手探索
    BInfo negaMax(BitBoard board,unsigned int depth);
};
#endif
