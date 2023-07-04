#include <DxLib.h>
#include "../Utility/AsoUtility.h"
#include "../Manager/SceneManager.h"
#include "../Manager/GravityManager.h"
#include "Common/Transform.h"
#include "WarpStar.h"
#include "Planet.h"

Planet::Planet(Stage::NAME name, TYPE type, Transform transform)
{
	mGravityManager = &GravityManager::GetInstance();
	mName = name;
	mType = type;
	mTransform = transform;

	mGravityPow = 0.0f;
	mGravityRadius = 0.0f;
	mDeadLength = 0.0f;
}

Planet::~Planet(void)
{
}

void Planet::Init(void)
{
	mGravityPow = DEFAULT_GRAVITY_POW;
	mGravityRadius = DEFAULT_GRAVITY_RADIUS;
	mDeadLength = DEFAULT_DEAD_LENGTH;
}

void Planet::Update(void)
{
}

void Planet::Draw(void)
{
    MV1DrawModel(mTransform.modelId);
}

void Planet::Release(void)
{
	MV1DeleteModel(mTransform.modelId);
	mTransform.Release();
}

void Planet::SetPosition(const VECTOR& pos)
{
    mTransform.pos = pos;
    mTransform.Update();
}

void Planet::SetRotation(const Quaternion& rot)
{
	mTransform.quaRot = rot;
	mTransform.Update();
}

float Planet::GetGravityPow(void) const
{
	return mGravityPow;
}

void Planet::SetGravityPow(float pow)
{
	mGravityPow = pow;
}

float Planet::GetGravityRadius(void) const
{
	return mGravityRadius;
}

void Planet::SetGravityRadius(float radius)
{
	mGravityRadius = radius;
}

Planet::TYPE Planet::GetType(void) const
{
	return mType;
}

Transform* Planet::GetTransform(void)
{
	return &mTransform;
}

bool IsHitSpheres(const VECTOR& pos1, float radius1, const VECTOR& pos2)
{
	//ãÖëÃìØémÇÃè’ìÀîªíË
	bool hit = false;
	
	// ç¿ïWÇÃç∑Ç©ÇÁÇ®å›Ç¢ÇÃãóó£ÇéÊÇÈ
	VECTOR diff = VSub(pos1, pos2);

	//éOïΩï˚ÇÃíËóùÇ∆î‰är
	float dis = (diff.x * diff.x) + (diff.y * diff.y) + (diff.z * diff.z);

	if (dis < (radius1 * radius1))
	{
		hit = true;
	}

	return hit;
}

bool Planet::InRangeGravity(const VECTOR& pos) const
{
	//èdóÕîÕàÕ(îºåa)
	if (IsHitSpheres(mTransform.pos, mGravityRadius, pos))
	{
		return true;
	}
	return false;
}

bool Planet::InRangeDead(const VECTOR& pos) const
{
	//èdóÕîÕàÕ(îºåa)
	if (IsHitSpheres(mTransform.pos, 100.0f, pos))
	{
		return true;
	}
	return false;
}

void Planet::SetDeadLength(float len)
{
	
	mDeadLength = len;
}

Stage::NAME Planet::GetName(void) const
{
	return mName;
}


