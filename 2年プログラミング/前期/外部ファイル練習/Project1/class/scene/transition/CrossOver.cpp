#include <DxLib.h>
#include "CrossOver.h"

CrossOver::CrossOver(uniquScene beforScene, uniquScene afterScene, int screenSizeX, int screenSizeY):
	Transition(std::move(beforScene), std::move(afterScene), screenSizeX, screenSizeY)
{
	count_ = 255;
	countA_ = 0;
	DrawOwnScreen();
}

CrossOver::~CrossOver()
{

}

bool CrossOver::UpdateTransition(void)
{
	//255Ç…çsÇ¡ÇΩÇÁtrueÇ…
	count_--;

	

	if (count_ < 0)
	{
		count_ = 0;
		return true;
	}

	countA_++;

	if (countA_ == 255);

	DrawOwnScreen();
	return false;

	
}

void CrossOver::DrawOwnScreen(void)
{
	SetDrawScreen(screenID_);
	ClsDrawScreen();
	
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, (int)count_);
	beforScene_->Draw();
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);


	SetDrawBlendMode(DX_BLENDMODE_ALPHA, (int)countA_);
	afterScene_->Draw();
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
}
