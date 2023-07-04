#pragma once
#include "SceneBase.h"

class SceneManager;
class Stage;
class SkyDome;
class Player;
class Enemy;
class Mascot;

class GameScene : public SceneBase
{

public:

	GameScene(void);
	void Init(void) override;
	void Update(void) override;
	void Draw(void) override;
	void DrawDebug(void);
	void Release(void) override;

private:

	Stage* mStage;
	SkyDome* mSkyDome;
	Player* mPlayer;
	Enemy* mEnemy;
	Mascot* mMascot;

	
};
