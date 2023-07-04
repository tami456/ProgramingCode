#include <DxLib.>

#define SCREEN_SIZE_X 640
#define SCREEN_SIZE_Y 480

#define PLAYER_SIZE_X 32
#define PLAYER_SIZE_Y 32

#define ENEMY_SIZE_X 64
#define ENEMY_SIZE_Y 64

// プレイヤー関連
int player Image;	//グラフィックハンドル
int player PosX;	//X座標
int player PosY;	//Y座標
int playerSpeed;	//移動量(スピード）

//エネミー関連
int enemyImage	//グラフィックハンドル
int enemyPosX;	//X座標
int enemyPosY;	//Y座標
int enemySpeed;	//移動量(スピード)

int WINAPI WinMain (HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	//初期化処理
SetGraphMode(SCREEN_SIZE_X, SCREEN_SIZE_Y, 32);
ChangeWindowMode(true);
if DxLib_Init() == -1)
{
	return -1;
}

//画像の読み込み
playerImage = LoadGraph("image/player.png");

enemyImage = LoadGraph("image/enemy.png");

//変数の初期化
playerPosX = (SCREEN_SIZE_X - PLAYER_SIZE_X) / 2;
playerPosY = SCREEN_SIZE_Y - PLAYER_SIZE_Y;
playerSpeed = 1;

while (ProcessMessage() == 0 && CheckHitKey(KEY_INPUT_ESCAPE) == 0)
{
	// playerPosX = playerPosX + 1;
	// playerPosX =+ 1;
	// playerPosX++; //同じ意味
	if (CheckHitkey(KEY_INPUT_NUMPAD4))
	{
		playerPosX -= playerSpeed;
	}
	if (CheckHitKey(KEY_INPUT_NUMPAD6))
	{
		playerPosX -= playerSpeed;
	}
	if (CheckHitKey(KEY_INPUT_NUMPAD2))
	{
		playerPosY += playerSpeed;
	}








	SetDrawScreen(DX_SCREEN_BACK);
	ClsDrawScreen();
	DrawGraph(playerPosX,  playerPosY, playerImage, true);
	DrawGraph(enemyPosX, enemyPosY, enemyImage, true);
	ScreenFlip();
}

DxLib_End();
return 0
}