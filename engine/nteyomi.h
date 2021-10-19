#include<stdint.h>
#include"../eval.h"
#include"../othero.h"
#include"../learn.h"
#include"../engine.h"

#define NTE_DEPTH 5
#define NTE_SOLVE_DEPTH 9

#ifndef _NTE_
#define _NTE_
//一手読み
//te 先手，後手
class Nte : public Engine{
    public:
        //void one(int te);
        uint64_t bestPos(BitBoard board);
    private:
        Eval eval;
        uint64_t go() override;
        string woi() override;
};

#endif
