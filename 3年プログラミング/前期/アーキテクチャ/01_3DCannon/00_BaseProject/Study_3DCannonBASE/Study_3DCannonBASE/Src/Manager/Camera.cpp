#include "../Application.h"
#include "../Utility/AsoUtility.h"
#include "SceneManager.h"
#include "InputManager.h"
#include "Camera.h"

Camera::Camera(void)
{
}

Camera::~Camera()
{
}

void Camera::Init(void)
{
	//カメラの位置
	pos_ = { 0.0f,400.0f,-400.0f };

	dx = 0.0f;

	stepShotDelay = 0.0f;

	//カメラの角度
	angles_ = { 40.0f * DX_PI_F / 180.0f,0.0f,0.0f };

	//定点カメラを初期状態にする
	ChangeMode(MODE::FIXED_POINT);
}

void Camera::Update(void)
{
}

void Camera::SetBeforeDraw(void)
{
	//クリップ距離を設定する
	SetCameraNearFar(10.0f, 30000.0f);

	switch (mode_)
	{
	case Camera::MODE::FIXED_POINT:
		SetBeforeDrawFixedPoint();
		break;
	case Camera::MODE::FREE:
		SetBeforeDrawFree();
		break;
	}

	//カメラの設定(位置と角度による制御)
	SetCameraPositionAndAngle(
		pos_,
		angles_.x,
		angles_.y,
		angles_.z
	);
}

void Camera::SetBeforeDrawFixedPoint(void)
{
	//何もしない
}

void Camera::SetBeforeDrawFree(void)
{
	auto& ins = InputManager::GetInstance();

	//WASDでカメラの位置を変える
	float movePow = 3.0f;

 	if (ins.IsNew(KEY_INPUT_W)) { pos_.z += movePow; }
	if (ins.IsNew(KEY_INPUT_A)) { pos_.x -= movePow; }
	if (ins.IsNew(KEY_INPUT_S)) { pos_.z -= movePow; }
	if (ins.IsNew(KEY_INPUT_D)) { pos_.x += movePow; }
	if (ins.IsNew(KEY_INPUT_Q)) { pos_.y -= movePow; }
	if (ins.IsNew(KEY_INPUT_E)) { pos_.y += movePow; }

	if (ins.IsNew(KEY_INPUT_Z) && stepShotDelay <= 0.0f)
	{ 
		dx = 10.0f; 
		//玉発射後の硬直時間セット
		stepShotDelay = SHOT_DELAY;
	};

	//弾発射後の硬直時間を減らしていく
	if (stepShotDelay > 0.0f)
	{
		stepShotDelay -= 1.0f / SceneManager::DEFFAULT_FPS;
	}

	if (fabsf(dx) > 0.0f)
	{
		dx = -0.9f * dx;
	}
	SetPos(VGet(dx, pos_.y, pos_.z));

	// 矢印キーでカメラの角度を変える
	/*float rotPow = 1.0f * DX_PI_F / 180.0f;
	if (ins.IsNew(KEY_INPUT_DOWN)) { angles_.x -= rotPow; }
	if (ins.IsNew(KEY_INPUT_UP)) { angles_.x += rotPow; }
	if (ins.IsNew(KEY_INPUT_RIGHT)) { angles_.y += rotPow; }
	if (ins.IsNew(KEY_INPUT_LEFT)) { angles_.y -= rotPow; }*/
}

void Camera::Draw(void)
{

	DrawFormatString(0, 0, 0x000000,
		"カメラ座標：(% .2f, % .2f, % .2f)", pos_.x, pos_.y, pos_.z);
	DrawFormatString(0, 20, 0x000000, "カメラ角度：(%.2f, %.2f, %.2f)",
		angles_.x * 180.0f / DX_PI_F,
		angles_.y * 180.0f / DX_PI_F,
		angles_.z * 180.0f / DX_PI_F);
}

void Camera::Release(void)
{
}

void Camera::ChangeMode(MODE mode)
{
	// カメラモードの変更
	mode_ = mode;
	// 変更時の初期化処理
	switch (mode_)
	{
	case Camera::MODE::FIXED_POINT:
		break;
	case Camera::MODE::FREE:
		break;
	}

}

void Camera::SetPos(VECTOR pos)
{
	pos_ = pos;
}
