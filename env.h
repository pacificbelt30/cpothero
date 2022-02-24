#include <unistd.h>
#include "./engine/one.h"
#include "./engine/rand_ai.h"
#include "./engine/static.h"
#include "./engine/nteyomi.h"
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
const bool DEBUG_MODE = false;
//const bool DEBUG_MODE = true;

/*
 * ソルバーテスト用
*/
/* 黒6 */
//const uint64_t DEBUG_SOLVE_BLACK = 0x7010100810120500;
//const uint64_t DEBUG_SOLVE_WHITE = 0x8EECEFF76F6D3A3F;
//const TEBAN DEBUG_SOLVE_TEBAN = SENTE;

/* 黒4 */
//const uint64_t DEBUG_SOLVE_BLACK = 0x70485A227810FE;
//const uint64_t DEBUG_SOLVE_WHITE = 0xFC8CB7A5DD87ED00;
//const TEBAN DEBUG_SOLVE_TEBAN = SENTE;

/* 白4 */
//const uint64_t DEBUG_SOLVE_BLACK = 0x7F38344E66361C0C;
//const uint64_t DEBUG_SOLVE_WHITE = 0x87CBB199C92313;
//const TEBAN DEBUG_SOLVE_TEBAN = GOTE;

/**/
//const uint64_t DEBUG_SOLVE_BLACK = 0x101312303010100;
//const uint64_t DEBUG_SOLVE_WHITE = 0x9E7ECEDCFC1E0800;
//const TEBAN DEBUG_SOLVE_TEBAN = SENTE;

/* 白2石 */
//const uint64_t DEBUG_SOLVE_BLACK = 0x20D58C9C90849E00;
//const uint64_t DEBUG_SOLVE_WHITE = 0x1F2A73636F7B203E;
//const TEBAN DEBUG_SOLVE_TEBAN = GOTE;

/* 白2石 */
//const uint64_t DEBUG_SOLVE_BLACK = 0x78F04E46BED0A040;
//const uint64_t DEBUG_SOLVE_WHITE = 0x870F3139412F1D1E;
//const TEBAN DEBUG_SOLVE_TEBAN = GOTE;

/* 黒2石 */
//const uint64_t DEBUG_SOLVE_BLACK = 0x7E60587A6E58207E;
//const uint64_t DEBUG_SOLVE_WHITE = 0x1FA78591A7DC80;
//const TEBAN DEBUG_SOLVE_TEBAN = SENTE;

/* 黒4石 */
const uint64_t DEBUG_SOLVE_BLACK = 0x3EB8B8A894BA0EFE;
const uint64_t DEBUG_SOLVE_WHITE = 0x4446576B45F101;
const TEBAN DEBUG_SOLVE_TEBAN = SENTE;

// エンジン選択
//using ENGINE = RandAIEngine;
//using ENGINE = One;
//using ENGINE = Static;
using ENGINE = Nte;
#endif
