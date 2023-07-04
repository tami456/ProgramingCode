#pragma once
#include "../Object//Common/Transform.h"
#include "../Common/Quaternion.h"
class SceneManager;
class Planet;
class Player;
class Enemy;

class GravityManager
{

public:

	// 重力方向が完全に変わるまでの時間
	static constexpr float TIME_CHANGE_DIR = 1.0f;

	// 回転力
	static constexpr float DEFAULT_POW_SLERP = 0.08f;

	// 状態
	enum class STATE
	{
		IDLE,
		STAGE_CHANGE
	};

	// インスタンスの生成
	static void CreateInstance(void);

	// インスタンスの取得
	static GravityManager& GetInstance(void);

	void Init(void);

	void Update(void);
	void UpdateIdle(void);
	void UpdateChangeStage(void);

	// 重力方向や、正方向の計算
	void Calculate(void);

	// 現在プレイしている惑星
	Planet* GetActivePlanet(void) const;

	// 現在プレイしている惑星を変更する
	void ChangeActivePlanet(Planet* planet);

	// 操作プレイヤーをセット
	void SetPlayer(Player* player);
	void SetEnemy(Enemy* enemy);

	const Transform& GetTransform(void) const;
	
	// 重力方向を取得
	VECTOR GetDirGravity(void) const;

	// 重力方向の反対方向を取得
	VECTOR GetDirUpGravity(void) const;

	// 重力の強さを取得
	float GetPower(void) const;

	// 惑星切り替え前の回転情報
	Quaternion GetPremQuaRot(void) const;

	// プレイヤーと足元衝突した法線
	void SetLastHitNormal(const VECTOR& normal);

	// プレイヤーと足元衝突した位置
	void SetLastHitPos(const VECTOR& pos);
	VECTOR GetLastHitPos(void) const;

protected:

	// 静的インスタンス
	static GravityManager* mInstance;

	// プレイヤー
	Player* mPlayer;
	Enemy* mEnemy;

	// 現在プレイしている惑星
	Planet* mActivePlanet;

	Transform mTransform;

	// 状態
	STATE mState;

	// 重力方向
	VECTOR mDirGravity;
	VECTOR mPreDirGravity;

	// 重力方向の反対方向
	VECTOR mDirUpGravity;

	// プレイヤーと足元衝突した法線
	VECTOR mLastHitNormal;

	// プレイヤーと足元衝突した位置
	VECTOR mLastHitPos;

	// 更新ステップ
	float mStep;

	// 回転力
	double mSlerpPow;

	// デフォルトコンストラクタをprivateにして、
	// 外部から生成できない様にする
	GravityManager(void);
	~GravityManager(void);

	// 状態遷移
	void ChangeState(STATE state);

	// 重力方向を計算
	VECTOR CalcDirGravity(void) const;

};

