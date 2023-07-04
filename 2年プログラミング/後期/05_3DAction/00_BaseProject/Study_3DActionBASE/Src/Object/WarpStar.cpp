#include <DxLib.h>
#include <EffekseerForDxLib.h>
#include "../Utility/AsoUtility.h"
#include "../Manager/SceneManager.h"
#include "../Manager/ResourceManager.h"
#include "Common/Transform.h"
#include "Player.h"
#include "Common/Capsule.h"
#include "WarpStar.h"

WarpStar::WarpStar(Player* player, Transform transform)
{
	mPlayer = player;
	mTransform = transform;

	mState = STATE::NONE;
}

void WarpStar::Init(void)
{

	// モデルの基本情報
	mTransform.SetModel(
		ResourceManager::GetInstance().LoadModelDuplicate(
			ResourceManager::SRC::WARP_STAR)
	);

	mTransform.Update();

	//Zの無回転の状態を保持しておく
	VECTOR angle = mTransform.quaRot.ToEuler();
	mWarpQua = Quaternion::Euler(angle.x, angle.y, 0.0f);

	//ワープ準備用の移動座標
	mWarpReservePos = 
		VAdd(mTransform.pos,mWarpQua.
			PosAxis(WARP_RELATIVE_POS));
	
	//エフェクト
	mEffectRotParticle = ResourceManager::GetInstance().Load(
		ResourceManager::SRC::WARP_STAR_ROT_EFF).mHandleId;

	mStepEffect = TERM_EFFECT;

	ChangeState(STATE::IDLE);

	
}

void WarpStar::Update(void)
{

	switch (mState)
	{
	case WarpStar::STATE::IDLE:
		UpdateIdle();
		break;
	case WarpStar::STATE::RESERVE:
		UpdateReserve();
		break;
	case WarpStar::STATE::MOVE:
		UpdateMove();
		break;
	}

}

//自作関数練習用
bool IsHitSpheres(const VECTOR& pos1, float radius1, const VECTOR& pos2, float radius2)
{
	// 球体同士の衝突判定
	bool hit = false;

	// お互いの半径の合計
	float radius = radius1 + radius2;

	// 座標の差からお互いの距離を取る
	VECTOR diff = VSub(pos1, pos2);

	// 三平方の定理で比較(SqrMagnitudeと同じ) a^2 + b^2 = c^2
	float distance = (diff.x * diff.x) + (diff.y * diff.y) + (diff.z * diff.z);

	if (distance < (radius * radius))
	{
		hit = true;
	}

	return hit;
}

void WarpStar::UpdateIdle(void)
{

	//Z回転
	RotateZ(SPEED_ROT_IDLE);

	//エフェクトの発生
	PlayEffectRotParticle();

	//プレイヤー(点)とスター(球体)の球体の当たり判定
	
	//スターの座標
	VECTOR sPos = mTransform.pos;

	//プレイヤーの座標
	VECTOR pPos = mPlayer->GetCapsule()->GetCenter();
	float radius = mPlayer->GetCapsule()->GetRadius();

	//球体の半径
	//RADIUS

	if (IsHitSpheres(sPos, RADIUS, pPos, radius))
	{
		ChangeState(STATE::RESERVE);
	}

}

void WarpStar::UpdateReserve(void)
{

	//Z回転
	RotateZ(SPEED_ROT_RESERVE);

	//エフェクトの発生
	PlayEffectRotParticle();

	if (mPlayer->IsWarpMove())
	{
		ChangeState(STATE::MOVE);
	}

}

void WarpStar::UpdateMove(void)
{
	if(mPlayer->IsPlay())
	{
		ChangeState(STATE::IDLE);
	}
}

void WarpStar::Draw(void)
{
	MV1DrawModel(mTransform.modelId);
}

void WarpStar::Release(void)
{
}

Transform* WarpStar::GetTransform(void)
{
	return &mTransform;
}

void WarpStar::ChangeState(STATE state)
{
	//状態変更
	mState = state;

	//状態遷移をするときの初期化処理

	mState = state;
	switch (mState)
	{
	case WarpStar::STATE::IDLE:
		break;
	case WarpStar::STATE::RESERVE:
		//プレイヤーをワープ準備段階に移行させる
		mPlayer->StartWarp(
			TIME_WARP_RESERVE, mWarpQua, mWarpReservePos);
		break;
	case WarpStar::STATE::MOVE:
		break;
	}

}

void WarpStar::RotateZ(float speed)
{
	//星のモデルをZ回転させる
	
	mTransform.quaRot = 
		mTransform.quaRot.Mult(VScale(AsoUtility::AXIS_Z,AsoUtility::Deg2RadF(speed)));
	//speed：度(デグリー)

	mTransform.Update();

}

void WarpStar::PlayEffectRotParticle(void)
{

	mStepEffect -= SceneManager::GetInstance().GetDeltaTime();

	if (mStepEffect < 0.0f)
	{
		mStepEffect = TERM_EFFECT;

		int mPlayEffectRotParticle;

		mPlayEffectRotParticle = PlayEffekseer3DEffect(mEffectRotParticle);

		float Scale = 5.0f;

		SetScalePlayingEffekseer3DEffect(mPlayEffectRotParticle, Scale, Scale, Scale);
		VECTOR StarEffect;
		StarEffect =
			VAdd(mTransform.pos, mTransform.quaRot.
				PosAxis(EFFECT_RELATIVE_POS));

		SetPosPlayingEffekseer3DEffect(
			mPlayEffectRotParticle,
			StarEffect.x, StarEffect.y, StarEffect.z
		);
	}
}
