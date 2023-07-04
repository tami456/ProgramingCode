#include <DxLib.h>
#include "StartMes.h"
#include "../../_debug/_DebugConOut.h"

#define WAIT_TIME (60*3+15)	//開始までの待ち時間

StartMes::StartMes()
{
}

StartMes::~StartMes()
{
}

bool StartMes::Init(Vector2 screenSize)
{
	bool rtnFlag = true;
	imageSize_ = { 210, 150 / 2 };
	if (LoadDivGraph(
		"Resource/image/start_mes.png",
		1*2,
		1,
		2,
		imageSize_.x_,
		imageSize_.y_,
		& Image_[0]
	) == -1)
	{
		TRACE("スタートの読み込み失敗\n");
		rtnFlag = false;
	}
	screenSize_ = screenSize;
	count_ = 0;
	return rtnFlag;
}

bool StartMes::Release(void)
{
	DeleteGraph(Image_[0]);
	DeleteGraph(Image_[1]);
	return true;
}

bool StartMes::Update(void)
{
	if (count_ >= WAIT_TIME)
	{
		return false;
	}

	count_++;
	return true;
}

void StartMes::Draw(void)
{
	if (count_ >= WAIT_TIME)
	{
		return;
	}
	int id = count_ / 90 < 2 ? count_ / 90 : 1;

	DrawGraph(
		(screenSize_.x_ - imageSize_.x_) / 2, 
		(screenSize_.y_ - imageSize_.y_) / 2, 
		Image_[id], true);
}
