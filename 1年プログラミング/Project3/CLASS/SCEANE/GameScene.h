#pragma once
#include "SCN_ID.h"
#include "Player.h"
#include "../COMMON/Vector2.h"
#include "StartMes.h"
#include "MapCtl.h"
#include "../CharaImage.h"
class GameScene
{
public:
	enum class Winner{
		PL1,
		PL2,
		DRAW
	};

	GameScene();
	~GameScene();
	bool Init(CharID player1,CharID player2);
	bool Release(void);
	SCN_ID Update(char* keyData, char* keyDataOld);
	void Draw(void);
	void GameDraw(void);
	Winner GetWinner(void);
private:
	int bgImage;
	Player player_[PLAYER_MAX];
	int screenID_;
	int gameScreenID_;
	Vector2 screenSize_;
	Vector2 gameScreenSize_;
	Vector2 chipSize_;
	StartMes startMes_;
	MapCtl mapCtl_;
};

