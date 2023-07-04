#pragma once
#include <DxLib.h>
#include "../Object/Common/Transform.h"
class ParticleGenerator;
class SpriteAnimator;
class SpeechBalloon;
class PlayerShot;

class PlayerShip
{
public:

	//状態
	enum class STATE
	{
		NONE,
		RUN,		//通常(走行)状態
		DESTROY		//破壊状態
	};


	static constexpr float SPEED_MOVE = 10.0f;

	//自機の回転量
	static constexpr float SPEED_ROT_Y = DX_PI_F/180.0f * 1.0f;
	static constexpr float SPEED_ROT_X = DX_PI_F/180.0f * 1.0f;

	//衝突判定用の球体半径
	static constexpr float COLLISION_RADIUS = 20.0f;

	//衝突判定用の球体半径(弾)
	static constexpr float SHOT_COLLISION_RADIUS = 10.0f;

	//ブーストの間隔
	static constexpr float TIME_DELAY_BOOST = 3.0f;

	//ブースト時の加速スピード
	static constexpr float SPEED_BOOST = 6.0f;

	//ブーストの減速
	static constexpr float SUB_SPEED_BOOST = 2.0f;
	
	//弾の発射間隔
	static constexpr float TIME_DELAY_SHOT = 0.2f;

	// コンストラクタ
	PlayerShip(void);

	// デストラクタ
	~PlayerShip(void);

	void Init(void);
	void InitEffect(void);
	void Update(void);
	void UpdateRun(void);
	void UpdateDestroy(void);
	void Draw(void);
	void Release(void);

	//操作
	void ProcessTurn(void);
	void ProcessBoost(void);
	void ProcessShot(void);

	void Turn(double deg,VECTOR axis);


	const Transform& GetTransform(void) const;

	//自機破壊
	void Destroy(void);

	//自機が破壊されているか確認
	bool IsDestroy(void);

	//吹き出しクラスの取得
	SpeechBalloon* GetSpeechBalloon(void) const;
 
	//弾の取得
	std::vector<PlayerShot*>& GetShots(void);
private:

	//状態
	STATE mState;

	//パーティクルエフェクト
	ParticleGenerator* mParticleGenerator;

	//モデルの3D制御
	Transform mTransform;

	//爆発エフェクト
	SpriteAnimator* mExplosion;

	//吹き出し
	SpeechBalloon* mSpeechBalloon;


	//弾
	std::vector<PlayerShot*> mShots;

	//弾の発射間隔
	float mStepDelayShot;

	//爆発エフェクト(Effekseer)
	int mExplosion3D;
	int mExplosion3DPlay;

	//噴射エフェクト
	int mEffectJet;
	int mEffectJetPlayL;
	int mEffectJetPlayR;

	//ブーストエフェクト
	float mStepDelayBoost;
	float mSpeedBoost;
	int mEffectBoost;
	int mEffectBoostPlay;

	void ChangeState(STATE state);

	//エフェクト制御
	void SyncJetEffect(void);

	//ブーストエフェクト制御
	void SyncBoostEffect(void);

	//自機の弾を発射
	void CreateShot(void);
};

