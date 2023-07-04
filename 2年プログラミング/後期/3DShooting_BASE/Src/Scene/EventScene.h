#pragma once
#include "SceneBase.h"
class SpaceDome;
class BossShip;

class EventScene : public SceneBase
{

public:

	// コンストラクタ
	EventScene(void);

	// デストラクタ
	~EventScene(void);

	void Init(void) override;
	void Update(void) override;
	void Draw(void) override;
	void Release(void) override;

private:

	//スカイドーム
	SpaceDome* mSpaceDome;

	//ボスシップ
	BossShip* mBossShip;
};

