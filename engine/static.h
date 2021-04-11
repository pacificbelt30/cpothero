#include<stdint.h>
#include"../eval.h"
#include"../othero.h"
#include"../learn.h"
#include"../engine.h"

#ifndef _STATIC_
#define _STATIC_
#define ENGINEDEPTH 5
//一手読み
//te 先手，後手
class Static : public Engine{
    public:
        void one(int te);
    private:
        Eval eval;
        uint64_t bestPos(BitBoard board);
        uint64_t go() override;
        string woi() override;
        uint64_t search(BitBoard board,int depth);
};
#endif
