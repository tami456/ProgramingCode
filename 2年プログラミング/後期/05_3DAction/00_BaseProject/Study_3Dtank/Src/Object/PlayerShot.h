#pragma once
#include <DxLib.h>
#include "../Common/Quaternion.h"
#include "Common/Transform.h"
class AnimationController;

class PlayerShot
{
public:
	PlayerShot(const Transform* parent);
	~PlayerShot();
	void Init(void);

	void Create(VECTOR birthPos, VECTOR dir);

	void Update(void);

	// 生存チェック＆生存判定
	bool CheckAlive(void);

	void Move(void);
	void Draw(void);
	void Release(void);

	// 爆発エフェクト生成
	void NonAlive(void);

	// 弾の速度
	float GetSpeed(void) const;

	// 生存時間
	float GetTimeAlive(void) const;

	// 生存判定
	bool IsAlive(void) const;

	// 座標の取得
	Transform& GetTransform(void);

	// 球体当たり判定用の半径を取得
	float GetCollisionRadius(void) const;

protected:

	// モデル制御の基本情報
	Transform mTransform;

	AnimationController* mExplosion;

	// 発射方向
	VECTOR mDir;

	// 生存判定
	bool mIsAlive;

	// 生存時間
	float mStepAlive;

	// 当たり判定の半径
	float mCollisionRadius;

private:

	// 弾の標準速度
	static constexpr float DEFAULT_SPEED = 35.0f;

	// 弾の生存時間
	static constexpr float DEFAULT_TIME_ALIVE = 5.0f;

};

