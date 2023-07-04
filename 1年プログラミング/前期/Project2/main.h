#pragma once

//シーンID
enum SCN_ID {
	SCN_ID_TITLE,
	SCN_ID_GAME,
	SCN_ID_GAME_OVER,
	SCN_ID_MAX
};



//キャラの向き
enum DIR_ID {
	DIR_DOWN,
	DIR_LEFT,
	DIR_RIGHT,
	DIR_UP,
	DIR_MAX
};

//敵の左右の移動方向
enum MOVE_LR {
	MOVE_LEFT,		//左移動
	MOVE_RIGHT,		//右移動
	MOVE_MAX
};


enum MOVE_MODE{
	MOVE_MODE_LR,	//左右移動モード
	MOVE_MODE_DOWN,	//下移動モード
	MOVE_MODE_MAX	//
};



enum ENEMY_TYPE {
	ENEMY_TYPE_1,
	ENEMY_TYPE_2,
	ENEMY_TYPE_3,
	ENEMY_TYPE_4,
	ENEMY_TYPE_5,
	ENEMY_TYPE_6,
	ENEMY_TYPE_UFO,
	ENEMY_TYPE_MAX
};

//スコア
enum {
	SCR_PL1,		//プレイヤー
	SCR_HIGH,		//ハイスコア
	SCR_MAX	
};

#define TITLE_LOGO_IMAGE_SIZE_X 423		//タイトルのロゴサイズイメージX
#define TITLE_LOGO_IMAGE_SIZE_Y 198		//タイトルのロゴサイズイメージY

#define GAME_OVER_LOGO_IMAGE_SIZE_X 452	//ゲームオーバーのロゴサイズイメージX

#define ENEMY_NOMAL_MAX ENEMY_TYPE_UFO

#define SCREEN_SIZE_X 800	//画面の横サイズ
#define SCREEN_SIZE_Y 600	//画面の縦サイズ

#define GAME_SCREEN_X 40	//ゲームエリア画面の表示位置
#define GAME_SCREEN_Y 75	//ゲームエリア画面の表示位置

#define GAME_SCREEN_SIZE_X 640	//ゲームエリア画面の横サイズ
#define GAME_SCREEN_SIZE_Y 480	//ゲームエリア画面の縦サイズ


#define ANIM_MAX 3			//アニメーションパターン数

#define CHIP_SIZE_X 32		//キャラのサイズX
#define CHIP_SIZE_Y 32		//キャラのサイズY

#define EFFECT_SIZE_X 40	//爆発エフェクトサイズX
#define EFFECT_SIZE_Y 40	//爆発エフェクトサイズY



#define ENEMY_COUNT_X 8		//敵の横並びの数
#define ENEMY_COUNT_Y 7		//敵の縦並びの数
#define ENEMY_SPEED 8




#define PSHOT_SIZE_X 5			//自機の弾のサイズX
#define PSHOT_SIZE_Y 25			//自機の弾のサイズY
#define PSHOT_SPEED (-5)		//自機の弾の速度

#define ESHOT_SIZE_X 16			//敵の弾のサイズX
#define ESHOT_SIZE_Y 25			//敵の弾のサイズY
#define ESHOT_SPEED 4			//敵の弾の速度

#define ENEMY_SCR 10			//通常の敵を倒した時の点数
#define DEF_LINE_Y (GAME_SCREEN_SIZE_Y - CHIP_SIZE_Y * 2)	//防衛ラインのY座標

#define UFO_SCR 50

void TitleScene(void);
void TitleDraw(void);

void GameScene(void);
void GameDraw(void);

void GameOverScene(void);
void GameOverDraw(void);

void EnemyCtr(void);
void PlayerCtr(void);
void ShotCtr(void);
void CheckHitObj(void);
void PlayerDeathProc(bool backFlag);
void PlayerReProc(void);
void DrawTexts(void);
bool Init(void);
bool SysInit(void);
void GetKeyState(void);			//キー情報取得関数

