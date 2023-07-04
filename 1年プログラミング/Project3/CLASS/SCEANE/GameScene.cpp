#include <DxLib.h>
#include "GameScene.h"

#include "../../_debug/_DebugDispOut.h"
#include "../../_debug/_DebugConOut.h"



GameScene::GameScene()
{
	TRACE("GameSceneの生成\n");
}

GameScene::~GameScene()
{
	TRACE("GameSceneの破棄\n");
	Release();
}

bool GameScene::Init(CharID player1, CharID player2)
{
	TRACE("GameSceneのInitの呼び出し\n");
	GetScreenState(&screenSize_.x_, &screenSize_.y_, nullptr);
	gameScreenSize_ = { screenSize_.x_, 563 };
	screenID_ = MakeScreen(screenSize_.x_, screenSize_.y_, true);
	gameScreenID_ = MakeScreen(gameScreenSize_.x_, gameScreenSize_.y_, true);

	bgImage = LoadGraph("Resource/image/bg.png");
	if (bgImage == -1)
	{
		TRACE("bgImageの読み込み失敗\n");
		return false;
	}

	chipSize_ = { 32,32 };

	mapCtl_.Init();
	player_[0].Init(player1, true, chipSize_);
	player_[1].Init(player2, true, chipSize_);
	startMes_.Init(gameScreenSize_);

	return true;
}

bool GameScene::Release(void)
{
	TRACE("GameSceneのReleaseの呼び出し\n");
	DeleteGraph(bgImage);

	for (int no = 0; no < PLAYER_MAX; no++)
	{
		player_[no].Release();
	}
	startMes_.Release();
	mapCtl_.Release();
	DeleteGraph(screenID_);
	DeleteGraph(gameScreenID_);
	return true;
}

SCN_ID GameScene::Update(char* keyData, char* keyDataOld)
{
	TRACE("GameSceneのUpdateの呼び出し\n");
	int deadCnt = 0;
	if (!startMes_.Update())
	{
		mapCtl_.Update();
		for (int no = 0; no < PLAYER_MAX; no++)
		{
			player_[no].GetKeyState(keyData);
			if (player_[no].Update(&mapCtl_) == Player::State::DEAD)
			{
				deadCnt++;
			}
		}
	}
	GameDraw();
	if (deadCnt > 0)
	{
		return SCN_ID::RESULT;
	}
	return SCN_ID::GAME;
}

void GameScene::Draw(void)
{
	DrawGraph(0, 0, screenID_, true);
	DrawGraph(0, 64, gameScreenID_, true);
}

void GameScene::GameDraw(void)
{
	TRACE("GameSceneのDrawの呼び出し\n");
	SetDrawScreen(screenID_);
	ClsDrawScreen();
	DrawGraph(0, 0, bgImage, true);

	SetDrawScreen(gameScreenID_);
	ClsDrawScreen();
	mapCtl_.Draw();
	for (int no = 0; no < PLAYER_MAX; no++)
	{
		player_[no].Draw();
	}
	startMes_.Draw();
}

GameScene::Winner GameScene::GetWinner(void)
{
	
	if (player_[0].GetState() == Player::State::ALIVE)
	{
		return Winner::PL1;
	}
	else if (player_[1].GetState() == Player::State::ALIVE)
	{
		return Winner::PL2;
	}
	else
	{
		return Winner::DRAW;
	}
	
}
