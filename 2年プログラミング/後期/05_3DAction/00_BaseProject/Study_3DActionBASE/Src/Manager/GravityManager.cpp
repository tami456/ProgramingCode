#include <DxLib.h>
#include "../Utility/AsoUtility.h"
#include "../Manager/SceneManager.h"
#include "../Object/Common/Transform.h"
#include "../Object/Planet.h"
#include "../Object/Player.h"
#include "GravityManager.h"

GravityManager* GravityManager::mInstance = nullptr;

GravityManager::GravityManager(void)
{
	mActivePlanet = nullptr;
	mPlayer = nullptr;
	mPreDirGravity = AsoUtility::DIR_D;
	mDirGravity = AsoUtility::DIR_D;
	mDirUpGravity = AsoUtility::DIR_U;
	mLastHitNormal = AsoUtility::DIR_U;
	mLastHitPos = AsoUtility::VECTOR_ZERO;
	mSlerpPow = 0.0f;
	mStep = 0.0f;

	mState = STATE::IDLE;
}

GravityManager::~GravityManager(void)
{
	delete mInstance;
}

void GravityManager::CreateInstance(void)
{
	if (mInstance == nullptr)
	{
		mInstance = new GravityManager();
	}
	mInstance->Init();
}

GravityManager& GravityManager::GetInstance(void)
{
	return *mInstance;
}

void GravityManager::Init(void)
{

	mDirGravity = AsoUtility::DIR_D;
	mPreDirGravity = AsoUtility::VECTOR_ZERO;
	mStep = -1.0f;
	mSlerpPow = DEFAULT_POW_SLERP;

	mTransform.quaRot = Quaternion();
	mTransform.Update();

}

void GravityManager::Update(void)
{

	switch (mState)
	{
	case GravityManager::STATE::IDLE:
		UpdateIdle();
		break;
	case GravityManager::STATE::STAGE_CHANGE:
		UpdateChangeStage();
		break;
	}

	// 重力方向や、正方向の計算
	Calculate();

}

void GravityManager::UpdateIdle(void)
{
}

void GravityManager::UpdateChangeStage(void)
{
}

void GravityManager::Calculate(void)
{

	//重力方向を変えていく
	mDirGravity = CalcDirGravity();

	//重力の反対方向
	mDirUpGravity = VScale(mDirGravity, -1.0f);

	//2つのベクトル間の回転量を求める
	VECTOR up = mTransform.GetUp();
	Quaternion toRot = Quaternion::FromToRotation(up, mDirUpGravity);

	//到達したい回転
	mTransform.quaRot = Quaternion::Slerp(mTransform.quaRot, toRot.Mult(mTransform.quaRot), mSlerpPow);


}

Planet* GravityManager::GetActivePlanet(void) const
{
	return mActivePlanet;
}

void GravityManager::ChangeActivePlanet(Planet* planet)
{

	// 新しい惑星をセット
	mActivePlanet = planet;

}

void GravityManager::SetPlayer(Player* player)
{
	mPlayer = player;
}

void GravityManager::SetEnemy(Enemy* enemy)
{
	mEnemy = enemy;
}

const Transform& GravityManager::GetTransform(void) const
{
	return mTransform;
}

VECTOR GravityManager::GetDirGravity(void) const
{
	return mDirGravity;
}

VECTOR GravityManager::GetDirUpGravity(void) const
{
	return mDirUpGravity;
}

float GravityManager::GetPower(void) const
{
	float ret = 0.0f;
	if (mActivePlanet != nullptr)
	{
		ret = mActivePlanet->GetGravityPow();
	}
	return ret;
}

Quaternion GravityManager::GetPremQuaRot(void) const
{
	return mPreDirGravity;
}

void GravityManager::SetLastHitNormal(const VECTOR& normal)
{
	mLastHitNormal = normal;
}

void GravityManager::SetLastHitPos(const VECTOR& pos)
{
	mLastHitPos = pos;
}

VECTOR GravityManager::GetLastHitPos(void) const
{
	return mLastHitPos;
}

void GravityManager::ChangeState(STATE state)
{

	mState = state;
	switch (mState)
	{
	case GravityManager::STATE::IDLE:
		break;
	case GravityManager::STATE::STAGE_CHANGE:
		break;
	}

}

VECTOR GravityManager::CalcDirGravity(void) const
{

	VECTOR ret = AsoUtility::DIR_D;

	if (mActivePlanet == nullptr || mPlayer == nullptr)
	{
		return ret;
	}

	// ステージタイプによって、重力方向の計算を変える
	Planet::TYPE stageType = mActivePlanet->GetType();
	switch (stageType)
	{
	case Planet::TYPE::GROUND:
		ret = AsoUtility::DIR_D;
		break;
	case Planet::TYPE::SPHERE:

		VECTOR planetPos = mActivePlanet->GetTransform()->pos;

		ret = VNorm(VSub(planetPos,
			mPlayer->GetTransform()->pos));

		break;

	case Planet::TYPE::TRANS_ROT:
		break;
	case Planet::TYPE::TRANS_CAMERA_FIXED:
		break;
	case Planet::TYPE::ROAD:
		break;
	}

	return ret;

}
