#include<stdint.h>
#include<string>
#include "../engine.h"

using namespace std;
#ifndef _RANDAI_
#define _RANDAI_
void randAi();//rand着手のAI
uint64_t randPos(uint64_t legalboard);//合法手から一手選ぶ
class RandAIEngine : public Engine{
  public:
  private:
    uint64_t randPos(uint64_t legalboard);
    uint64_t go() override;
    string woi() override;
};
#endif
