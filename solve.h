#include<stdint.h>
#include"./eval.h"
#include"./othero.h"
#include"./learn.h"

#ifndef _SOLVE_
#define _SOLVE_

//白番から
//白2石勝ち
//白H7→黒H8→白A1→黒B1→白A8→黒B7→白B8
//#define BLACK 0x20D58C9C90849E00
/*
00100000
11010101
10001100
10011100
10010000
10000100
10011110
00000000
*/
//#define WHITE 0x1F2A73636F7B203E
/*
00011111
00101010
01110011
01100011
01101111
01111011
00100000
00111110
*/
/*
#define BLACK 0x101312303010100
#define WHITE 0x9E7ECEDCFC1E0800
*/

//白スタート 白2
/*
#define BLACK 0x78F04E46BED0A040
#define WHITE 0x870F3139412F1D1E
*/

//黒スタート　黒6
/*
#define BLACK 0x7010100810120500
#define WHITE 0x8EECEFF76F6D3A3F
*/

//白スタート　白4
/*
#define BLACK 0x7F38344E66361C0C
#define WHITE 0x87CBB199C92313
*/

//黒スタート　黒4

#define BLACK 0x70485A227810FE
#define WHITE 0xFC8CB7A5DD87ED00

#define INFINITY 1.0e9

class Solve{
  public:
    Solve();
    static int solver(BitBoard board);
    static int solverMM(BitBoard board);
    //void inverseTEBAN(BitBoard *board);
};
#endif
