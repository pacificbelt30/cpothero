#include <unistd.h>
#include "./engine/one.h"
#include "./engine/rand_ai.h"
#include "./engine/static.h"
#include "./othero.h"
#ifndef __ENV__
#define __ENV__
//const uint64_t INITBLACK = 0x1008000000;
//const uint64_t INITWHITE = 0x810000000;
//const uint64_t INITBLACK = 69123178496; 
//const uint64_t INITWHITE = 34359738368;
//const uint64_t INITBLACK = 68988437630;
//const uint64_t INITWHITE = 34495019520;
const uint64_t INITBLACK = 69123964928;
const uint64_t INITWHITE = 34359738368;
const TEBAN INITTEBAN = GOTE;
//const bool DEBUG_MODE = false;
const bool DEBUG_MODE = true;
//using ENGINE = RandAIEngine;
using ENGINE = One;
//using ENGINE = Static;
#endif
