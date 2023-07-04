#pragma once
#include <vector>
#include "SceneBase.h"
class Stage;
class SpaceDome;
class PlayerShip;
class Turret;
class RockManager;
class Rock;

class GameScene : public SceneBase
{

public:
	//自機破壊時空のリスタート時間
	static constexpr float TIME_RESTART = 2.0f;

	// コンストラクタ
	GameScene(void);

	// デストラクタ
	~GameScene(void);

	void Init(void) override;
	void Update(void) override;
	void Draw(void) override;
	void Release(void) override;

private:
	Stage* mStage;
	SpaceDome* mSpaceDome;
	PlayerShip* mPlayerShip;
	Turret* mTurret;
	//背景の岩
	RockManager* mRockManager;

	//自機の破壊演出時間
	float mStepShipdestroy;
};
