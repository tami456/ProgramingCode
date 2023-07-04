#include <dxlib.h>
#include "BaseScene.h"
#include "../Input/Keyboard.h"
#include "../Input/Gamepad.h"

BaseScene::BaseScene()
{

	SetDrawScreen(DX_SCREEN_BACK);
	GetDrawScreenSize(&screenSize_.x, &screenSize_.y);
	screenID_ = MakeScreen(screenSize_.x, screenSize_.y,true);
	Init();
}

BaseScene::~BaseScene()
{
	
}

bool BaseScene::Init(void)
{
	if (GetJoypadNum())
	{
		controller_ = std::make_unique<Gamepad>();
	}
	else
	{
		controller_ = std::make_unique<Keyboard>();
	}
	return true;
}

//void BaseScene::Update(void)
//{
//	SetDrawScreen(screenID_);
//	ClsDrawScreen();
//	DrawString(0,0,"BaseSceneÇÃUpdate()åƒÇ—èoÇµ",0xffffff);
//	
//	
//	DrawOwnScreen();
//}

void BaseScene::DrawOwnScreen(void)
{
	SetDrawScreen(screenID_);
	DrawString(0, 16, "BaseSceneÇÃDraw()åƒÇ—èoÇµ", 0xffffff);
}

void BaseScene::Draw(void)
{
	DrawGraph(drawOffset_.x, drawOffset_.y, screenID_, true);
}
