#include<stdint.h>
#include"../eval.h"
#include"../othero.h"
#include"../learn.h"
#include"../engine.h"

#ifndef _ONE_
#define _ONE_
//一手読み
//te 先手，後手
class One : public Engine{
    public:
        void one(int te);
    private:
        Eval eval;
        uint64_t bestPos(BitBoard board);
        uint64_t go() override;
        string woi() override;
};
//置く処理
//reverseと同様の処理をしている場所を書き直す
BitBoard vput(uint64_t pos,BitBoard *board);
int max(int array[],int n);
int min(int array[],int n);
#endif
