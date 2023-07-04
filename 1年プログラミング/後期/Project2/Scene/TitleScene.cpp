#include <dxlib.h>
#include "TitleScene.h"
#include "GameScene.h"
#include "SceneMng.h"
#include "../Input/Keyboard.h"
#include "../Input/Gamepad.h"
#include "../common/Frame.h"
#include "../_debug/_DebugConOut.h"
#include "../_debug/_DebugDispOut.h"

TitleScene::TitleScene()
{
	Init();
}

TitleScene::~TitleScene()
{
}

bool TitleScene::Init(void)
{
	titleObj.imageObj = ImageLoader().execute("data/image/title.png");
	titleObj.pos = (lpSceneMng.GetScreenSize() - Vector2D{640,480})/2;
	return true;
}

uniqueScene TitleScene::Update(uniqueScene ownScene)
{
	controller_->Update(*ownScene.get());
	SetDrawScreen(screenID_);
	ClsDrawScreen();
	//DrawString(0, 0, "TitleSceneのUpdate()呼び出し", 0xffffff);

	DrawOwnScreen();
	auto contData = controller_->GetInputData();
	if (contData[NowFlame][static_cast<int>(InputID::Btn3)] && !contData[OldFlame][static_cast<int>(InputID::Btn3)])
	{
		//ownScene.reset(new GameScene());
		//return std::move(ownScene);

		//ゲームシーンが入っているユニークポインタが返る。
		//TitleSceneが破棄されたので、~TitleSceneが呼ばれる。
		return std::make_unique<GameScene>();
	}
	//タイトルシーンが入っているユニークポインタが帰る
	return std::move(ownScene);
}

void TitleScene::DrawOwnScreen(void)
{
	SetDrawScreen(screenID_);
	//DrawString(0, 16, "TitleSceneのDraw()呼び出し", 0xffffff);
	titleObj.Draw();
	if ((blinkCount++ / 30) % 2 == 0)	//前演算後演算に差はない
	{
		int length = GetDrawStringWidth("ＰＬＥＡＳＥ  'C'　ＫＥＹ", 33);
		DrawString((640 - length) / 2, 480 - 150,
			"ＰＬＥＡＳＥ  'C'　ＫＥＹ",
			0xff00ff);
	}
}

