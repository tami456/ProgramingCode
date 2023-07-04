#pragma once
#include <DxLib.h>
#include "Common/Transform.h"
#include "Stage.h"
class ResourceManager;
class GravityManager;
class AnimationController;
class Collider;
class Capsule;
class Player;

class Enemy
{
public:
	//移動スピード
	static constexpr float SPEED_MOVE = 5.0f;

	//回転完了までの時間
	static constexpr float TIME_ROT = 1.0f;

	static constexpr float POS_RADIUS = 20.0f;

	// 状態
	enum class STATE
	{
		NONE,
		PLAY,
		IDLE,
		WARP_RESERVE,
		WARP_MOVE,
		DEAD,
		VICTORY,
		END
	};

	// アニメーション種別
	enum class ANIM_TYPE
	{
		IDLE,
		RUN,
		FAST_RUN,
		JUMP,
		WARP_PAUSE,
		FLY,
		FALLING,
		VICTORY,
		DEAD
	};

	Enemy(Player* player);
	~Enemy();

	void Init(void);
	void InitAnimation(void);
	void Update(void);
	void UpdatePlay(void);
	void UpdateIdle(void);

	void Draw(void);
	void DrawShadow(void);
	void DrawDebug(void);


	void Release(void);

	Transform& GetTransform(void);
	Capsule* GetCapsule(void);
	float GetCollisionRadius(void) const;

	//生存判定
	bool IsAlive(void) const;

	//回転(振り向きたい角度を決める)
	void SetGoalRotate(float rotRad);

	//回転(ゴールに対して、球面補完を行う)
	void Rotate(void);

	//操作
	void ProcessMove(void);

	//重力の計算
	void CalcGravityPow(void);

	//傾斜の計算
	void CalcSlope(void);

	//衝突判定に用いられるコライダ
	void AddCollider(Collider* collider);
	void ClearCollider(void);

	//敵にダメージ入れる
	void Damage(void);

	bool IsDead(void);

private:
	ResourceManager& mResourceManager;
	GravityManager& mGravityManager;

	Transform mTransform;

	// アニメーション
	AnimationController* mAnimationController;

	// 状態
	STATE mState;

	//プレイヤーの情報
	Player* mPlayer;

	//移動スピード
	float mSpeed;

	//移動方向
	VECTOR mMoveDir;

	//移動量
	VECTOR mMovePow;

	//移動後の座標
	VECTOR mMovedPos;

	VECTOR dir;

	//接地点
	VECTOR mHitPos;

	//回転(現在のプレイヤーの向き)
	Quaternion mPlayerRotY;

	//回転(到達したい向き=ゴール)
	Quaternion mGoalQuaRot;

	//回転の時間計測用
	float mStepRot;

	//ジャンプ量
	VECTOR mJumpPow;

	//衝突判定に用いられるコライダ
	Capsule* mCapsule;
	std::vector<Collider*> mColliders;

	//設置衝突で使用する線分座標
	VECTOR mGravHitDown;
	VECTOR mGravHitUp;

	//丸影
	int mImgShadow;

	//傾斜の方向
	VECTOR mSlopeDir;

	//傾斜角
	float mSlopeAngleDeg;

	//傾斜の移動量
	VECTOR mSlopePow;

	//今、プレイヤーが設置しているポリゴンの法線
	VECTOR mHitNormal;

	//フレーム間の移動値
	VECTOR mMoveDiff;

	//生存判定
	bool mIsAlive;

	int mHp;

	// 状態遷移
	void ChangeState(STATE state);

	//衝突判定用の関数
	void Collision(void);
	void CollisionCapsule(void);
	void CollisionGravity(void);
};

