#include <string>
#include <EffekseerForDxLib.h>
#include "../Application.h"
#include "../Utility/AsoUtility.h"
#include "../Manager/SceneManager.h"
#include "../Manager/ResourceManager.h"
#include "../Manager/GravityManager.h"
#include "../Manager/Camera.h"
#include "Common/Capsule.h"
#include "Common/Collider.h"
#include "Planet.h"
#include "PlayerShot.h"
#include "Player.h"

Player::Player(void) : mResourceManager(ResourceManager::GetInstance()), mGravityManager(GravityManager::GetInstance())
{
	mTransformWheelL.SetModel(mResourceManager.LoadModelDuplicate(
		ResourceManager::SRC::TANK_WHEEL));

	mTransformWheelR.SetModel(mResourceManager.LoadModelDuplicate(
		ResourceManager::SRC::TANK_WHEEL));

	mTransformStand.SetModel(mResourceManager.LoadModelDuplicate(
		ResourceManager::SRC::TANK_BODY));

	mTransformGun.SetModel(mResourceManager.LoadModelDuplicate(
		ResourceManager::SRC::TANK_BARREL));
	
	mState = STATE::NONE;
}

Player::~Player(void)
{
}

void Player::Init(void)
{

	// モデルの基本設定
	
	mTransformWheelL.scl = VGet(SCALE, SCALE, SCALE);
	mTransformWheelL.quaRot = Quaternion();
	mTransformWheelL.quaRotLocal =
		Quaternion::Euler({ 0.0f, AsoUtility::Deg2RadF(180.0f), 0.0f });

	//親も出るとの同期(主に座標と回転)
	SyncParent(mTransformWheelL, { 0.0f,0.0f,0.0f });

	mTransformWheelR.scl = VGet(SCALE, SCALE, SCALE);
	mTransformWheelR.quaRot = Quaternion();
	mTransformWheelR.quaRotLocal =
		Quaternion::Euler({ 0.0f, AsoUtility::Deg2RadF(180.0f), 0.0f });

	//親も出るとの同期(主に座標と回転)
	SyncParent(mTransformWheelR, { 0.0f,0.0f,0.0f });

	mTransformStand.scl = VGet(SCALE, SCALE, SCALE);
	mTransformStand.quaRot = Quaternion();
	mTransformStand.quaRotLocal =
		Quaternion::Euler({ 0.0f, AsoUtility::Deg2RadF(180.0f), 0.0f });

	//親も出るとの同期(主に座標と回転)
	SyncParent(mTransformStand, { 0.0f,0.0f,0.0f });

	mTransformGun.scl = VGet(SCALE, SCALE, SCALE);
	mTransformGun.quaRot = Quaternion();
	mTransformGun.quaRotLocal =
		Quaternion::Euler({ 0.0f, AsoUtility::Deg2RadF(180.0f), 0.0f });

	//親も出るとの同期(主に座標と回転)
	SyncParent(mTransformGun, { 0.0f,0.0f,0.0f });

	mTransformWheelL.pos = { -140.0f, -30.0f, 0.0f };
	mTransformWheelL.pos = { 140.0f, -30.0f, 0.0f };
	mTransformStand.pos = { 0.0f, 50.0f, 0.0f };
	mTransformGun.pos = { 0.0f,0.0f, -50.0f };

	mPlayerRotY = Quaternion();
	mGoalQuaRot = Quaternion();
	mStepRot = 0.0f;

	mGravHitDown = AsoUtility::VECTOR_ZERO;
	mGravHitUp = AsoUtility::VECTOR_ZERO;

	//丸影画像
	mImgShadow = LoadGraph((Application::PATH_IMAGE + "Shadow.png").c_str());

	//カプセルコライダ
	mCapsule = new Capsule(&mTransformWheelL);
	mCapsule = new Capsule(&mTransformStand);
	mCapsule = new Capsule(&mTransformGun);
	mCapsule->SetRelativePosTop({0.0f,110.0f,0.0f});
	mCapsule->SetRelativePosDown({0.0f,30.0f,0.0f});
	mCapsule->SetRadius(20.0f);

	mMoveDiff = AsoUtility::VECTOR_ZERO;


	mSlopeDir = AsoUtility::VECTOR_ZERO;
	mSlopeAngleDeg = 0.0f;
	mSlopePow = AsoUtility::VECTOR_ZERO;
	mHitNormal = AsoUtility::VECTOR_ZERO;
	mHitPos = AsoUtility::VECTOR_ZERO;

	//初期状態
	ChangeState(STATE::PLAY);
}


void Player::Update(void)
{

	switch (mState)
	{
	case Player::STATE::NONE:
		break;
	case Player::STATE::PLAY:
		UpdatePlay();
		break;
	case Player::STATE::DEAD:
		UpdateDead();
		break;
	case Player::STATE::VICTORY:
		break;
	case Player::STATE::END:
		break;
	}

	
}

void Player::UpdatePlay(void)
{
	//移動処理(移動量を決める)
	ProcessMove();
	
	Attack();

	//傾斜の計算
	CalcSlope();

	//重力による移動量を計算
	CalcGravityPow();

	//衝突判定
	Collision();

	auto name = mGravityManager.GetActivePlanet()->GetName();
	auto planetDead = mGravityManager.GetActivePlanet();
	if (mTransformWheelL.pos.y < -1000.0f || (planetDead->InRangeDead(mTransformWheelL.pos) && name != mPreWarpName))
	{
		ChangeState(STATE::DEAD);
	}

}

void Player::UpdateDead(void)
{
	//普通の操作状態の戻す
	ChangeState(STATE::DEAD);
}

void Player::Draw(void)
{

	// モデルの描画
	MV1DrawModel(mTransformWheelL.modelId);
	MV1DrawModel(mTransformWheelR.modelId);
	MV1DrawModel(mTransformStand.modelId);
	MV1DrawModel(mTransformGun.modelId);

	

	// デバッグ用描画
	//DrawDebug();

}

void Player::Release(void)
{

	delete mCapsule;
	DeleteGraph(mImgShadow);

	//弾の開放
	for (auto s : mShots)
	{
		s->Release();
		delete s;
	}
	mShots.clear();
}

void Player::Attack(void)
{
	//砲台の稼働回転(rad)

	if (CheckHitKey(KEY_INPUT_RIGHT))
	{
		mLocalAddAxisStand.y += AsoUtility::Deg2RadF(1.0f);

	}
	if (CheckHitKey(KEY_INPUT_LEFT))
	{
		mLocalAddAxisStand.y -= AsoUtility::Deg2RadF(1.0f);
	}

	//砲台を親モデルと同期させる
	SyncParent(mTransformStand, mLocalAddAxisStand);

	//砲身の稼働回転(rad)
	if (CheckHitKey(KEY_INPUT_UP))
	{
		mLocalAddAxisGun.x += AsoUtility::Deg2RadF(1.0f);
		if (mLocalAddAxisGun.x > ANGLE_Y_MAX_GUN)
		{
			mLocalAddAxisGun.x = ANGLE_Y_MAX_GUN;
		}

	}
	if (CheckHitKey(KEY_INPUT_DOWN))
	{
		mLocalAddAxisGun.x -= AsoUtility::Deg2RadF(1.0f);
		if (mLocalAddAxisGun.x < ANGLE_Y_MIN_GUN)
		{
			mLocalAddAxisGun.x = ANGLE_Y_MIN_GUN;
		}
	}

	//砲台が回転した分、砲身も回転する必要があるので親子回転
	Quaternion stand = Quaternion::Euler(mLocalAddAxisStand);
	Quaternion gun = Quaternion::Euler(mLocalAddAxisGun);
	Quaternion mix = stand.Mult(gun);

	//砲身を親モデルと同期させる
	SyncParent(mTransformGun, mix.ToEuler());
	SyncParent(mTransformWheelL, mix.ToEuler());
	SyncParent(mTransformWheelR, mix.ToEuler());
}

Transform* Player::GetTransform(void)
{
	return &mTransformWheelL;
}

Capsule* Player::GetCapsule(void)
{
	return mCapsule;
}

void Player::ProcessMove(void)
{

	//移動量を0
	mMovePow = AsoUtility::VECTOR_ZERO;

	//X軸回転を除いた、重力方向に垂直なカメラ角度(XZ平面)を取得
	Quaternion cameraRot = mTransformStand.quaRot;

	//移動方向
	VECTOR dir = AsoUtility::VECTOR_ZERO;

	if (CheckHitKey(KEY_INPUT_W))
	{
		//移動方向を決める
		dir = cameraRot.GetForward();
	}
	if (CheckHitKey(KEY_INPUT_S))
	{
		//移動方向を決める
		dir = cameraRot.GetBack();
	}

	//移動キーが入力されていたら	  &&  ジャンプモーション中だったら
	if (!AsoUtility::EqualsVZero(dir))
	{

		mSpeed = SPEED_MOVE;

		//移動方向から移動量を算出する
		//移動量　= 方向　×　スピード
		mMoveDir = dir;
		mMovePow = VScale(dir, mSpeed);

		if (CheckHitKey(KEY_INPUT_E))
		{
			ProcessShot();
		}
	}
}

void Player::ProcessShot(void)
{
	//弾の生成ディレイ
	mStepDelayShot -= SceneManager::GetInstance().GetDeltaTime();
	if (mStepDelayShot < 0.0f)
	{
		mStepDelayShot = 0.0f;
	}

	//キーチェック
	if (CheckHitKey(KEY_INPUT_E) && mStepDelayShot <= 0.0f)
	{
		//ディレイ時間をセット
		mStepDelayShot = TIME_DELAY_SHOT;

		//弾を生成
		CreateShot();
	}
}

void Player::CalcSlope(void)
{
	//重力の反対方向
	VECTOR gravityUp = mGravityManager.GetDirUpGravity();

	//重力の反対方向から地面の法線方向に向けた回転量
	Quaternion up2NorQua = Quaternion::FromToRotation(gravityUp, mHitNormal);

	//取得した回転の軸と角度を取得する
	float angle = 0.0f;
	VECTOR axis;
	up2NorQua.ToAngleAxis(&angle, &axis);

	//90度を足して、傾斜ベクトルへの回転を取得する
	Quaternion slopeQua = Quaternion::AngleAxis(
		angle + AsoUtility::Deg2RadF(90.0f), axis);

	//地面の法線方向を傾斜方向に回転させる
	mSlopeDir = slopeQua.PosAxis(gravityUp);

	//傾斜の角度
	mSlopeAngleDeg = static_cast<float>(
		AsoUtility::AngleDeg(gravityUp, mSlopeDir));
}

void Player::CalcGravityPow(void)
{

	//重力方向
	VECTOR dirGravity = mGravityManager.GetDirGravity();

	//重力の強さ
	float gravityPow = mGravityManager.GetPower();

	//重力
	VECTOR gravity = VScale(dirGravity,gravityPow);

	//上下の移動量に重力を加える
	mJumpPow = VAdd(mJumpPow, gravity);

	float dot = VDot(dirGravity, mJumpPow);
	if (dot >= 0.0f)
	{
		//重力方向と反対方向でなければ、ジャンプ力を打ち消す
		mJumpPow = gravity;
	}

}

void Player::AddCollider(Collider* collider)
{
	mColliders.emplace_back(collider);

}

void Player::ClearCollider(void)
{
	mColliders.clear();
}

bool Player::IsPlay(void)
{
	return mState == STATE::PLAY;
}



bool Player::IsDead(void)
{
	return mState == STATE::DEAD;
}

std::vector<PlayerShot*>& Player::GetShots(void)
{
	return mShots;
}

void Player::SyncParent(Transform& transform, VECTOR addAxis)
{
	//ボス戦艦の3D情報を取得
	Quaternion parentRot = mTransformStand.quaRot;

	Quaternion localRot;
	Quaternion axis;

	//Y
	axis = Quaternion::AngleAxis(
		mLocalAddAxis.y, AsoUtility::AXIS_Y);
	localRot = localRot.Mult(axis);

	//X
	axis = Quaternion::AngleAxis(
		mLocalAddAxis.x, AsoUtility::AXIS_X);
	localRot = localRot.Mult(axis);

	//Z
	axis = Quaternion::AngleAxis(
		mLocalAddAxis.z, AsoUtility::AXIS_Z);
	localRot = localRot.Mult(axis);

	//自立稼働分の回転をさらに加える
	//------------------------------------------
	//Y
	axis = Quaternion::AngleAxis(
		addAxis.y, AsoUtility::AXIS_Y);
	localRot = localRot.Mult(axis);

	//X
	axis = Quaternion::AngleAxis(
		addAxis.x, AsoUtility::AXIS_X);
	localRot = localRot.Mult(axis);

	//Z
	axis = Quaternion::AngleAxis(
		addAxis.z, AsoUtility::AXIS_Z);
	localRot = localRot.Mult(axis);
	//------------------------------------------

	//親の回転に沿って、砲台・砲身を回転させる
	transform.quaRot = parentRot.Mult(localRot);

	//親の回転情報に合わせた位置を計算
	VECTOR localPos = Quaternion::PosAxis(parentRot, mLocalPos);
	//□今まで習ったやり方
	////相対座標を絶対座標に変換
	//transform.pos = VAdd(
	//mTransformParent->pos, localPos);

	//モデルの大きさ分、スケールした相対座標を絶対座標に変換
	transform.pos = VAdd(
		mTransformStand.pos, VScale(localPos, SCALE));

	//モデル制御の基本情報更新
	transform.Update();

}


void Player::ChangeState(STATE state)
{

	mState = state;
	switch (mState)
	{
	case Player::STATE::NONE:
		break;
	case Player::STATE::PLAY:
		break;
	case Player::STATE::DEAD:
		break;
	case Player::STATE::VICTORY:
		break;
	case Player::STATE::END:
		break;
	}

}

void Player::Collision(void)
{
	
	//現在座標を起点に移動後座標を決める
	mMovedPos = VAdd(mTransformStand.pos, mMovePow);

	//衝突処理(カプセル)
	CollisionCapsule();

	//衝突処理(重力)
	CollisionGravity();

	//フレーム間の移動量
	mMoveDiff = VSub(mMovedPos, mTransformStand.pos);

	//移動
	mTransformStand.pos = mMovedPos;
}

void Player::CollisionCapsule(void)
{
	//カプセルを移動する
	//コピーコンストラクタ…
	//コンストラクタのような形式を呼んで自分自身を呼ぶ
	
	Transform trans = Transform(mTransformStand);
	trans.pos = mMovedPos;
	trans.Update();
	Capsule cap = mCapsule->Copy(&trans);

	//カプセルとステージモデルの衝突判定
	for (auto c : mColliders)
	{
		auto hits = MV1CollCheck_Capsule(
			c->mModelId,-1,
			cap.GetPosTop(),
			cap.GetPosDown(),
			cap.GetRadius()
			);
		
		//衝突処理

		for (int i = 0; i < hits.HitNum; i++)
		{
			//一つ一つの衝突情報を取得する
			auto hit = hits.Dim[i];

			for (int tryCnt = 0; tryCnt < 10; tryCnt++)
			{
				//処理負荷を減らすために、1ポリゴンと
				//カプセルの衝突判定にする
				int pHit = HitCheck_Capsule_Triangle(
					cap.GetPosTop(),
					cap.GetPosDown(),
					cap.GetRadius(),
					hit.Position[0],
					hit.Position[1],
					hit.Position[2]
				);

				if (pHit)
				{
					//衝突している
					float pow = 1.0f;				//ちょっとだけ動かす
					auto Nhit = hit.Normal;			//衝突したポリゴンの法線

					//mMovedPosを衝突したポリゴンの法線に、
					//ちょっとだけ移動させる
					mMovedPos = VAdd(mMovedPos,VScale(Nhit, pow));

					//カプセルを移動させる
					trans.pos = mMovedPos;
					trans.Update();
					cap = mCapsule->Copy(&trans);

					continue;
				}

				//衝突していないのでループ終了
				break;
			}
		}
		//検出したポリゴンとの衝突情報を後始末
		MV1CollResultPolyDimTerminate(hits);
	}
}

void Player::CollisionGravity(void)
{

	//重力の方向
	VECTOR dirGravity = mGravityManager.GetDirGravity();

	//重力の反対方向
	VECTOR dirUpGravity = mGravityManager.GetDirUpGravity();

	//重力の強さ
	float gravityPow = mGravityManager.GetPower();

	//足元 mMovedPos
	
	float checkPow = 10.0f;

	//足元のちょっと上の座標
	mGravHitUp = VAdd(mMovedPos, VScale(dirUpGravity, gravityPow));
	mGravHitUp = VAdd(mGravHitUp, VScale(dirUpGravity, checkPow * 2.0f));

	//足元のちょっと下の座標
	mGravHitDown = VAdd(mMovedPos, VScale(dirGravity, checkPow));
	
	for (auto c : mColliders)
	{
		//地面(ステージモデル)との衝突
		//線分の場合は衝突の範囲が1ポリゴンだけになる
		auto hit = MV1CollCheck_Line(c->mModelId, -1, mGravHitUp, mGravHitDown);
		if (hit.HitFlag > 0)
		{
			//傾斜判定用に衝突情報を取っておく
			mHitNormal = hit.Normal;
			mHitPos = hit.HitPosition;

			//衝突している(衝突した座標(ちょい上))
			float dis = 2.0f;
			mMovedPos = VAdd(hit.HitPosition,
				VScale(dirUpGravity, dis));
		}
	}
}

void Player::CreateShot(void)
{
	bool isCreate = false;

	VECTOR pos = mTransformGun.pos;

	for (auto s : mShots)
	{
		if (!s->IsAlive())
		{
			//生存していなかったら
			//インスタンスを使いまわす
			s->Create(
				VGet(pos.x - 10.0f,pos.y,pos.z),
				mTransformGun.GetForward());
			isCreate = true;
		}
	}

	if (!isCreate)
	{
		//インスタンスを新しく作る
		auto newShot = new PlayerShot(&mTransformGun);
		newShot->Create(
			VGet(pos.x - 10.0f, pos.y, pos.z),
			mTransformGun.GetForward());

		//可変長配列に追加
		mShots.emplace_back(newShot);
	}
}
