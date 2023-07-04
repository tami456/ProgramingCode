#include <string>
#include <DxLib.h>
#include "CharaImage.h"


#include "../../Project3/_debug/_DebugDispOut.h"
#include "../../Project3/_debug/_DebugConOut.h"

#define CHIP_SIZE_X 32
#define CHIP_SIZE_Y 32

CharImage::CharImage()
{

}

CharImage::~CharImage()
{

}

bool CharImage::Init(CharID charID)
{
	bool rtnFlag = true;

	if (charID >= CharID::DRAW)
	{
		return false;
	}

	std::string charNameList[static_cast<int>(CharID::MAX)] ={
		"man",
		"cat",
		"girl",
		"war",
		"mag",
		""
	};

	if (LoadDivGraph(
		("Resource/image/" + charNameList[static_cast<int>(charID)] + ".png").c_str(),
		ANIM_IMAGE_MAX* static_cast<int>(Dir::MAX),
		ANIM_IMAGE_MAX,
		static_cast<int>(Dir::MAX),
		CHIP_SIZE_X,
		CHIP_SIZE_Y,
		&chipImage_[0][0]
	) == -1)
	{
		TRACE("ÉLÉÉÉââÊëúÇÃì«Ç›çûÇ›é∏îs\n");
		rtnFlag = false;
	}
	
	charID_ = charID;
	return rtnFlag;
}

bool CharImage::Release(void)
{
	for (int dir = 0; dir < static_cast<int>(Dir::MAX);dir++)
	{
		for (int anim = 0; anim < ANIM_IMAGE_MAX; anim++)
		{
			DeleteGraph(chipImage_[dir][anim]);
		}
	}
	return true;
}