#include<stdint.h>
#include "../engine.h"

#ifndef _RANDAI_
#define _RANDAI_
void randAi();//rand着手のAI
uint64_t randPos(uint64_t legalboard);//合法手から一手選ぶ
class RandAIEngine : public Engine{
  public:
  private:
    uint64_t randPos(uint64_t legalboard);
};
#endif
