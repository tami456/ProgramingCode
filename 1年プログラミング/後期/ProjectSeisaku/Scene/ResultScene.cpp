#include <dxlib.h>
#include "ResultScene.h"
#include "TitleScene.h"
#include "SceneMng.h"
#include "../Input/Keyboard.h"
#include "../common/Frame.h"
#include "../_debug/_DebugConOut.h"
#include "../_debug/_DebugDispOut.h"

ResultScene::ResultScene()
{
	Init();
}

ResultScene::~ResultScene()
{
}

bool ResultScene::Init(void)
{
	sh = LoadSoundMem("data/bgm/Comical_Cats.mp3");
	vol = 100;
	ChangeVolumeSoundMem(vol, sh);
	resultImage = LoadGraph("./data/image/result.png");
	PlaySoundMem(sh, DX_PLAYTYPE_LOOP);
	return true;
}

uniqueScene ResultScene::Update(uniqueScene ownScene)
{
	controller_->Update(*ownScene.get());
	SetDrawScreen(screenID_);
	ClsDrawScreen();
	DrawOwnScreen();
	auto contData = controller_->GetInputData();
	if (contData[NowFlame][static_cast<int>(InputID::Btn3)] && !contData[OldFlame][static_cast<int>(InputID::Btn3)])
	{
		StopSoundMem(sh);
		return std::make_unique<TitleScene>();
	}

	return std::move(ownScene);
}

void ResultScene::DrawOwnScreen(void)
{
	SetDrawScreen(screenID_);
	Draw();
	DrawString(0, 0, "ResultScene‚ÌDraw()ŒÄ‚Ño‚µ", 0xffffff);
}

void ResultScene::Draw()
{
	DrawGraph(0, 0, resultImage, true);
	if ((blinkCount++ / 30) % 2 == 0)	//‘O‰‰ZŒã‰‰Z‚É·‚Í‚È‚¢
	{
		int length = GetDrawStringWidth("‚o‚k‚d‚`‚r‚d  'C'@‚j‚d‚x", 33);
		DrawString((640 - length) / 2, 480 - 150,
			"‚o‚k‚d‚`‚r‚d  'C'@‚j‚d‚x",
			0xff00ff);
	}
}



