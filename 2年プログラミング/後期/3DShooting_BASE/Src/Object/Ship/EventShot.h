#pragma once
#include "ShotBase.h" 
class Transform;
class EventShot : public ShotBase
{
public:
	//コンストラクタ
	EventShot(const Transform* parent);

	//デストラクタ
	~EventShot(void);

	//更新
	void Update(void) override;

	//弾の速度
	float GetSpeed(void) const override;

	// 生存時間
	float GetTimeAlive(void) const override;


};

