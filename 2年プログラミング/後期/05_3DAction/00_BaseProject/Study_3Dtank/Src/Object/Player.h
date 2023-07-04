#pragma once
#include <DxLib.h>
#include "Common/Transform.h"
#include "Stage.h"

class ResourceManager;
class GravityManager;
class Collider;
class Capsule;
class PlayerShot;


class Player
{
public:

	//移動スピード
	static constexpr float SPEED_MOVE = 5.0f;

	//弾の生存時間
	static constexpr float ALIVE_SHOT = 10.0f;

	//回転完了までの時間
	static constexpr float TIME_ROT = 1.0f;

	//煙エフェクトの発生間隔
	static constexpr float TIME_FOOT_SMOKE = 0.3f;

	//弾の発射間隔
	static constexpr float TIME_DELAY_SHOT = 0.2f;

	// タレットの大きさ
	static constexpr float SCALE = 0.3f;

	//砲台のローカル回転稼働角
	static constexpr float ANGLE_Y_MIN_STAND = -30 * (DX_PI_F / 180.0f);
	static constexpr float ANGLE_Y_MAX_STAND = 30 * (DX_PI_F / 180.0f);

	static constexpr float ANGLE_Y_MIN_GUN = -10 * (DX_PI_F / 180.0f);
	static constexpr float ANGLE_Y_MAX_GUN = 20 * (DX_PI_F / 180.0f);

	// 状態
	enum class STATE
	{
		NONE,
		PLAY,
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
		KICK
	};

	Player(void);
	~Player(void);

	void Init(void);
	void Update(void);
	void UpdatePlay(void);
	void UpdateDead(void);

	void Draw(void);

	void Release(void);

	void Attack(void);

	Transform* GetTransform(void);
	Capsule* GetCapsule(void);

	//操作
	void ProcessMove(void);
	void ProcessShot(void);

	//傾斜の計算
	void CalcSlope(void);

	//重力の計算
	void CalcGravityPow(void);

	//衝突判定に用いられるコライダ
	void AddCollider(Collider* collider);
	void ClearCollider(void);

	//値渡しだとコピーしないといけないから時間がかかる
	//参照渡しだとここにあると教えてくれるだけ
	//const付けながら参照渡しをすることで安全にデータを渡せる

	//プレイヤーの状態判断
	bool IsPlay(void);
	bool IsDead(void);

	//弾の取得
	std::vector<PlayerShot*>& GetShots(void);

	// ボス戦艦(親モデル)と回転と位置の同期
	void SyncParent(Transform& transform, VECTOR addAxis);

private:

	// ボスの基本情報
	const Transform* mTransformParent;

	ResourceManager& mResourceManager;
	GravityManager& mGravityManager;

	// モデル制御の基本情報(ホイール)
	Transform mTransformWheelL;
	Transform mTransformWheelR;

	// モデル制御の基本情報(砲台)
	Transform mTransformStand;

	// モデル制御の基本情報(砲身)
	Transform mTransformGun;

	//砲台のローカル稼働量(deg)
	float mAnglePowStand;

	//砲身のローカル稼働量(deg)
	float mAnglePowGun;

	// 追加のローカル回転
	VECTOR mLocalAddAxis;

	// 更に追加の砲台ローカル回転
	VECTOR mLocalAddAxisStand;

	// 更に追加の砲身ローカル回転
	VECTOR mLocalAddAxisGun;

	// 戦艦からの相対座標
	VECTOR mLocalPos;


	// 状態
	STATE mState;

	//移動スピード
	float mSpeed;

	//移動方向
	VECTOR mMoveDir;

	//移動量
	VECTOR mMovePow;

	//移動後の座標
	VECTOR mMovedPos;

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

	//フレーム間の移動値
	VECTOR mMoveDiff;
	

	//ワープ前の惑星名
	Stage::NAME mPreWarpName;


	//傾斜の方向
	VECTOR mSlopeDir;

	//傾斜角
	float mSlopeAngleDeg;

	//傾斜の移動量
	VECTOR mSlopePow;

	//今、プレイヤーが設置しているポリゴンの法線
	VECTOR mHitNormal;

	//接地点
	VECTOR mHitPos;

	//弾
	std::vector<PlayerShot*> mShots;

	//弾の発射間隔
	float mStepDelayShot;

	// 状態遷移
	void ChangeState(STATE state);

	//衝突判定用の関数
	void Collision(void);
	void CollisionCapsule(void);
	void CollisionGravity(void);

	//自機の弾を発射
	void CreateShot(void);
};

