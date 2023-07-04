#pragma once
#include "BaseScene.h"
#include "../common/Vector2D.h"
#include "../common/ImageCtl.h"

class TitleScene:
	public BaseScene
{
public:
	TitleScene();
	~TitleScene();
	bool Init(void);
	uniqueScene Update(uniqueScene ownScene) override;
	void DrawOwnScreen(void) override;
private:
	DrawObj titleObj;
	int sH;
	int sH2;
	int volume;
	int vol2;
	int blinkCount;
};

