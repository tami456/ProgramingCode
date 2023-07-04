#include <dxlib.h>
#include "ResultScene.h"
#include "TitleScene.h"
#include "SceneMng.h"
#include "../Input/Keyboard.h"
#include "../Input/Gamepad.h"
#include "../common/Frame.h"
#include "../_debug/_DebugConOut.h"
#include "../_debug/_DebugDispOut.h"

ResultScene::ResultScene()
{
}

ResultScene::~ResultScene()
{
}

void ResultScene::Init(void)
{
}

uniqueScene ResultScene::Update(uniqueScene ownScene)
{
	controller_->Update(*ownScene.get());
	SetDrawScreen(screenID_);
	ClsDrawScreen();
	//DrawString(0, 0, "ResultSceneのUpdate()呼び出し", 0xffffff);

	DrawOwnScreen();
	auto contData = controller_->GetInputData();
	if (contData[NowFlame][static_cast<int>(InputID::Btn3)] && !contData[OldFlame][static_cast<int>(InputID::Btn3)])
	{
		/*ownScene.reset(new GameScene());
		return std::move(ownScene);*/

		//ゲームシーンが入っているユニークポインタが返る。
		//TitleSceneが破棄されたので、~TitleSceneが呼ばれる。
		return std::make_unique<TitleScene>();
	}

	return std::move(ownScene);
}

void ResultScene::DrawOwnScreen(void)
{
	SetDrawScreen(screenID_);
	//DrawString(0, 16, "ResultSceneのDraw()呼び出し", 0xffffff);
}

