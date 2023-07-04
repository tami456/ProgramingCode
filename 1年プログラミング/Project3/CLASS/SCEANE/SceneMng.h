#pragma once
#include "SCN_ID.h"
#include "TitleScene.h"
#include "SelectScene.h"
#include "GameScene.h"
#include "ResultScene.h"
#include "Player.h"
#include "../CharaImage.h"
class SceneMng
{
public:
	SceneMng();
	~SceneMng();
	void Run(void);

private:		//äOÇ©ÇÁêGÇÈÇ±Ç∆Ç™Ç≈Ç´Ç»Ç¢
	void SetupScene(void);

	bool Init(void);
	bool Release(void);
	void Update(void);
	void Draw(void);

	void ReleaseScene(void);

	bool SysInit(void);

	SCN_ID scnID_;
	SCN_ID nextScnID_;
	TitleScene titleScene_;
	SelectScene selectScene_;
	GameScene gameScene_;
	ResultScene resultScene_;
	char keyBuf_[256];
	char keyBufOld_[256];

};