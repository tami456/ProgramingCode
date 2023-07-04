#include "../Application.h"
#include "../Utility/AsoUtility.h"
#include "../Common/Quaternion.h"
#include "../Manager/SceneManager.h"
#include "../Manager/ResourceManager.h"
#include "../Manager/Resource.h"
#include "Common/Transform.h"
#include "Common/AnimationController.h"
#include "PlayerShot.h"

PlayerShot::PlayerShot(const Transform* parent)
{
    mIsAlive = false;

    mCollisionRadius = 10.0f;

	// モデル制御の基本情報
	ResourceManager& rem = ResourceManager::GetInstance();
	mTransform.modelId = rem.LoadModelDuplicate(ResourceManager::SRC::SHOT);
	float scale = 0.2f;
	mTransform.scl = { scale, scale, scale };
	mTransform.pos = parent->pos;
	mTransform.quaRot = Quaternion();
	mTransform.quaRotLocal = Quaternion::Euler(AsoUtility::Deg2RadF(90.0f), 0.0f, 0.0f);
	mTransform.Update();

	mStepAlive = 0.0f;
	mDir = { 0.0f, 0.0f, 0.0f };
}

PlayerShot::~PlayerShot()
{
}

void PlayerShot::Init(void)
{
}

void PlayerShot::Create(VECTOR birthPos, VECTOR dir)
{
	mDir = dir;

	mTransform.pos = birthPos;
	mTransform.quaRot = Quaternion::LookRotation(mDir);

	//生存系初期化
	mIsAlive = true;
	mStepAlive = GetTimeAlive();
}

void PlayerShot::Update(void)
{

	// 生存チェック＆生存判定
	if (!CheckAlive())
	{
		return;
	}

	// 移動処理
	Move();

	// モデル制御の基本情報更新
	mTransform.Update();
}

bool PlayerShot::CheckAlive(void)
{
	// 生存時間
	mStepAlive -= SceneManager::GetInstance().GetDeltaTime();
	if (mStepAlive < 0.0f)
	{
		mIsAlive = false;
	}

	return mIsAlive;
}

void PlayerShot::Move(void)
{
	// 移動
	VECTOR velocity = VScale(mDir, GetSpeed());
	mTransform.pos = VAdd(mTransform.pos, velocity);
}

void PlayerShot::Draw(void)
{
	if (mIsAlive)
	{
		MV1DrawModel(mTransform.modelId);
	}
}

void PlayerShot::Release(void)
{
}

void PlayerShot::NonAlive(void)
{
	mIsAlive = false;
}

float PlayerShot::GetSpeed(void) const
{
	return DEFAULT_SPEED;
}

float PlayerShot::GetTimeAlive(void) const
{
    return DEFAULT_TIME_ALIVE;
}

bool PlayerShot::IsAlive(void) const
{
    return mIsAlive;
}

Transform& PlayerShot::GetTransform(void)
{
    return mTransform;
}

float PlayerShot::GetCollisionRadius(void) const
{
    return mCollisionRadius;
}
