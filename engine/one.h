#include<stdint.h>
#include"../eval.h"
#include"../othero.h"
#include"../learn.h"

#ifndef _ONE_
#define _ONE_
//一手読み
//te 先手，後手
class One{
    public:
Eval eval;
void one(int te);


};
//置く処理
//reverseと同様の処理をしている場所を書き直す
BitBoard vput(uint64_t pos,BitBoard *board);
int max(int array[],int n);
int min(int array[],int n);
#endif
