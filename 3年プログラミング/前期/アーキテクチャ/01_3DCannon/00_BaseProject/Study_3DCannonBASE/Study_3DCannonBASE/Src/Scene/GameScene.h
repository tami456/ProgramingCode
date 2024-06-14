#pragma once
#include "SceneBase.h"
#include "../Object/Stage.h"
#include "../Object/Cannon.h"

class SceneManager;
class EnemyBase;
class Enemy;

class GameScene :
    public SceneBase
{
public:

	//ゲームオーバー地点衝突判定用球体半径
	static constexpr float OVER_COL_RADIUS = 35.0f;

	// コンストラクタ
	GameScene(void);

	// デストラクタ
	~GameScene(void);

	void Init(void) override;
	void Update(void) override;
	void Draw(void) override;
	void Release(void) override;

private:

	Stage* stage_;
	Cannon* cannon_;

	//敵の画像
	int enemyModelId_;
	std::vector<EnemyBase*> enemys_;
	EnemyBase* enemy_;

	//ゲームオーバー地点
	VECTOR gameoverPoint_;

	//ゲームオーバー判定
	bool isGameover_;

	//ゲームオーバー画像
	int imgGameover_;

	//ゲームオーバー判定
	bool isGameClear_;

	//ゲームオーバー画像
	int imgGameClear_;
};

