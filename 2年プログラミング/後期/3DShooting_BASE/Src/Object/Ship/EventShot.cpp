#include "../../Manager/SceneManager.h"
#include "../../Manager/Camera.h"
#include "../Common/SpriteAnimator.h"
#include "EventShot.h"


EventShot::EventShot(const Transform* parent):ShotBase(parent)
{
}

EventShot::~EventShot(void)
{
}

void EventShot::Update(void)
{

	// 着弾時のエフェクト
	mExplosion->Update();

	// 生存チェック＆生存判定
	if (!IsAlive())
	{
		return;
	}

	// 移動処理
	Move();

	//カメラの視野前チェック
	VECTOR pos = ConvWorldPosToScreenPos(mTransform.pos);
	

	if (!(pos.z > 0.0f && pos.z < 1.0f))
	//if (CheckCameraViewClip(mTransform.pos))
	{
		//1:(0,100,-498)
		//2:(0,90,-19)
		//カメラの視野外に到達したら

		//カメラに映るように位置を進行方向の後ろに移動させる
		VECTOR revDir = VScale(mDir, -1.0f);
		VECTOR velocity = VScale(revDir, 100.0f);
		VECTOR newPos = VAdd(mTransform.pos, velocity);
		mTransform.pos = newPos;
		mTransform.Update();

		//爆発エフェクトの開始
		CreateExplosion(nullptr);

		//カメラを揺らす
		SceneManager::GetInstance().GetCamera()->ChangeMode(
			Camera::MODE::SHAKE
		);
	}

	// モデル制御の基本情報更新
	mTransform.Update();

}

float EventShot::GetSpeed(void) const
{
    return 20.0f;
}

float EventShot::GetTimeAlive(void) const
{
    return 10.0f;
}
