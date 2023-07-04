#pragma once

#include <string>		//文字列を扱うことができるクラス
#include <dxlib.h>
#include <vector>
#include "Vector2D.h"
#include "../_debug/_DebugConOut.h"
#include "../_debug/_DebugDispOut.h"

using ImageVec = std::vector<int>;

struct DrawObj			//描画用のオブジェクト
{
	ImageVec imageObj;		//画像を格納
	Vector2D pos;
	void Draw(void);
};

struct ImageLoader		//画像読み込み用のクラス
{
	ImageVec execute(std::string imageName)//LoadGraph版
	{
		ImageVec imageObj;
		imageObj.resize(1);
		imageObj[0] = LoadGraph(imageName.c_str());
		if (imageObj[0] == -1)
		{
			TRACE("画像の読み込み失敗");
		}
		return std::move(imageObj);
	}
	ImageVec execute(std::string imageName,Vector2D divCnt,Vector2D divSize)//LoadDivGraph版
	{
		ImageVec imageObj;
		imageObj.resize(static_cast<size_t>(divCnt.x) * static_cast<size_t>(divCnt.y));
		
		auto flag = LoadDivGraph(("data/image/"+imageName).c_str(),
			divCnt.x * divCnt.y,divCnt.x,divCnt.y,
			divSize.x, divSize.y, &imageObj[0]);

		//imageObj.data() &imageObj[0]
		if (flag == -1)
		{
			TRACE("画像の読み込み失敗");
		}

		return std::move(imageObj);
	}
};

