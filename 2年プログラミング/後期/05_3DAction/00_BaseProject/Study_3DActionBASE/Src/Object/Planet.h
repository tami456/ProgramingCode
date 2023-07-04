#pragma once
#include "../Common//Quaternion.h"
#include "Stage.h"
class GravityManager;
class WarpStar;

class Planet
{

public:

	// 重力の強さ
	static constexpr float DEFAULT_GRAVITY_POW = 25.0f;

	// 重力がかかる範囲
	static constexpr float DEFAULT_GRAVITY_RADIUS = 5000.0f;

	// ゲームオーバー範囲
	static constexpr float DEFAULT_DEAD_LENGTH = 1000.0f;

	enum class TYPE
	{
		GROUND,
		SPHERE,
		TRANS_ROT,
		TRANS_CAMERA_FIXED,
		ROAD,
	};

	Planet(Stage::NAME name, TYPE type, Transform transform);
	~Planet(void);

	void Init(void);
	void Update(void);
	void Draw(void);
	void Release(void);
	
	// 惑星種別
	TYPE GetType(void) const;

	// 惑星名
	Stage::NAME GetName(void) const;

	// 位置の設定
	void SetPosition(const VECTOR& pos);

	// 回転の設定
	void SetRotation(const Quaternion& rot);

	// 重力の強さの取得・設定
	float GetGravityPow(void) const;
	void SetGravityPow(float pow);
	
	// 重力範囲の取得・設定
	float GetGravityRadius(void) const;
	void SetGravityRadius(float radius);

	Transform* GetTransform(void);

	// 重力範囲内
	bool InRangeGravity(const VECTOR& pos) const;

	// ゲームオーバー範囲内
	bool InRangeDead(const VECTOR& pos) const;

	// ゲームオーバー範囲
	void SetDeadLength(float len);

private:

	GravityManager* mGravityManager;

	Transform mTransform;

	// 惑星種別
	TYPE mType;

	// 惑星名
	Stage::NAME mName;

	// 重力の強さ
	float mGravityPow;

	// 重力範囲
	float mGravityRadius;

	// ゲームーオーバー距離
	float mDeadLength;

};
