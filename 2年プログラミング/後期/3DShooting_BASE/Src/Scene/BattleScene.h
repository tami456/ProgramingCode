#pragma once
#include "SceneBase.h"
class SpaceDome;
class BossShip;
class RockManager;
class PlayerShip;
class Turret;


class BattleScene : public SceneBase
{
public:
	//自機破壊時空のリスタート時間
	static constexpr float TIME_RESTART = 2.0f;

	//ボス破壊からのリスタート時間
	static constexpr float TIME_RESTART_BOSS_DESTROY = 5.0f;

	// コンストラクタ
	BattleScene(void);

	// デストラクタ
	~BattleScene(void);

	void Init(void) override;
	void Update(void) override;
	void Draw(void) override;
	void Release(void) override;

private:

	//背景岩
	RockManager* mRockManager;
	
	//スカイドーム
	SpaceDome* mSpaceDome;

	//ボスシップ
	BossShip* mBossShip;

	//自機
	PlayerShip* mPlayerShip;

	Turret* mTurret;

	//自機の破壊演出時間
	float mStepShipDestroy;

	//ゲームクリア画像
	int mImgEndLogo;

	//ボス破壊後の演出時間
	float mStepBossDestroy;
};

