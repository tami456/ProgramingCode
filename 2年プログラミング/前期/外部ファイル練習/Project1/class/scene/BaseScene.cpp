#include <DxLib.h>
#include "BaseScene.h"

//コンストの後ろに書くと初期化の予約ができる
//必要なければコンストラクタもデストラクタもいらない
BaseScene::BaseScene(int screenSizeX, int screenSizeY):screenSize_(screenSizeX,screenSizeY)
{

	//GetDrawScreenSize(&screenSize_.x, &screenSize_.y);
	screenID_ = MakeScreen(screenSize_.x, screenSize_.y, true);
}

BaseScene::~BaseScene()
{

}

void BaseScene::Draw(void)
{
	DrawGraph(0, 0, screenID_, true);
}
