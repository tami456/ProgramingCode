#include <string>
#include <EffekseerForDxLib.h>
#include "../Application.h"
#include "../Utility/AsoUtility.h"
#include "../Manager/SceneManager.h"
#include "../Manager/ResourceManager.h"
#include "../Manager/GravityManager.h"
#include "../Manager/Camera.h"
#include "Common/AnimationController.h"
#include "Common/Capsule.h"
#include "Common/Collider.h"
#include "Common/SpeechBalloon.h"
#include "Planet.h"
#include "Player.h"
#include "Enemy.h"



Enemy::Enemy(Player* player) : mResourceManager(ResourceManager::GetInstance()), mGravityManager(GravityManager::GetInstance())
{
    mAnimationController = nullptr;
	mPlayer = player;
    mState = STATE::NONE;
}

Enemy::~Enemy()
{
}

void Enemy::Init(void)
{
	// モデルの基本設定
	mTransform.SetModel(mResourceManager.LoadModelDuplicate(
		ResourceManager::SRC::ENEMY));
	mTransform.scl = AsoUtility::VECTOR_ONE;
	mTransform.pos = { 100.0f, -30.0f, 100.0f };
	mTransform.quaRot = Quaternion();
	mTransform.quaRotLocal =
		Quaternion::Euler({ 0.0f, AsoUtility::Deg2RadF(180.0f), 0.0f });
	mTransform.Update();

	mPlayerRotY = Quaternion();
	mGoalQuaRot = Quaternion();
	mStepRot = 0.0f;

	mJumpPow = AsoUtility::VECTOR_ZERO;
	mGravHitDown = AsoUtility::VECTOR_ZERO;
	mGravHitUp = AsoUtility::VECTOR_ZERO;

	//丸影画像
	mImgShadow = LoadGraph((Application::PATH_IMAGE + "Shadow.png").c_str());

	//カプセルコライダ
	mCapsule = new Capsule(&mTransform);
	mCapsule->SetRelativePosTop({ 0.0f,110.0f,0.0f });
	mCapsule->SetRelativePosDown({ 0.0f,30.0f,0.0f });
	mCapsule->SetRadius(20.0f);

	mMoveDiff = AsoUtility::VECTOR_ZERO;

	mSlopeDir = AsoUtility::VECTOR_ZERO;
	mSlopeAngleDeg = 0.0f;
	mSlopePow = AsoUtility::VECTOR_ZERO;
	mHitNormal = AsoUtility::VECTOR_ZERO;
	mHitPos = AsoUtility::VECTOR_ZERO;

	mIsAlive = false;

	mHp = 100;

	// アニメーションの設定
	InitAnimation();

	//初期状態
	ChangeState(STATE::PLAY);
}

void Enemy::InitAnimation(void)
{
	std::string path = Application::PATH_MODEL + "Enemy/";
	mAnimationController = new AnimationController(mTransform.modelId);
	mAnimationController->Add((int)ANIM_TYPE::IDLE, path + "Dancing.mv1", 50.0f);
	mAnimationController->Add((int)ANIM_TYPE::RUN, path + "Run.mv1", 50.0f);
	mAnimationController->Add((int)ANIM_TYPE::DEAD, path + "Dead.mv1", 20.0f);

	mAnimationController->Play((int)ANIM_TYPE::RUN);
}

void Enemy::Update(void)
{
	switch (mState)
	{
	case Enemy::STATE::NONE:
		break;
	case Enemy::STATE::PLAY:
		UpdatePlay();
		break;
	case Enemy::STATE::IDLE:
		UpdateIdle();
		break;
	case Enemy::STATE::DEAD:
		break;
	}

	mTransform.Update();
	mAnimationController->Update();
}

bool IsHitSpheres(VECTOR& pos1, float radius1, VECTOR& pos2, float radius2)
{
	bool isHit = false;
	VECTOR diff = VSub(pos1, pos2);
	float radius = radius1 + radius2;

	auto dis = (diff.x * diff.x) + (diff.y * diff.y) + (diff.z * diff.z);

	if (dis < (radius * radius))
	{
		isHit = true;
	}

	return isHit;
}

void Enemy::UpdatePlay(void)
{
	

	//移動処理(移動量を決める)
	ProcessMove();

	//衝突判定
	Collision();

	//重力による移動量を計算
	CalcGravityPow();

	//回転
	mTransform.quaRot = mGravityManager.GetTransform().quaRot;

	//向かせたい向きを合成
	mTransform.quaRot = mTransform.quaRot.Mult(mPlayerRotY);

	//続きはプレイヤーに当たったらアニメーション変更
	if(IsHitSpheres(mPlayer->GetTransform()->pos, POS_RADIUS, mTransform.pos, POS_RADIUS))
	{
		ChangeState(STATE::IDLE);
	}
}

void Enemy::UpdateIdle(void)
{
	
	//続きはプレイヤーに当たったらアニメーション変更
	if (!IsHitSpheres(mPlayer->GetTransform()->pos, POS_RADIUS, mTransform.pos, POS_RADIUS))
	{
		ChangeState(STATE::PLAY);
	}
}

void Enemy::Draw(void)
{
	// モデルの描画
	MV1DrawModel(mTransform.modelId);

	//丸影描画
	DrawShadow();

	//DrawDebug();
}

void Enemy::DrawDebug(void)
{

	int white = 0xffffff;
	int black = 0x000000;
	int red = 0xff0000;
	int green = 0x00ff00;
	int blue = 0x0000ff;
	int yellow = 0xffff00;
	int purpl = 0x800080;

	//設置衝突用の線分
	DrawLine3D(mGravHitUp, mGravHitDown, 0xff0000);

	//カプセルコライダの表示
	mCapsule->Draw();

};

void Enemy::DrawShadow(void)
{
	float PLAYER_SHADOW_HEIGHT = 300.0f;
	float PLAYER_SHADOW_SIZE = 30.0f;

	int i, j;
	MV1_COLL_RESULT_POLY_DIM HitResDim;
	MV1_COLL_RESULT_POLY* HitRes;
	VERTEX3D Vertex[3];
	VECTOR SlideVec;
	int ModelHandle;

	// ライティングを無効にする
	SetUseLighting(FALSE);

	// Ｚバッファを有効にする
	SetUseZBuffer3D(TRUE);

	// テクスチャアドレスモードを CLAMP にする( テクスチャの端より先は端のドットが延々続く )
	SetTextureAddressMode(DX_TEXADDRESS_CLAMP);

	// 影を落とすモデルの数だけ繰り返し
	for (auto c : mColliders)
	{
		// チェックするモデルは、jが0の時はステージモデル、1以上の場合はコリジョンモデル
		ModelHandle = c->mModelId;


		// プレイヤーの直下に存在する地面のポリゴンを取得
		HitResDim = MV1CollCheck_Capsule(ModelHandle, -1,
			mTransform.pos,
			VAdd(mTransform.pos,
				VGet(0.0f, -PLAYER_SHADOW_HEIGHT, 0.0f)), PLAYER_SHADOW_SIZE);

		// 頂点データで変化が無い部分をセット
		Vertex[0].dif = GetColorU8(255, 255, 255, 255);
		Vertex[0].spc = GetColorU8(0, 0, 0, 0);
		Vertex[0].su = 0.0f;
		Vertex[0].sv = 0.0f;
		Vertex[1] = Vertex[0];
		Vertex[2] = Vertex[0];

		// 球の直下に存在するポリゴンの数だけ繰り返し
		HitRes = HitResDim.Dim;
		for (i = 0; i < HitResDim.HitNum; i++, HitRes++)
		{
			// ポリゴンの座標は地面ポリゴンの座標
			Vertex[0].pos = HitRes->Position[0];
			Vertex[1].pos = HitRes->Position[1];
			Vertex[2].pos = HitRes->Position[2];

			// ちょっと持ち上げて重ならないようにする
			SlideVec = VScale(HitRes->Normal, 0.5f);
			Vertex[0].pos = VAdd(Vertex[0].pos, SlideVec);
			Vertex[1].pos = VAdd(Vertex[1].pos, SlideVec);
			Vertex[2].pos = VAdd(Vertex[2].pos, SlideVec);

			// ポリゴンの不透明度を設定する
			Vertex[0].dif.a = 0;
			Vertex[1].dif.a = 0;
			Vertex[2].dif.a = 0;
			if (HitRes->Position[0].y > mTransform.pos.y - PLAYER_SHADOW_HEIGHT)
				Vertex[0].dif.a = 128 * (1.0f - fabs(HitRes->Position[0].y - mTransform.pos.y) / PLAYER_SHADOW_HEIGHT);

			if (HitRes->Position[1].y > mTransform.pos.y - PLAYER_SHADOW_HEIGHT)
				Vertex[1].dif.a = 128 * (1.0f - fabs(HitRes->Position[1].y - mTransform.pos.y) / PLAYER_SHADOW_HEIGHT);

			if (HitRes->Position[2].y > mTransform.pos.y - PLAYER_SHADOW_HEIGHT)
				Vertex[2].dif.a = 128 * (1.0f - fabs(HitRes->Position[2].y - mTransform.pos.y) / PLAYER_SHADOW_HEIGHT);

			// ＵＶ値は地面ポリゴンとプレイヤーの相対座標から割り出す
			Vertex[0].u = (HitRes->Position[0].x - mTransform.pos.x) / (PLAYER_SHADOW_SIZE * 2.0f) + 0.5f;
			Vertex[0].v = (HitRes->Position[0].z - mTransform.pos.z) / (PLAYER_SHADOW_SIZE * 2.0f) + 0.5f;
			Vertex[1].u = (HitRes->Position[1].x - mTransform.pos.x) / (PLAYER_SHADOW_SIZE * 2.0f) + 0.5f;
			Vertex[1].v = (HitRes->Position[1].z - mTransform.pos.z) / (PLAYER_SHADOW_SIZE * 2.0f) + 0.5f;
			Vertex[2].u = (HitRes->Position[2].x - mTransform.pos.x) / (PLAYER_SHADOW_SIZE * 2.0f) + 0.5f;
			Vertex[2].v = (HitRes->Position[2].z - mTransform.pos.z) / (PLAYER_SHADOW_SIZE * 2.0f) + 0.5f;

			// 影ポリゴンを描画
			DrawPolygon3D(Vertex, 1, mImgShadow, TRUE);
		}

		// 検出した地面ポリゴン情報の後始末
		MV1CollResultPolyDimTerminate(HitResDim);
	}

	// ライティングを有効にする
	SetUseLighting(TRUE);

	// Ｚバッファを無効にする
	SetUseZBuffer3D(FALSE);
}

void Enemy::Release(void)
{
	mAnimationController->Release();
	delete mAnimationController;

	delete mCapsule;
	DeleteGraph(mImgShadow);
}

Transform& Enemy::GetTransform(void)
{
	return mTransform;
}

Capsule* Enemy::GetCapsule(void)
{
	return mCapsule;
}

float Enemy::GetCollisionRadius(void) const
{
	return POS_RADIUS;
}

bool Enemy::IsAlive(void) const
{
	return mIsAlive;
}

void Enemy::SetGoalRotate(float rotRad)
{
	//rad単位のアングルをとる
	VECTOR enemyAngle = SceneManager::GetInstance().GetCamera()->GetAngles();

	Quaternion axis =
		Quaternion::AngleAxis(
			rotRad,
			AsoUtility::AXIS_Y);

	//現在設定されている回転との角度差を求める
	double angleDiff = Quaternion::Angle(axis, mGoalQuaRot);

	//しきい値
	if (angleDiff > 0.1f)
	{
		mStepRot = TIME_ROT;
	}

	//到達したい回転を設定
	mGoalQuaRot = axis;
}

void Enemy::Rotate(void)
{
	mStepRot -= SceneManager::GetInstance().GetDeltaTime();

	//mPlayerRotY = Quaternion::Slerp(
	//mPlayerRotY,mGoalQuaRot,(TIME_ROT - mStepRot) / TIME_ROT);

	mPlayerRotY = Quaternion::Slerp(
		mPlayerRotY, mGoalQuaRot, 0.5f);
}

void Enemy::ProcessMove(void)
{

	//移動量を0
	mMovePow = AsoUtility::VECTOR_ZERO;

	float rotRad = 0;

	//移動方向を決める
	// ３Ｄモデル２から３Ｄモデル１に向かうベクトルを算出
	dir = VSub(mPlayer->GetTransform()->pos, mTransform.pos);
	dir = VNorm(dir);

	// atan2 を使用して角度を取得
	float Angle = atan2(dir.x, dir.z);

	mSpeed = SPEED_MOVE;

	//回転(到達したい回転を決める)
	SetGoalRotate(Angle);

	//移動方向から移動量を算出する
		//移動量　= 方向　×　スピード
	mMoveDir = dir;
	mMovePow = VScale(dir, mSpeed);

	//回転(ゴールに対して、球面補完を行う)
	Rotate();
}

void Enemy::CalcGravityPow(void)
{
	//重力方向
	VECTOR dirGravity = mGravityManager.GetDirGravity();

	//重力の強さ
	float gravityPow = mGravityManager.GetPower();

	//重力
	VECTOR gravity = VScale(dirGravity, gravityPow);

	//上下の移動量に重力を加える
	mJumpPow = VAdd(mJumpPow, gravity);

	float dot = VDot(dirGravity, mJumpPow);
	if (dot >= 0.0f)
	{
		//重力方向と反対方向でなければ、ジャンプ力を打ち消す
		mJumpPow = gravity;
	}
}

void Enemy::CalcSlope(void)
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

	//傾斜による移動
	if (AsoUtility::SqrMagnitude(mJumpPow) == 0.0f)
	{
		float CHK_ANGLE = 120.0f;
		if (mSlopeAngleDeg >= CHK_ANGLE)
		{
			float diff = abs(mSlopeAngleDeg - CHK_ANGLE);
			mSlopePow = VScale(mSlopeDir, diff / 3.0f);
			mMovePow = VAdd(mMovePow, mSlopePow);
		}
	}
}

void Enemy::AddCollider(Collider* collider)
{
	mColliders.emplace_back(collider);
}

void Enemy::ClearCollider(void)
{
	mColliders.clear();
}

void Enemy::Damage(void)
{
	mTransform.pos = VAdd(mTransform.pos, VScale(mTransform.GetBack(), 30.0f));
	mTransform.Update();
	mHp -= 1;
	if (mHp <= 0)
	{
		ChangeState(STATE::DEAD);
	}
}

bool Enemy::IsDead(void)
{
	return mState == STATE::DEAD;
}

void Enemy::ChangeState(STATE state)
{
	mState = state;
	switch (mState)
	{
	case Enemy::STATE::NONE:
		break;
	case Enemy::STATE::PLAY:
		mAnimationController->Play((int)ANIM_TYPE::RUN);
		mIsAlive = true;
		break;
	case Enemy::STATE::IDLE:
		mAnimationController->Play((int)ANIM_TYPE::IDLE);
		mIsAlive = true;
		break;
	case Enemy::STATE::DEAD:
		mAnimationController->Play((int)ANIM_TYPE::DEAD, false);
		mIsAlive = false;
		break;
	}
}

void Enemy::Collision(void)
{
	//現在座標を起点に移動後座標を決める
	mMovedPos = VAdd(mTransform.pos, mMovePow);

	//衝突処理(カプセル)
	CollisionCapsule();

	//衝突処理(重力)
	CollisionGravity();

	//フレーム間の移動量
	mMoveDiff = VSub(mMovedPos, mTransform.pos);

	//移動
	mTransform.pos = mMovedPos;
}

void Enemy::CollisionCapsule(void)
{
	//カプセルを移動する
	//コピーコンストラクタ…
	//コンストラクタのような形式を呼んで自分自身を呼ぶ

	Transform trans = Transform(mTransform);
	trans.pos = mMovedPos;
	trans.Update();
	Capsule cap = mCapsule->Copy(&trans);

	//カプセルとステージモデルの衝突判定
	for (auto c : mColliders)
	{
		auto hits = MV1CollCheck_Capsule(
			c->mModelId, -1,
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
					mMovedPos = VAdd(mMovedPos, VScale(Nhit, pow));

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

void Enemy::CollisionGravity(void)
{
	//ジャンプ量を加算
	mMovedPos = VAdd(mMovedPos, mJumpPow);

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
		auto mJumpDot = VDot(dirGravity, VNorm(mJumpPow));
		if (hit.HitFlag > 0 && mJumpDot > 0.9f)
		{
			//傾斜判定用に衝突情報を取っておく
			mHitNormal = hit.Normal;
			mHitPos = hit.HitPosition;

			//衝突している(衝突した座標(ちょい上))
			float dis = 2.0f;
			mMovedPos = VAdd(hit.HitPosition,
				VScale(dirUpGravity, dis));

			//抵抗力の代わりに(着地したら重力を打ち消す)
			mJumpPow = AsoUtility::VECTOR_ZERO;

		}
	}
}
