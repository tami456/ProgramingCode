#pragma once
#include <DxLib.h>
#include "Common/Transform.h"
#include "Stage.h"
class ResourceManager;
class GravityManager;
class AnimationController;
class Collider;
class Capsule;
class PlayerShot;
class SpeechBalloon;


class Player
{
public:

	//移動スピード
	static constexpr float SPEED_MOVE = 5.0f;
	static constexpr float SPEED_RUN = 10.0f;
	static constexpr float SPEED_FLY = 30.0f;

	//弾の生存時間
	static constexpr float ALIVE_SHOT = 10.0f;

	//回転完了までの時間
	static constexpr float TIME_ROT = 1.0f;

	//ジャンプ力
	static constexpr float POW_JUMP = 35.0f;

	//ジャンプ受付時間
	static constexpr float TIME_JUMP_IN = 0.5f;

	//煙エフェクトの発生間隔
	static constexpr float TIME_FOOT_SMOKE = 0.3f;

	//軌跡エフェクトの回転
	static constexpr float TIME_ORBIT = 0.3f;

	//弾の発射間隔
	static constexpr float TIME_DELAY_SHOT = 0.2f;

	// 状態
	enum class STATE
	{
		NONE,
		PLAY,
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
		KICK
	};

	Player(void);
	~Player(void);

	void Init(void);
	void InitAnimation(void);
	void Update(void);
	void UpdatePlay(void);
	void UpdateWarpReserve(void);
	void UpdateWarpMove(void);
	void UpdateDead(void);

	void Draw(void);
	void DrawShadow(void);
	void DrawDebug(void);

	void Release(void);

	Transform* GetTransform(void);
	Capsule* GetCapsule(void);

	//操作
	void ProcessMove(void);
	void ProcessJump(void);
	void ProcessShot(void);

	//回転(振り向きたい角度を決める)
	void SetGoalRotate(float rotRad);

	//回転(ゴールに対して、球面補完を行う)
	void Rotate(void);

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

	//ワープの開始
	void StartWarp(
		float time, 
		const Quaternion& goalRot,
		const VECTOR& goalPos);

	//軌跡エフェクトの停止
	void StopOrbitEffect(void);

	//プレイヤーの状態判断
	bool IsPlay(void);
	bool IsWarpMove(void);
	bool IsDead(void);

	//弾の取得
	std::vector<PlayerShot*>& GetShots(void);

	//吹き出しクラスの取得
	SpeechBalloon* GetSpeechBalloon(void) const;



private:

	ResourceManager& mResourceManager;
	GravityManager& mGravityManager;

	Transform mTransform;

	std::string msg;

	// アニメーション
	AnimationController* mAnimationController;

	//スピーチバルーン
	SpeechBalloon* mSpeechBalloon;

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

	//ジャンプ判定
	bool mIsJump;

	//ジャンプの入力受付時間
	float mStepJump;

	//衝突判定に用いられるコライダ
	Capsule* mCapsule;
	std::vector<Collider*> mColliders;

	//設置衝突で使用する線分座標
	VECTOR mGravHitDown;
	VECTOR mGravHitUp;

	//丸影
	int mImgShadow;

	//足煙のエフェクト
	int mEffectSmoke;
	float mStepFootSmoke;
	int mPlayEffectSmoke;


	//フレーム間の移動値
	VECTOR mMoveDiff;
	
	//ワープ用
	float mTimeWarp;
	float mStepWarp;

	//ワープ準備完了時の情報
	Quaternion mWarpQua;
	VECTOR mWarpReservePos;

	//ワープ準備完了時のプレイヤー情報
	Quaternion mReserveStartQua;
	VECTOR mReserveStartPos;

	//ワープ前の惑星名
	Stage::NAME mPreWarpName;

	//軌跡エフェクト
	int mEffectWarpOrbit;

	int mPlayOrbitEffectL;
	int mPlayOrbitEffectR;

	//プレイヤーの手(フレーム番号)
	int mFrameLeftHand;
	int mFrameRightHand;

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

	//着地モーションの終了判定
	bool IsEndLanding(void);

	//足煙エフェクト
	void EffectFootSmoke(void);

	//軌跡エフェクト
	void EffectWarpOrbit(void);
	void SyncWarpOrbitPos(void);

	//自機の弾を発射
	void CreateShot(void);
};

