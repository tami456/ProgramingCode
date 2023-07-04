#include <DxLib.h>
#include "SelectScene.h"

#include "../../_debug/_DebugDispOut.h"
#include "../../_debug/_DebugConOut.h"


SelectScene::SelectScene()
{
	TRACE("SelectSceneの生成\n");
}

SelectScene::~SelectScene()
{
	TRACE("SelectSceneの破棄\n");
	Release();
}

bool SelectScene::Init(void)
{
	TRACE("SelectSceneのInitの呼び出し\n");
	GetScreenState(&screenSize_.x_, &screenSize_.y_, nullptr);
	screenID_ = MakeScreen(screenSize_.x_, screenSize_.y_, true);
	gameScreenID_ = MakeScreen(screenSize_.x_, 536, true);
	for (int j = 0; j < static_cast<int>(CharID::MAX); j++)
	{
		charImage_[j].Init(static_cast<CharID>(j));
	}



	//if ((bgImage = LoadGraph("Resource/image/bg.png")) == -1)
	bgImage = LoadGraph("Resource/image/bg.png");
	if (bgImage == -1)
	{
		TRACE("bgImageの読み込み失敗\n");
		return false;
	}
	selectImage = LoadGraph("Resource/image/char_sel.png");
	if (selectImage == -1)
	{
		TRACE("selectImageの読み込み失敗\n");
		return false;
	}
	GetScreenState(&screenSize_.x_,&screenSize_.y_,nullptr);

	for (int no = 0; no < PLAYER_MAX; no++)
	{
		player_[no].Init(static_cast<CharID>(no), false, { 0,0 });
	}
	return true;
}

bool SelectScene::Release(void)
{
	TRACE("SelectSceneのReleaseの呼び出し\n");
	DeleteGraph(bgImage);
	DeleteGraph(selectImage);
	DeleteGraph(screenID_);
	for (int no = 0; no < static_cast<int>(CharID::MAX); no++)
	{
		charImage_[no].Release();
	}

	for (int no = 0; no < PLAYER_MAX; no++)
	{
		player_[no].Release();
	}
	DeleteGraph(screenID_);
	DeleteGraph(gameScreenID_);
	return true;
}

SCN_ID SelectScene::Update(char* keyData, char* keyDataOld)
{
	TRACE("SelectSceneのUpdateの呼び出し\n");
	
	for (int no = 0; no < PLAYER_MAX; no++)
	{
		player_[no].GetKeyState(keyData);
		if (player_[no].keyNow_[static_cast<int>(Dir::LEFT)]
		&& !player_[no].keyOld_[static_cast<int>(Dir::LEFT)])
		{
			if (player_[no].charID_ != CharID::MAN)
			{
				player_[no].charID_ = static_cast<CharID>(static_cast<int>(player_[no].charID_) - 1);
			}
			else
			{
				player_[no].charID_ = CharID::MAG;
			}
		}

		if (player_[no].keyNow_[static_cast<int>(Dir::RIGHT)]
		&& !player_[no].keyOld_[static_cast<int>(Dir::RIGHT)])
		{

			if (player_[no].charID_ != CharID::MAG)
			{
				player_[no].charID_ = static_cast<CharID>(static_cast<int>(player_[no].charID_) + 1);
			}
			else
			{
				player_[no].charID_ = CharID::MAN;
			}
		}
		if (player_[no].keyNow_[static_cast<int>(Dir::UP)]
			&& !player_[no].keyOld_[static_cast<int>(Dir::UP)])
		{
			player_[no].SetState(Player::State::ALIVE);
		}
		if (player_[no].keyNow_[static_cast<int>(Dir::DOWN)]
			&& !player_[no].keyOld_[static_cast<int>(Dir::DOWN)])
		{
			player_[no].SetState(Player::State::NON);
		}
	}

	SelectDraw();
	if ((player_[0].GetState() == Player::State::ALIVE) && (player_[1].GetState() == Player::State::ALIVE))
	{
		return SCN_ID::GAME;
	}
	return SCN_ID::SELECT;
}


void SelectScene::Draw(void)
{
	DrawGraph(0, 0, screenID_, true);
	DrawGraph(0, 64, gameScreenID_, true);
}

void SelectScene::SelectDraw(void)
{
	SetDrawScreen(screenID_);
	ClsDrawScreen();
	DrawGraph(0, 0, bgImage, true);
	DrawGraph((screenSize_.x_ - 400) / 2, 0, selectImage, true);

	SetDrawScreen(gameScreenID_);
	ClsDrawScreen();
	

	//for (int no = 0; no < static_cast<int>(CharID::DRAW); no++)
	//{
	//	DrawGraph(no*32, 0, charImage_[no].chipImage_[0][0], true);
	//}
	for (int no = 0; no < PLAYER_MAX; no++)
	{
		player_[no].Draw(charImage_);
	}
	
	TRACE("SelectSceneのDrawの呼び出し\n");
}

CharID SelectScene::GetSelPlayer(int no)
{
	if (no > PLAYER_MAX)
	{
		//プレイヤーを超えた指定の場合は、とりあえずMANで返す
		return CharID::MAN;
	}
	return player_[no].charID_;
}
