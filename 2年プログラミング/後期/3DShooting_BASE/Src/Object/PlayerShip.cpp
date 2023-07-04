#include <string>
#include <EffekseerForDxLib.h>
#include "../Manager/ResourceManager.h"
#include "../Manager/Resource.h"
#include "../Manager/SceneManager.h"
#include "../Utility/AsoUtility.h"
#include "ParticleGenerator.h"
#include "./Common/SpriteAnimator.h"
#include "./Common/SpeechBalloon.h"
#include "./Ship/PlayerShot.h"
#include "PlayerShip.h"


PlayerShip::PlayerShip(void)
{
}

PlayerShip::~PlayerShip(void)
{
}

void PlayerShip::Init(void)
{
	//モデル制御の基本情報
	mTransform.SetModel(
		ResourceManager::GetInstance().
		LoadModelDuplicate(ResourceManager::SRC::PLAYER_SHIP)
	);
	mTransform.scl = { 10.0f,10.0f,10.0f };
	mTransform.quaRot = Quaternion();
	mTransform.quaRotLocal = Quaternion();
	mTransform.pos = { 0.0f,0.0f,0.0f };
	//変更したらUpdate
	mTransform.Update();

	//パーティクルエフェクト
	mParticleGenerator = new ParticleGenerator(mTransform.pos,10.0f);
	mParticleGenerator->Init();

	//エフェクトの初期化
	InitEffect();

	//吹き出し
	std::string msg = "追って！";
	//シーンによって状態を変える
	SceneManager::SCENE_ID sceneId = SceneManager::GetInstance().GetmSceneID();

	if (sceneId == SceneManager::SCENE_ID::EVENT)
	{
		msg = "倒して！";
	}
	mSpeechBalloon = new SpeechBalloon(
	SpeechBalloon::TYPE::SPEECH,&mTransform);

	mSpeechBalloon->SetText(msg.c_str());
	mSpeechBalloon->SetTime(15.0f);
	mSpeechBalloon->SetRelativePos({15.0f,25.0f,0.0f});

	//初期状態を走行状態にする
	ChangeState(STATE::RUN);

	//ブースト関連
	mStepDelayBoost = 0.0f;
	mSpeedBoost = 0.0f;
	mEffectBoostPlay = -1;
}

void PlayerShip::InitEffect(void)
{
	mExplosion = new SpriteAnimator(
		ResourceManager::SRC::SHIP_EXPLOSION,
		20.0f, 8.0f);

	mExplosion3D = ResourceManager::GetInstance().Load(
		ResourceManager::SRC::EXPLOSION).mHandleId;
	mExplosion3DPlay = -1;

	//噴射エフェクト
	mEffectJet = ResourceManager::GetInstance().Load(ResourceManager::SRC::JET).mHandleId;
	//Effekseerのエフェクト生成
	mEffectJetPlayL = PlayEffekseer3DEffect(mEffectJet);
	mEffectJetPlayR = PlayEffekseer3DEffect(mEffectJet);

	float SCALE = 5.0f;
	SetScalePlayingEffekseer3DEffect(mEffectJetPlayL, SCALE, SCALE, SCALE);
	SetScalePlayingEffekseer3DEffect(mEffectJetPlayR, SCALE, SCALE, SCALE);

	//ブーストエフェクト
	mEffectBoost = ResourceManager::GetInstance().Load(ResourceManager::SRC::BOOST).mHandleId;
	mEffectBoostPlay = PlayEffekseer3DEffect(mEffectBoost);

	float SCALEB = 10.0f;
	SetScalePlayingEffekseer3DEffect(mEffectBoostPlay, SCALEB, SCALEB, SCALEB);

	//エフェクト制御
	SyncJetEffect();
	SyncBoostEffect();
}

void PlayerShip::Update(void)
{
	switch (mState)
	{
	case PlayerShip::STATE::RUN:
		UpdateRun();
		break;
	case PlayerShip::STATE::DESTROY:
		UpdateDestroy();
		break;
	}
}

void PlayerShip::UpdateRun(void)
{
	//回転操作
	ProcessTurn();

	//加速処理
	ProcessBoost();

	//弾の処理
	ProcessShot();

	VECTOR forward = mTransform.GetForward();
	mTransform.pos = VAdd(mTransform.pos, VScale(forward, SPEED_MOVE + mSpeedBoost));
	mTransform.Update();

	//プレイヤーの位置とエフェクトの位置を同期

	//プレイヤーの位置の前方方向に円を出す
	VECTOR forwardPos = VAdd(mTransform.pos, VScale(mTransform.GetForward(), 18.0f));
	mParticleGenerator->SetPos(forwardPos);

	//プレイヤーの後方向を向くように
	Quaternion pBackRot = mTransform.quaRot;
	pBackRot = pBackRot.Mult(Quaternion::AngleAxis(AsoUtility::Deg2RadD(-90), AsoUtility::AXIS_X));
	mParticleGenerator->SetRot(pBackRot);
	mParticleGenerator->Update();

	SyncJetEffect();

	if (CheckHitKey(KEY_INPUT_B))
	{
		SyncBoostEffect();
	}
}
	

void PlayerShip::UpdateDestroy(void)
{
	//A:自機が破壊されたらカメラを止める
	
	//B:爆発エフェクトを自機の手前に描画するようにする
	VECTOR forwardPos = VAdd(mTransform.pos, VScale(mTransform.GetForward(), 150.0f));

	//爆発エフェクト
	mExplosion->SetPos(forwardPos);
	mExplosion->Update();
}

void PlayerShip::Draw(void)
{
	switch (mState)
	{
	case PlayerShip::STATE::RUN:
		//自機
		MV1DrawModel(mTransform.modelId);
		//パーティクル
		mParticleGenerator->Draw();
		break;
	case PlayerShip::STATE::DESTROY:
		//爆発エフェクト
		mExplosion->Draw();
		break;
	}
	
	//自機の当たり判定
	//DrawSphere3D(mTransform.pos, COLLISION_RADIUS, 5, 0xffffff, 0xffffff,false);

}

void PlayerShip::Release(void)
{
	mParticleGenerator->Release();
	delete mParticleGenerator;

	mExplosion->Release();
	delete mExplosion;

	if (mExplosion3DPlay != -1)
	{
		StopEffekseer3DEffect(mExplosion3DPlay);
	}

	StopEffekseer3DEffect(mEffectJetPlayL);
	StopEffekseer3DEffect(mEffectJetPlayR);

	StopEffekseer3DEffect(mEffectBoostPlay);

	mSpeechBalloon->Release();
	delete mSpeechBalloon;

	//弾の開放
	for (auto s : mShots)
	{
		s->Release();
		delete s;
	}
	mShots.clear();
}

void PlayerShip::ProcessTurn(void)
{
	//右旋回
	if (CheckHitKey(KEY_INPUT_RIGHT))
	{
		Turn(SPEED_ROT_Y,AsoUtility::AXIS_Y);
	}

	//左旋回
	if (CheckHitKey(KEY_INPUT_LEFT))
	{
		Turn(-SPEED_ROT_Y, AsoUtility::AXIS_Y);
	}

	//上旋回
	if (CheckHitKey(KEY_INPUT_UP))
	{
		Turn(-SPEED_ROT_X, AsoUtility::AXIS_X);
	}

	//下旋回
	if (CheckHitKey(KEY_INPUT_DOWN))
	{
		Turn(SPEED_ROT_X, AsoUtility::AXIS_X);
	}

}

void PlayerShip::ProcessBoost(void)
{
	mStepDelayBoost += SceneManager::GetInstance().GetDeltaTime();
	if (CheckHitKey(KEY_INPUT_B) && mStepDelayBoost > TIME_DELAY_BOOST)
	{
		//ディレイ(クールタイム)制御
		mStepDelayBoost = 0.0f;

		//加速する
		mSpeedBoost = SPEED_BOOST;

		//エフェクト再生
		mEffectBoostPlay = PlayEffekseer3DEffect(mEffectBoost);

		//大きさ
		float SCALE = 10.0f;
		SetScalePlayingEffekseer3DEffect(mEffectBoostPlay, SCALE, SCALE, SCALE);

		//エフェクト制御
		SyncBoostEffect();

	}
	if (mStepDelayBoost < TIME_DELAY_BOOST && mEffectBoostPlay > 0)
	{
		//エフェクト制御
		SyncBoostEffect();
	}
	//mStepDelayBoost 0.0 = エフェクトしたらダメ	= エフェクト中		= 同期させたい
	//mStepDelayBoost 3.0 = エフェクトしたらダメ	= エフェクト中		= 同期させたい
	//mStepDelayBoost 3.1 = エフェクトしてよい		= エフェクトが終了	= 同期意味ない


	//減速処理
	if (mSpeedBoost > 0.0f)
	{
		//徐々に減速
		mSpeedBoost -= (SUB_SPEED_BOOST * SceneManager::GetInstance().GetDeltaTime());
		if (mSpeedBoost < 0.0f)
		{
			//通常走行が減速されないようゼロにしておく
			mSpeedBoost = 0.0f;
		}

		SyncBoostEffect();

	}

}

void PlayerShip::ProcessShot(void)
{
	//弾の生成ディレイ
	mStepDelayShot -= SceneManager::GetInstance().GetDeltaTime();
	if (mStepDelayShot < 0.0f)
	{
		mStepDelayShot = 0.0f;
	}

	//キーチェック
	if (CheckHitKey(KEY_INPUT_N) && mStepDelayShot <= 0.0f)
	{
		//ディレイ時間をセット
		mStepDelayShot = TIME_DELAY_SHOT;

		//弾を生成
		CreateShot();
	}
}

void PlayerShip::Turn(double deg, VECTOR axis)
{

	////デグリー角をラジアン角に変換
	//float rad = AsoUtility::Deg2RadF(deg);

	////指定軸(axis)に対する回転量(rad)を加えた回転(Quaternion)を作る
	//Quaternion axisQ = Quaternion::AngleAxis(rad, axis);

	////自機の回転に、↑で作成した回転を加える(合成する)
	//mTransform.quaRot = mTransform.quaRot.Mult(axisQ);

	mTransform.quaRot = mTransform.quaRot.Mult(VScale(axis,deg));
}

const Transform& PlayerShip::GetTransform(void) const
{
	return mTransform;
}

void PlayerShip::Destroy(void)
{

	ChangeState(STATE::DESTROY);

}

bool PlayerShip::IsDestroy(void)
{
	return mState == STATE::DESTROY;
}

SpeechBalloon* PlayerShip::GetSpeechBalloon(void) const
{
	return mSpeechBalloon;
}

std::vector<PlayerShot*>& PlayerShip::GetShots(void)
{
	return mShots;
}

void PlayerShip::ChangeState(STATE state)
{
	//状態変化(遷移)時の初期処理
	mState = state;
	switch (mState)
	{
	case PlayerShip::STATE::RUN:
		break;
	case PlayerShip::STATE::DESTROY:
	{
		//爆発エフェクトの生成(開始)
		//mExplosion->Create(mTransform.pos, nullptr);
		//Effekseerのエフェクト生成
		mExplosion3DPlay = PlayEffekseer3DEffect(mExplosion3D);
		VECTOR pos = mTransform.pos;
		pos = VAdd(pos, VScale(mTransform.GetForward(), 100.0f));
		//位置
		SetPosPlayingEffekseer3DEffect(mExplosion3DPlay, pos.x, pos.y, pos.z);
		//大きさ
		SetScalePlayingEffekseer3DEffect(mExplosion3DPlay, 10.0f,10.0f,10.0f);
		//回転
		SetRotationPlayingEffekseer3DEffect(mExplosion3DPlay, 0.0f,0.0f,0.0f);
	}
		break;
	}
}

void PlayerShip::SyncJetEffect(void)
{

	VECTOR pos;

	const VECTOR angles = Quaternion::ToEuler(mTransform.quaRot);
	
#pragma region 左噴射エフェクト
	//相対座標を回転させる
	VECTOR localLPos = { -5.0f,0.0f,-15.0f };
	//相対座標を回転させる
	localLPos = mTransform.quaRot.PosAxis(localLPos);
	//相対座標からワールド座標に変換する
	pos = VAdd(mTransform.pos, localLPos);
	//エフェクトの位置設定
	SetPosPlayingEffekseer3DEffect(mEffectJetPlayL, pos.x, pos.y, pos.z);
#pragma endregion


#pragma region 右噴射エフェクト

	//相対座標を回転させる
	VECTOR localRPos = { 5.0f,0.0f,-15.0f };
	//相対座標を回転させる
	localRPos = mTransform.quaRot.PosAxis(localRPos);
	//相対座標からワールド座標に変換する
	pos = VAdd(mTransform.pos, localRPos);
	//エフェクトの位置設定
	SetPosPlayingEffekseer3DEffect(mEffectJetPlayR, pos.x, pos.y, pos.z);
#pragma endregion

	//回転
	SetRotationPlayingEffekseer3DEffect(mEffectJetPlayL, angles.x, angles.y, angles.z);
	SetRotationPlayingEffekseer3DEffect(mEffectJetPlayR, angles.x, angles.y, angles.z);

}

void PlayerShip::SyncBoostEffect(void)
{

#pragma region 回転同期
	//クォータニオンからオイラー角に変換
	const VECTOR angles = Quaternion::ToEuler(mTransform.quaRot);

	//回転
	SetRotationPlayingEffekseer3DEffect(mEffectBoostPlay, angles.x, angles.y, angles.z);
#pragma endregion

#pragma region 位置同期
	VECTOR pos;

	//相対座標を回転させる
	VECTOR localBPos = { 0.0f,0.0f,100.0f };
	//相対座標を回転させる
	localBPos = mTransform.quaRot.PosAxis(localBPos);
	//相対座標からワールド座標に変換する
	pos = VAdd(mTransform.pos, localBPos);
	//エフェクトの位置設定
	SetPosPlayingEffekseer3DEffect(mEffectBoostPlay, pos.x, pos.y, pos.z);
#pragma endregion

}

void PlayerShip::CreateShot(void)
{
	bool isCreate = false;
	for (auto s : mShots)
	{
		if (!s->IsAlive())
		{
			//生存していなかったら
			//インスタンスを使いまわす
			s->Create(mTransform.pos,mTransform.GetForward());
			isCreate = true;
		}
	}

	if (!isCreate)
	{
		//インスタンスを新しく作る
		auto newShot = new PlayerShot(&mTransform);
		newShot->Create(mTransform.pos, mTransform.GetForward());

		//可変長配列に追加
		mShots.emplace_back(newShot);
	}

}

//同じ
	//const VECTOR angles = mTransform.quaRot.ToEuler();
	//方向から座標を作る
	//VECTOR localPos = VScale(mTransform.GetBack(), 15.0f);
	//pos = VAdd(mTransform.pos, localPos);
	//VECTOR localLeftPos = VScale(mTransform.GetBack(), 5.0f);
	//localLeftPos = VAdd(localLeftPos,VScale(mTransform.GetLeft(), 5.0f));
	//pos = VAdd(mTransform.pos, localLeftPos);
	////位置
	//SetPosPlayingEffekseer3DEffect(mEffectJetPlayL, -pos.x, pos.y, pos.z);
	//SetPosPlayingEffekseer3DEffect(mEffectJetPlayR, pos.x, pos.y, pos.z);