#pragma once

#include <vector>
#include "ShotBase.h" 
#include "../Common/Transform.h"

class TurretShot;
class SpriteAnimator;

class Turret
{
public:

	//衝突判定(球体の判定)
	static constexpr float COLLISION_RADIUS = 200.0f;

	//被ダメージエフェクト
	static constexpr float TIME_DAMAGED_EFFECT = 2.0f;

	//砲台のローカル回転稼働角
	static constexpr float ANGLE_Y_MIN_STAND = -30 * (DX_PI_F / 180.0f);
	static constexpr float ANGLE_Y_MAX_STAND = 30 * (DX_PI_F / 180.0f);

	//砲台のローカル回転量
	static constexpr float ANGLE_POW_STAND = 0.2f;

	//砲身のローカル回転稼働角
	static constexpr float ANGLE_Y_MIN_GUN = -10 * (DX_PI_F / 180.0f);
	static constexpr float ANGLE_Y_MAX_GUN = 20 * (DX_PI_F / 180.0f);

	//砲身のローカル回転量
	static constexpr float ANGLE_POW_GUN = 0.2f;

	// タレットの大きさ
	static constexpr float SCALE = 80.0f;

	//弾の発射間隔
	static constexpr float TIME_DELAY_SHOT = 0.2f;
	// 状態
	enum class STATE
	{
		NONE,
		ATTACK,
		DESTROY
	};

	// コンストラクタ
	Turret(
		const Transform* transformParent,
		VECTOR localPos,
		VECTOR localAddAxis);

	// デストラクタ
	~Turret(void);

	void Init(void);

	void Update(void);
	void UpdateAttack(void);
	void UpdateDestroy(void);
	void UpdateShot(void);
	

	void Draw(void);
	void DrawAttack(void);
	void DrawDestroy(void);
	void DrawShot(void);

	void Release(void);

	// ボス戦艦(親モデル)と回転と位置の同期
	void SyncParent(Transform& transform,VECTOR addAxis);

	// 弾の取得
	std::vector<TurretShot*>& GetShots(void);

	//生存判定
	bool IsAlive(void) const;

	//座標の取得
	VECTOR GetPos(void) const;

	//タレットにダメージを与える処理
	void Damage(void);

private:

	ShotBase* mShotBase;

	//弾
	std::vector<TurretShot*> mShots;

	// ボスの基本情報
	const Transform* mTransformParent;

	//自身の爆発エフェクトの
	SpriteAnimator* mExplosion;


	// モデル制御の基本情報(砲台)
	Transform mTransformStand;

	// モデル制御の基本情報(砲身)
	Transform mTransformGun;

	// 状態
	STATE mState;

	// 戦艦からの相対座標
	VECTOR mLocalPos;

	// 追加のローカル回転
	VECTOR mLocalAddAxis;

	// 更に追加の砲身ローカル回転
	VECTOR mLocalAddAxisStand;

	// 更に追加の砲身ローカル回転
	VECTOR mLocalAddAxisGun;

	// 耐久力
	int mHp;

	//砲台のローカル稼働量(deg)
	float mAnglePowStand;

	//砲身のローカル稼働量(deg)
	float mAnglePowGun;

	//弾の発射間隔
	float mStepDelayShot;

	//被ダメージエフェクトの時間計測用
	float mStepDamaged;

	// 状態遷移
	void ChangeState(STATE state);

	void CreateShot(VECTOR birthPos, VECTOR dir);
	
};

