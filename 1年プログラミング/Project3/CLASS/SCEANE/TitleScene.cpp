#include <DxLib.h>
#include "TitleScene.h"


#include "../../_debug/_DebugDispOut.h"
#include "../../_debug/_DebugConOut.h"

#define SCREEN_SIZE_X 800	//‰æ–Ê‚Ì‰¡ƒTƒCƒY
#define SCREEN_SIZE_Y 600	//‰æ–Ê‚ÌcƒTƒCƒY

TitleScene::TitleScene()
{
	TRACE("TitleScene‚Ì¶¬\n");
}

TitleScene::~TitleScene()
{
	TRACE("TitleScene‚Ì”jŠü\n");
	Release();
}

bool TitleScene::Init(void)
{
	TRACE("TitleScene‚ÌInit‚ÌŒÄ‚Ño‚µ\n");
	GetScreenState(&screenSize_.x_, &screenSize_.y_, nullptr);
	screenID_ = MakeScreen(screenSize_.x_, screenSize_.y_,true);
	titleImage_ = LoadGraph("Resource/image/Title.png");
	TRACE("titleImage:%d\n", titleImage_);
	return true;
}

bool TitleScene::Release(void)
{
	TRACE("TitleScene‚ÌRelease‚ÌŒÄ‚Ño‚µ\n");
	
	DeleteGraph(titleImage_);
	DeleteGraph(screenID_);
	return true;
}

SCN_ID TitleScene::Update(char* keyData, char* keyDataOld)
{
	TRACE("TitleScene‚ÌUpdate‚ÌŒÄ‚Ño‚µ\n");
	TitleDraw();
	if (keyData[KEY_INPUT_SPACE] && !keyDataOld[KEY_INPUT_SPACE])
	{
		return SCN_ID::SELECT;
	}
	return SCN_ID::TITLE;
}

void TitleScene::Draw(void)
{
	DrawGraph(0, 0, screenID_, true);
}

void TitleScene::TitleDraw(void)
{
	SetDrawScreen(screenID_);
	ClsDrawScreen();
	DrawGraph(0, 0, titleImage_, false);
	if ((blinkCount++ / 30) % 2 == 0)	//‘O‰‰ZŒã‰‰Z‚É·‚Í‚È‚¢
	{
		int length = GetDrawStringWidth("‚o‚k‚d‚`‚r‚d  '‚r‚o‚`‚b‚d'@‚j‚d‚x", 33);
		DrawString((SCREEN_SIZE_X - length) / 2, SCREEN_SIZE_Y - 150,
			"‚o‚k‚d‚`‚r‚d  '‚r‚o‚`‚b‚d'@‚j‚d‚x",
			0x000000);
	}
	TRACE("TitleScene‚ÌDraw‚ÌŒÄ‚Ño‚µ\n");
	
}
