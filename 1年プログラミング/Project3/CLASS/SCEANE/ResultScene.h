#pragma once
#include "SCN_ID.h"
#include "Player.h"
#include "../COMMON/Vector2.h"
#include "GameScene.h"

class ResultScene
{
public:
	ResultScene();
	~ResultScene();
	bool Init(GameScene::Winner winnerID);
	bool Release(void);
	SCN_ID Update(char* keyData, char* keyDataOld);
	void Draw(void);
	void ResultDraw(void);
private:
	int screenID_;
	int bgScreenID_;
	int winMesImage_[3];
	Vector2 screenSize_;
	GameScene::Winner winnerID_;
};

