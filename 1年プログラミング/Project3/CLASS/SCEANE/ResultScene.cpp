#include <DxLib.h>
#include "ResultScene.h"
#include "../../_debug/_DebugDispOut.h"
#include "../../_debug/_DebugConOut.h"

#define WIN_MAS_IMAGE_SIZE_X 450
#define WIN_MAS_IMAGE_SIZE_Y (150/3)

ResultScene::ResultScene()
{
	TRACE("ResultSceneの生成\n");
}

ResultScene::~ResultScene()
{
	TRACE("ResultSceneの破棄\n");
	Release();
}

bool ResultScene::Init(GameScene::Winner winnerID)
{
	TRACE("ResultSceneのInitの呼び出し\n");
	GetScreenState(&screenSize_.x_, &screenSize_.y_, nullptr);
	screenID_ = MakeScreen(screenSize_.x_, screenSize_.y_, true);
	bgScreenID_ = MakeScreen(screenSize_.x_, screenSize_.y_, true);
	GetDrawScreenGraph(0,0, screenSize_.x_, screenSize_.y_, bgScreenID_);
	winnerID_ = winnerID;

	LoadDivGraph(
		"Resource/image/Result.png",
		3,
		1,
		3,
		WIN_MAS_IMAGE_SIZE_X,
		WIN_MAS_IMAGE_SIZE_Y,
		winMesImage_);
	return true;
	
}

bool ResultScene::Release(void)
{
	TRACE("ResultSceneのReleaseの呼び出し\n");
	DeleteGraph(screenID_);
	DeleteGraph(bgScreenID_);
	for (int j = 0; j < 3; j++)
	{
		DeleteGraph(winMesImage_[j]);
	}
	return true;
}

SCN_ID ResultScene::Update(char* keyData, char* keyDataOld)
{
	TRACE("ResultSceneのUpdateの呼び出し\n");
	ResultDraw();
	if (keyData[KEY_INPUT_SPACE] && !keyDataOld[KEY_INPUT_SPACE])
	{
		return SCN_ID::TITLE;
	}
	return SCN_ID::RESULT;
}

void ResultScene::Draw(void)
{
	DrawGraph(0, 0, screenID_, true);
}

void ResultScene::ResultDraw(void)
{
	SetDrawScreen(screenID_);
	ClsDrawScreen();
	SetDrawBlendMode(DX_BLENDMODE_INVSRC, 255);
	DrawGraph(0, 0, bgScreenID_,true);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
	DrawGraph(
		(screenSize_.x_ - WIN_MAS_IMAGE_SIZE_X) / 2,
		8, 
		winMesImage_[static_cast<int>(winnerID_)],true);
	TRACE("ResultSceneのDrawの呼び出し\n");

}