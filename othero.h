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
typedef enum _TEBAN : int{
	SENTE = -1,
	GAMEOVER = 0,
	GOTE = 1,
}TEBAN;

//みんな大好きBitBoard 黒番、白番の盤面状態と手番情報を保持
typedef struct _BitBoard{
	uint64_t black,white;
	TEBAN teban;
}BitBoard;

class Othero
{
  public:
static int bitCount(uint64_t board);//1の立っている数を数える,分割統治法
static void init(BitBoard *board);//初期盤面

static int checkGameover(BitBoard *board);//GAMEOVER判定 (全て同じ色で埋まっているか，全てのマスが埋まっている
static uint64_t inputPos();//座標の入力
static void show(BitBoard *board);//盤面表示
//ランダムAI用関
static void result(BitBoard *board);//対局結果を出力,関数名を変えたい
static uint64_t reverse(uint64_t pos, BitBoard *board);//裏返し処理
static uint64_t canReverse(BitBoard *board);//置ける場所を返す
static uint64_t put(uint64_t pos,BitBoard *board);//posの座標に石を置く,posは合法手である必要がある
static BitBoard vput(uint64_t pos,BitBoard *board);//posの座標に石を置いたときの盤面を返す,posは合法手である必要がある

static void inverseTEBAN(BitBoard *board);//手番変え

static int checkLeagal(BitBoard board);
};
#endif
