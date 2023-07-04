#include <stdlib.h>		
#include<time.h>		
#include <DxLib.h>
#include "main.h"

#include "_debug/_DebugConOut.h"
#include "_debug/_DebugDispOut.h"

// プレイヤー関連
int playerImage;	//グラフィックハンドル
int playerPosX;		//X座標
int playerPosY;		//Y座標
int playerSpeed;	//移動量(スピード）
bool playerFlag;	//状態フラグ(true:生存　false:死亡)

//エネミー関連
int enemyImage;		//グラフィックハンドル
int enemyPosX;		//X座標
int enemyPosY;		//Y座標
int enemySpeed;		//移動量(スピード)
bool enemyFlag;		//状態フラグ (true:生存 false:死亡)

int insekiImage;
int insekiPosX;
int insekiPosY;
int insekiSpeed;
bool insekiFlag;


//弾関連
int shotImage;		//グラフィックハンドル
int shotPosX;		//X座標
int shotPosY;		//Y座標
int shotSpeed;		//移動量(スピード)
bool shotFlag;		//状態フラグ true:発射中 false:非発射中

int bgImage;		//グラフィックハンドル
int bgPosY1;		//背景1用のY座標
int bgPosY2;		//背景２用のY座標
int bgSpeed;		//移動量

int kumoImage;
int kumoPosY1;
int kumoPosY2;
int kumoSpeed;

int score;

int zanki;

int WINAPI WinMain (HINSTANCE hInstance,
					HINSTANCE hPrevInstance,
					LPSTR lpCmdLine,
					int nCmdShow)
{
	//初期化処理
	SetWindowText("2116020_水田渚");

	SetGraphMode(SCREEN_SIZE_X, SCREEN_SIZE_Y, 32);
	ChangeWindowMode(true);
	if (DxLib_Init() == -1)
		{
		return -1;
		}

	LoadGraphScreen(0, 0, "image/start.png", TRUE);
	WaitKey();
	
	srand((unsigned int)time(NULL));

	_dbgSetup(SCREEN_SIZE_X, SCREEN_SIZE_Y, 255);
	TRACE("DXLIBの初期化成功\n");
	//画像の読み込み
	playerImage = LoadGraph("image/player.png");
	TRACE("playerImage = %d\n",playerImage);
	
	enemyImage = LoadGraph("image/enemy.png");
	TRACE("enemyImage = %d\n", enemyImage);

	shotImage = LoadGraph("image/shot.png");
	TRACE("	shotImage = %d\n", shotImage);

	bgImage = LoadGraph("image/haikei.png");
	TRACE("	bgImage = %d\n", bgImage);

	kumoImage = LoadGraph("image/kumo.png");
	TRACE("	kumoImage = %d\n", kumoImage);

	insekiImage = LoadGraph("image/inseki.png");
	TRACE("	insekiImage = %d\n", insekiImage);

	//変数の初期化
	//プレイヤー
	playerPosX = (SCREEN_SIZE_X - PLAYER_SIZE_X) / 2;
	playerPosY = SCREEN_SIZE_Y - PLAYER_SIZE_Y;
	playerSpeed = 2;
	playerFlag = true;

	//敵
	enemyPosX = (SCREEN_SIZE_X - ENEMY_SIZE_X) / 2;
	enemyPosY = (SCREEN_SIZE_Y - ENEMY_SIZE_Y) / 2;
	enemySpeed = 2;
	enemyFlag = false;

	//隕石
	insekiPosX = (SCREEN_SIZE_X - INSEKI_SIZE_X) / 2;
	insekiPosY = (SCREEN_SIZE_Y - INSEKI_SIZE_Y) / 2;
	insekiSpeed = 1;
	insekiFlag = false;

	//弾
	shotPosX = 0;
	shotPosY = SCREEN_SIZE_Y;
	shotSpeed = -4;
	shotFlag = false;

	//背景
	bgPosY1 = 0;
	bgPosY2 = -BG_SIZE_Y;
	bgSpeed = 2;

	kumoPosY1 = 0;
	kumoPosY2 = -KUMO_SIZE_Y;
	kumoSpeed = 4;

	score = 0;

	zanki = 3;


	TRACE("ゲームループ開始\n");

	while (ProcessMessage() == 0 && CheckHitKey(KEY_INPUT_ESCAPE) == 0)
	{
		_dbgStartDraw();
		// playerPosX = playerPosX + 1;
		// playerPosX =+ 1;
		// playerPosX++; //同じ意味
		

		//背景の移動処理
		bgPosY1 += bgSpeed;
		bgPosY2 += bgSpeed;
		if (bgPosY1 >= SCREEN_SIZE_Y)
		{
			bgPosY1 = bgPosY2 - BG_SIZE_Y;
		}
		if (bgPosY2 >= SCREEN_SIZE_Y)
		{
			bgPosY2 = bgPosY1 - BG_SIZE_Y;
		}

		kumoPosY1 += kumoSpeed;
		kumoPosY2 += kumoSpeed;
		if (kumoPosY1 >= SCREEN_SIZE_Y)
		{
			kumoPosY1 = kumoPosY2 - KUMO_SIZE_Y;
		}
		if (kumoPosY2 >= SCREEN_SIZE_Y)
		{
			kumoPosY2 = kumoPosY1 - KUMO_SIZE_Y;
		}

		//_dbgDrawBox(0, bgPosY1, 640, bgPosY1 + BG_SIZE_Y, GetColor(255, 0, 0), false);
		//_dbgDrawBox(0, bgPosY2, 640, bgPosY2 + BG_SIZE_Y, GetColor(255, 0, 0), false);
		
		//リスポーン処理
		if (!playerFlag)
		{
			TRACE("プレイヤーのリスポーン処理\n");
			playerPosX = (SCREEN_SIZE_X - PLAYER_SIZE_X) / 2;
			playerPosY = SCREEN_SIZE_Y - PLAYER_SIZE_Y;
			playerFlag = true;
		}

		if (!enemyFlag)
		{
			TRACE("敵のリスポーン処理\n");
			//rand()は0～RAND_MAX(32767)の範囲の数値を取得できる
			//今回求めたいのは0～(SCREEN_SIZE_X - ENEMY_SIZE_X)
			//乱数の起点の事をランダムシードと言う
			enemyPosX = rand() % (SCREEN_SIZE_X - ENEMY_SIZE_X);
			enemyPosY = -ENEMY_SIZE_Y;
			enemyFlag = true;
			
		}

		if (!insekiFlag)
		{
			TRACE("隕石のリスポーン処理\n");
			insekiPosX = rand() % (SCREEN_SIZE_X - INSEKI_SIZE_X);
			insekiPosY = -INSEKI_SIZE_Y;
			insekiFlag = true;
		}
		
		//プレイヤーの移動処理
		if (playerFlag)
		{
			//スクリーンキーボードでしかテンキー出せませんでした
			if (CheckHitKey(KEY_INPUT_NUMPAD4) || CheckHitKey(KEY_INPUT_LEFT))
				if (playerPosX > 0)
				{
					playerPosX -= playerSpeed;
				}
			if (CheckHitKey(KEY_INPUT_NUMPAD6) || CheckHitKey(KEY_INPUT_RIGHT))
				if (playerPosX < SCREEN_SIZE_X - PLAYER_SIZE_X)
				{
					playerPosX += playerSpeed;
				}
			if (CheckHitKey(KEY_INPUT_NUMPAD8) || CheckHitKey(KEY_INPUT_UP))
				if (playerPosY > 0)
				{
					playerPosY -= playerSpeed;
				}
			if (CheckHitKey(KEY_INPUT_NUMPAD2) || CheckHitKey(KEY_INPUT_DOWN))
				if (playerPosY < SCREEN_SIZE_Y - PLAYER_SIZE_Y)
				{
					playerPosY += playerSpeed;
				}
		}

		_dbgDrawBox(playerPosX + 31, playerPosY + 31, playerPosX, playerPosY, 0xfff, false);
		
		if (shotFlag)
		{
			shotPosY += shotSpeed;
			if (shotPosY < 0)
			{
				shotFlag = false;
			}
		}
		
		
			

		

		//Zキーを押したら、処理する
		if (playerFlag)
		{
			if (CheckHitKey(KEY_INPUT_Z)) //CheckHitKeyを先に入力すると処理内容が少ない
			{
				if (!shotFlag)
				{
					//PlaySoundFile("GB-mp3/GB-Shooter01-01(Shoot).mp3", DX_PLAYTYPE_NORMAL);
					//自機の座標を弾の座標に代入する
					shotPosX = playerPosX;
					shotPosY = playerPosY;
					//状態を発射中にする
					shotFlag = true;

					
				}
			}
		}


		

		//エネミーの移動処理
		
		if (enemyFlag)
		{
			enemyPosY += enemySpeed;
			if (enemyPosY >= SCREEN_SIZE_Y)
			{
				enemyFlag = false;
			}
		}
		_dbgDrawBox(enemyPosX + 63, enemyPosY + 63, enemyPosX, enemyPosY, 0xffff, false);		
		//_dbgDrawFormatString(0, 0, 0xffffff, "敵座標:%d,%d", enemyPosX, enemyPosY);
		//_dbgDrawFormatString(0, 16, 0xffffff, "PL座標:%d,%d", playerPosX, playerPosY);
		
		//隕石の移動処理
		if (insekiFlag)
		{
			insekiPosY += insekiSpeed;
			if (insekiPosY >= SCREEN_SIZE_Y)
			{
				insekiFlag = false;
			}
		}
		_dbgDrawBox(insekiPosX + 127, insekiPosY + 63, insekiPosX, insekiPosY, 0xff00ff, false);
		//当たり判定
		//自機と敵
		if (playerFlag && enemyFlag)
		{
			if (playerPosX  < enemyPosX + ENEMY_SIZE_X - 20 && //自機の左 < 敵の右
				enemyPosX  < playerPosX + PLAYER_SIZE_X -20 && //敵の左 < 自機の右
				playerPosY < enemyPosY + ENEMY_SIZE_Y -20 && //自機の上 < 敵の下
				enemyPosY < playerPosY + PLAYER_SIZE_Y -20   //敵の上 < 自機の下
				)
			{
				//　リスポーン処理
				playerFlag = false;
				enemyFlag = false;
				
				
				if (zanki >= 0)
				{
					zanki -= 1;
					if(zanki == -1)
					{
						WaitKey();
						zanki += 4;
						score = 0;
					}
				}
				//TRACE("当たった\n");
				//_dbgDrawFormatString(0, 48, 0xffffff,"当たった\n");
			}
		}
		//自機と隕石
		if (playerFlag && insekiFlag)
		{
			if (playerPosX < insekiPosX + INSEKI_SIZE_X&& //自機の左 < 隕石の右
				insekiPosX < playerPosX + PLAYER_SIZE_X  && //隕石の左 < 自機の右
				playerPosY < insekiPosY + INSEKI_SIZE_Y -5 && //自機の上 < 敵の下
				insekiPosY < playerPosY + PLAYER_SIZE_Y -5     //隕石の上 < 自機の下
				)
			{
				//　リスポーン処理
				playerFlag = false;
				insekiFlag = false;

				if (zanki >= 0)
				{
					zanki -= 1;
					if (zanki == -1)
					{
						WaitKey();
						zanki += 4;
						score = 0;
					}
				}
				//TRACE("当たった\n");
				//_dbgDrawFormatString(0, 48, 0xffffff,"当たった\n");
			}
		}
		
		if (shotFlag && enemyFlag)
		{
			//自機の弾と敵の当たり判定
			if (shotPosX < enemyPosX + ENEMY_SIZE_X -20 &&
				enemyPosX < shotPosX + SHOT_SIZE_X -10&&
				shotPosY < enemyPosY + ENEMY_SIZE_Y -20 &&
				enemyPosY < shotPosY + SHOT_SIZE_Y -10
				)
			{
				enemyFlag = false;
				score += 100;

				shotFlag = false;
				shotPosX = SCREEN_SIZE_X;
				shotPosY = SCREEN_SIZE_Y;
			}
		}
		if (shotFlag && insekiFlag)
		{
			//自機の弾と隕石の当たり判定
			if (shotPosX < insekiPosX + INSEKI_SIZE_X &&
				insekiPosX < shotPosX + SHOT_SIZE_X &&
				shotPosY < insekiPosY + INSEKI_SIZE_Y -5&&
				insekiPosY < shotPosY + SHOT_SIZE_Y -5
				)
			{
				insekiFlag = false;
				score += 50;

				shotFlag = false;
				shotPosX = SCREEN_SIZE_X;
				shotPosY = SCREEN_SIZE_Y;
			}
		}



		//描画処理
		SetDrawScreen(DX_SCREEN_BACK);
		ClsDrawScreen();
		DrawGraph(0, bgPosY1, bgImage, true);
		DrawGraph(0, bgPosY2, bgImage, true);
		if (shotFlag) //!shotFlag非発射中
		{
			DrawGraph(shotPosX, shotPosY, shotImage, true);
		}
		if (playerFlag)
		{
			DrawGraph(playerPosX, playerPosY, playerImage, true);
		}
		if (enemyFlag)
		{
			DrawGraph(enemyPosX, enemyPosY, enemyImage, true);
		}
		if (insekiFlag)
		{
			DrawGraph(insekiPosX, insekiPosY, insekiImage, true);
		}
		DrawGraph(0, kumoPosY1, kumoImage, true);
		DrawGraph(0, kumoPosY2, kumoImage, true);
		DrawFormatString(0, 0, 0xffffff, "スコア : %d", score);
		if (zanki == -1)
		{
			DrawFormatString(SCREEN_SIZE_X/2, SCREEN_SIZE_Y/2, 0xffffff, "スコア : %d", score);
		}
		DrawFormatString(560, 464, 0xffffff, "残機 : %d", zanki);
		_dbgDraw();
		ScreenFlip();
		
	}

	DxLib_End();
	return 0;
}














