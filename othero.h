#include<stdint.h>

#ifndef _OTHERO_
#define _OTHERO_

#define INPUT_ERROR 3
#define PASS 1
#define GAME_OVER 2
#define MASK_HORIZONTAL 0x7e7e7e7e7e7e7e7e
#define MASK_VERTICAL 0x00ffffffffffff00
#define MASK_DIAGONAL 0x007e7e7e7e7e7e00
//手番の列挙型
typedef enum _TEBAN{
	SENTE = -1,
	GAMEOVER = 0,
	GOTE = 1,
}TEBAN;

//みんな大好きBitBoard 黒番、白番の盤面状態と手番情報を保持
typedef struct _BitBoard{
	uint64_t black,white;
	TEBAN teban;
}BitBoard;

class Ohtero
{
  public:
uint64_t inputPos();
int bitCount(uint64_t board);//1の立っている数を数える,分割統治法
void init(BitBoard *board);//初期盤面

int checkGameover(BitBoard *board);//GAMEOVER判定 (全て同じ色で埋まっているか，全てのマスが埋まっている
uint64_t inputPos();//座標の入力
void show(BitBoard *board);//盤面表示
//ランダムAI用関
void result(BitBoard *board);//対局結果を出力,関数名を変えたい
uint64_t reverse(uint64_t pos, BitBoard *board);//裏返し処理
uint64_t canReverse(BitBoard *board);//置ける場所を返す
uint64_t put(uint64_t pos,BitBoard *board);//posの座標に石を置く,posは合法手である必要がある

void inverseTEBAN(BitBoard *board);//手番変え

int checkLeagal(BitBoard board);
}
#endif
