#include "Camera.h"
#include "../Actor/Actor.h"
Camera::Camera()
{
	//Init();
}

Camera::~Camera()
{
}

bool Camera::Init(const Vector2D& worldSize)
{
	Vector2D winSize;
	GetWindowSize(&winSize.x, &winSize.y);
	moveArea_ = { winSize.x / 2,winSize.y / 2,worldSize.x - winSize.x / 2,worldSize.y - winSize.y / 2 };


	pos_.x = moveArea_.left;
	pos_.y = moveArea_.top;
	return true;
}

void Camera::Update(void)
{
	if (target_.expired())
	{
		return;		//終了していたら抜ける
	}
	auto targetPos = target_.lock()->GetPos();
	if (targetPos.x >= moveArea_.left && targetPos.x <= moveArea_.right)
	{
		pos_.x = targetPos.x;
	}
	if (targetPos.y > moveArea_.top && targetPos.y < moveArea_.bottom)
	{
		pos_.y = targetPos.y;
	}
}

bool Camera::ReConnect(std::weak_ptr<Actor> actor)		//ターゲットのposを取得
{
	target_ = actor;
	return false;
}
