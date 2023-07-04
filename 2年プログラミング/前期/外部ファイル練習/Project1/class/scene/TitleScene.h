#pragma once
#include "BaseScene.h"


class TitleScene :
    public BaseScene
{
public:
	TitleScene(int screenSizeX, int screenSizeY);
	~TitleScene();
private:
	//public‚É’u‚¢‚Ä‚µ‚Ü‚¤‚Æ”h¶‚ÅéŒ¾‚µ‚Äg‚¦‚Ä‚µ‚Ü‚¤
	bool Init(void) override;
	uniquScene Update(uniquScene ownScene) override;
	void DrawOwnScreen(void)override;

};

