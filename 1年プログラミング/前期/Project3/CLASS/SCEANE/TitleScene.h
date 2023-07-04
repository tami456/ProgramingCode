#pragma once
#include "SCN_ID.h"
#include "../COMMON/Vector2.h"

class TitleScene
{
public:
	TitleScene();
	~TitleScene();
	bool Init(void);
	bool Release(void);
	SCN_ID Update(char*keyData, char* keyDataOld);
	void Draw(void);
	void TitleDraw(void);
private:
	int blinkCount;
	int titleImage_;
	int screenID_;
	Vector2 screenSize_;
};

