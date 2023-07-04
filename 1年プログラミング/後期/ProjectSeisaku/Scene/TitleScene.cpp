#include <dxlib.h>
#include "TitleScene.h"
#include "GameScene.h"
#include "SceneMng.h"
#include "../Input/Keyboard.h"
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
	sH = LoadSoundMem("data/bgm/Comical_Cats.mp3");
	volume = 100;
	sH2 = LoadSoundMem("data/bgm/魔王魂 効果音 システム48.mp3");
	vol2 = 120;
	titleObj.imageObj = ImageLoader().execute("data/image/title.png");
	ChangeVolumeSoundMem(vol2, sH2);
	ChangeVolumeSoundMem(volume, sH);
	PlaySoundMem(sH, DX_PLAYTYPE_LOOP);
	
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
		StopSoundMem(sH);
		PlaySoundMem(sH2, DX_PLAYTYPE_NORMAL);
		return std::make_unique<GameScene>();
	}
	//タイトルシーンが入っているユニークポインタが帰る
	return std::move(ownScene);
}

void TitleScene::DrawOwnScreen(void)
{
	SetDrawScreen(screenID_);
	DrawString(0, 0, "TitleSceneのDraw()呼び出し", 0xffffff);
	titleObj.Draw();
	if ((blinkCount++ / 30) % 2 == 0)	//前演算後演算に差はない
	{
		int length = GetDrawStringWidth("ＰＬＥＡＳＥ  'C'　ＫＥＹ", 33);
		DrawString((640 - length) / 2, 480 - 150,
			"ＰＬＥＡＳＥ  'C'　ＫＥＹ",
			0xff00ff);
	}
}

