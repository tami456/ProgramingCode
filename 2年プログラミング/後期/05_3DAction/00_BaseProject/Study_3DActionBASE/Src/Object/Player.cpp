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
#include "PlayerShot.h"
#include "Player.h"

Player::Player(void) : mResourceManager(ResourceManager::GetInstance()), mGravityManager(GravityManager::GetInstance())
{

	mAnimationController = nullptr;
	mState = STATE::NONE;
}

Player::~Player(void)
{
}

void Player::Init(void)
{

	// モデルの基本設定
	mTransform.SetModel(mResourceManager.LoadModelDuplicate(
		ResourceManager::SRC::PLAYER));
	mTransform.scl = AsoUtility::VECTOR_ONE;
	mTransform.pos = { 0.0f, -30.0f, 0.0f };
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

	mIsJump = false;
	mStepJump = 0.0f;

	//カプセルコライダ
	mCapsule = new Capsule(&mTransform);
	mCapsule->SetRelativePosTop({0.0f,110.0f,0.0f});
	mCapsule->SetRelativePosDown({0.0f,30.0f,0.0f});
	mCapsule->SetRadius(20.0f);

	//足煙のエフェクト
	mEffectSmoke = mResourceManager.Load(
		ResourceManager::SRC::FOOT_SMOKE).mHandleId;
	
	mStepFootSmoke = TIME_FOOT_SMOKE;
	mMoveDiff = AsoUtility::VECTOR_ZERO;
	mPlayEffectSmoke = -1;

	//ワープエフェクト
	mEffectWarpOrbit = mResourceManager.Load(
		ResourceManager::SRC::WARP_ORBIT).mHandleId;

	//モデルフレーム(手)モデルフレームを探す
	mFrameLeftHand = MV1SearchFrame(mTransform.modelId,"mixamorig:LeftHand");
	mFrameRightHand = MV1SearchFrame(mTransform.modelId, "mixamorig:RightHand");


	mSlopeDir = AsoUtility::VECTOR_ZERO;
	mSlopeAngleDeg = 0.0f;
	mSlopePow = AsoUtility::VECTOR_ZERO;
	mHitNormal = AsoUtility::VECTOR_ZERO;
	mHitPos = AsoUtility::VECTOR_ZERO;


	mSpeechBalloon = new SpeechBalloon(
		SpeechBalloon::TYPE::SPEECH, &mTransform);

	mSpeechBalloon->SetTime(15.0f);
	mSpeechBalloon->SetRelativePos({ 15.0f,180.0f,0.0f });

	// アニメーションの設定
	InitAnimation();

	//初期状態
	ChangeState(STATE::PLAY);
}

void Player::InitAnimation(void)
{

	std::string path = Application::PATH_MODEL + "Player/";
	mAnimationController = new AnimationController(mTransform.modelId);
	mAnimationController->Add((int)ANIM_TYPE::IDLE, path + "Idle.mv1", 20.0f);
	mAnimationController->Add((int)ANIM_TYPE::RUN, path + "Run.mv1", 20.0f);
	mAnimationController->Add((int)ANIM_TYPE::FAST_RUN, path + "FastRun.mv1", 20.0f);
	mAnimationController->Add((int)ANIM_TYPE::JUMP, path + "Jump.mv1", 60.0f);
	mAnimationController->Add((int)ANIM_TYPE::WARP_PAUSE, path + "WarpPose.mv1", 60.0f);
	mAnimationController->Add((int)ANIM_TYPE::FLY, path + "Flying.mv1", 60.0f);
	mAnimationController->Add((int)ANIM_TYPE::FALLING, path + "Falling.mv1", 80.0f);
	mAnimationController->Add((int)ANIM_TYPE::VICTORY, path + "Victory.mv1", 60.0f);
	mAnimationController->Add((int)ANIM_TYPE::KICK, path + "magic.mv1", 90.0f);

	mAnimationController->Play((int)ANIM_TYPE::IDLE);

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
	case Player::STATE::WARP_RESERVE:
		UpdateWarpReserve();
		break;
	case Player::STATE::WARP_MOVE:
		UpdateWarpMove();
		break;
	case Player::STATE::DEAD:
		UpdateDead();
		break;
	case Player::STATE::VICTORY:
		break;
	case Player::STATE::END:
		break;
	}

	mTransform.Update();
	mAnimationController->Update();
	
}

void Player::UpdatePlay(void)
{
	//移動処理(移動量を決める)
	ProcessMove();
	
	//ジャンプ処理
	ProcessJump();

	//傾斜の計算
	CalcSlope();

	//重力による移動量を計算
	CalcGravityPow();

	//衝突判定
	Collision();

	//回転
	mTransform.quaRot = mGravityManager.GetTransform().quaRot;

	//向かせたい向きを合成
	mTransform.quaRot = mTransform.quaRot.Mult(mPlayerRotY);

	//足煙のエフェクト処理
	EffectFootSmoke();
	auto name = mGravityManager.GetActivePlanet()->GetName();
	auto planetDead = mGravityManager.GetActivePlanet();
	if (mTransform.pos.y < -1000.0f || (planetDead->InRangeDead(mTransform.pos) && name != mPreWarpName))
	{
		ChangeState(STATE::DEAD);
	}

}

void Player::UpdateWarpReserve(void)
{
	mStepWarp -= SceneManager::GetInstance().GetDeltaTime();
	if (mStepWarp < 0.0f)
	{
		//到達したら
		mTransform.quaRot = mWarpQua;
		mTransform.pos = mWarpReservePos;
		ChangeState(STATE::WARP_MOVE);

		SyncWarpOrbitPos();
	}
	else
	{
		//徐々にプレイヤーを準備座標に移動・回転させる

		//スタート回転：mReserveStartQua;
		//スタート座標：mReserveStartPos;
		
		//ゴール回転：mWarpQua;
		//ゴール座標：mWarpReservePos;

		//移動前と移動後の座標と回転の比率を渡す
		//座標は線形補完
		//tの値が、0.0fだったらmReserveStartPos
		//tの値が、0.5fだったら↑↓の中間
		//tの値が、1.0fだったらmWarpReservePos

		float t;
		t = 1.0f - (mStepWarp/mTimeWarp);
		mTransform.pos = AsoUtility::Lerp(mReserveStartPos, mWarpReservePos, t);
		

		//回転は球面補完
		mTransform.quaRot = Quaternion::Slerp(mReserveStartQua, mWarpQua, t);
	}
}

void Player::UpdateWarpMove(void)
{
	
	//キャラクターを移動させる処理
	//スターの方向、移動したい方向(回転)はすでに決まっているので
	//その方向にプレイヤーを移動させる
	mTransform.pos = VAdd(mTransform.pos,VScale(mWarpQua.GetForward(), SPEED_FLY));
	mTransform.Update();

	auto name = mGravityManager.GetActivePlanet()->GetName();
	if (name != mPreWarpName)
	{
		//エフェクト停止
		StopOrbitEffect();

		//落下アニメーション
		mAnimationController->Play(
			static_cast<int>(ANIM_TYPE::JUMP),
			true, 13.0f, 25.0f);

		mAnimationController->SetEndLoop(23.0f, 25.0f, 5.0f);

		//普通の操作状態の戻す
		ChangeState(STATE::PLAY);

	}
	
	//エフェクトの位置同期
	SyncWarpOrbitPos();
}

void Player::UpdateDead(void)
{
	//普通の操作状態の戻す
	ChangeState(STATE::DEAD);
}

void Player::Draw(void)
{

	// モデルの描画
	MV1DrawModel(mTransform.modelId);

	//丸影描画
	DrawShadow();

	// デバッグ用描画
	//DrawDebug();

}

void Player::DrawShadow(void)
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
			if (HitRes->Position[0].y >mTransform.pos.y - PLAYER_SHADOW_HEIGHT)
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



void Player::DrawDebug(void)
{

	int white = 0xffffff;
	int black = 0x000000;
	int red = 0xff0000;
	int green = 0x00ff00;
	int blue = 0x0000ff;
	int yellow = 0xffff00;
	int purpl = 0x800080;

	VECTOR v;

	// キャラ基本情報
	//-------------------------------------------------------
	// キャラ座標
	v = mTransform.pos;
	DrawFormatString(20, 60, black, "キャラ座標 ： (%0.2f, %0.2f, %0.2f)",
		v.x, v.y, v.z
	);

	v = mTransform.pos;
	DrawFormatString(20, 60, black, "傾斜角 ： %0.2f",
		mSlopeAngleDeg
	);
	//-------------------------------------------------------

	//設置衝突用の線分
	DrawLine3D(mGravHitUp,mGravHitDown,0xff0000);

	//カプセルコライダの表示
	mCapsule->Draw();

};
void Player::Release(void)
{
	mAnimationController->Release();
	delete mAnimationController;

	delete mCapsule;
	DeleteGraph(mImgShadow);

	//弾の開放
	for (auto s : mShots)
	{
		s->Release();
		delete s;
	}
	mShots.clear();

	mSpeechBalloon->Release();
	delete mSpeechBalloon;
}

Transform* Player::GetTransform(void)
{
	return &mTransform;
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
	Quaternion cameraRot = SceneManager::GetInstance().GetCamera()->GetQuaRotOutX();

	//移動方向
	VECTOR dir = AsoUtility::VECTOR_ZERO;
	float rotRad = 0;

	if (CheckHitKey(KEY_INPUT_W))
	{
		//移動方向を決める
		dir = cameraRot.GetForward();
		rotRad = AsoUtility::Deg2RadF(0.0f);
	}
	if (CheckHitKey(KEY_INPUT_S))
	{
		//移動方向を決める
		dir = cameraRot.GetBack();
		rotRad = AsoUtility::Deg2RadF(180.0f);
	}
	if (CheckHitKey(KEY_INPUT_D))
	{
		//移動方向を決める
		dir = cameraRot.GetRight();
		rotRad = AsoUtility::Deg2RadF(90.0f);
	}
	if (CheckHitKey(KEY_INPUT_A))
	{
		//移動方向を決める
		dir = cameraRot.GetLeft();
		rotRad = AsoUtility::Deg2RadF(-90.0f);
	}

	//移動キーが入力されていたら	  &&  ジャンプモーション中だったら
	if (!AsoUtility::EqualsVZero(dir) && (mIsJump || IsEndLanding()))
	{
		//回転(到達したい回転を決める)
		SetGoalRotate(rotRad);

		ANIM_TYPE type = ANIM_TYPE::RUN;
		mSpeed = SPEED_MOVE;
		if (CheckHitKey(KEY_INPUT_LSHIFT))
		{
			mSpeed = SPEED_RUN;
			type = ANIM_TYPE::FAST_RUN;
		}
		//移動方向から移動量を算出する
		//移動量　= 方向　×　スピード
		mMoveDir = dir;
		mMovePow = VScale(dir, mSpeed);
		
		//カメラの方向を向く
		//mTransform.quaRot = cameraRot.Mult(Quaternion::AngleAxis(rotRad,AsoUtility::AXIS_Y));
		if (!mIsJump && IsEndLanding())
		{
			//アニメーション再生(それぞれで指定出来ないからint型にする)
			mAnimationController->Play(static_cast<int>(type));
		}

	}
	else
	{
		if (!mIsJump && IsEndLanding())
		{

			if (CheckHitKey(KEY_INPUT_E))
			{
				
				mAnimationController->Play(static_cast<int>(ANIM_TYPE::KICK),13.0f,20.0f,false);
				ProcessShot();
			}
			else
			{
				//アニメーション再生
				mAnimationController->Play(static_cast<int>(ANIM_TYPE::IDLE));
			}
			
		}

		

	}

	

	//回転(ゴールに対して、球面補完を行う)
	Rotate();
}

void Player::ProcessJump(void)
{

	bool isHit = CheckHitKey(KEY_INPUT_BACKSLASH);

	//ジャンプ受け付け
	if (isHit && (mIsJump || IsEndLanding()))
	{
		if (!mIsJump)
		{
			//mAnimationController->Play(static_cast<int>(ANIM_TYPE::JUMP));
			//mAnimationController->Play(static_cast<int>(ANIM_TYPE::JUMP), false);
			
			////切り取りアニメーション
			//mAnimationController->Play(
			// static_cast<int>(ANIM_TYPE::JUMP), false,
			//	13.0f, 24.0f);

			//無理やり浮遊中モーションを作る
			mAnimationController->Play(
				static_cast<int>(ANIM_TYPE::JUMP), true,
				13.0f, 24.0f);
			mAnimationController->SetEndLoop(23.0f,25.0f,5.0f);

		}

		mIsJump = true;

		//ジャンプの入力受付時間を減らす
		mStepJump += SceneManager::GetInstance().GetDeltaTime();
		if (mStepJump < TIME_JUMP_IN)
		{
			mJumpPow = VScale(mGravityManager.GetDirUpGravity(),POW_JUMP);
		}
	}

	

	//ボタンを離したらジャンプ力に加算しない
	if (!isHit)
	{
		mStepJump = TIME_JUMP_IN;
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

void Player::SetGoalRotate(float rotRad)
{
	//rad単位のアングルをとる
	VECTOR cameraAngles = 
		SceneManager::GetInstance().GetCamera()->GetAngles();

	Quaternion axis = 
		Quaternion::AngleAxis(
			cameraAngles.y + rotRad,
			AsoUtility::AXIS_Y);

	//現在設定されている回転との角度差を求める
	double angleDiff = Quaternion::Angle(axis,mGoalQuaRot);

	//しきい値
	if (angleDiff > 0.1f)
	{
		mStepRot = TIME_ROT;
	}

	//到達したい回転を設定
	mGoalQuaRot = axis;

}

void Player::Rotate(void)
{

	mStepRot -= SceneManager::GetInstance().GetDeltaTime();

	//mPlayerRotY = Quaternion::Slerp(
	//mPlayerRotY,mGoalQuaRot,(TIME_ROT - mStepRot) / TIME_ROT);

	mPlayerRotY = Quaternion::Slerp(
		mPlayerRotY, mGoalQuaRot, 0.2f);

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

void Player::StartWarp(
	float time, const Quaternion& goalRot, const VECTOR& goalPos)
{

	mTimeWarp = time;
	mStepWarp = time;
	//進行方向
	mWarpQua = goalRot;
	mWarpReservePos = goalPos;
	mPreWarpName = mGravityManager.GetActivePlanet()->GetName();

	//ワープの準備段階に入る
	ChangeState(STATE::WARP_RESERVE);
}

void Player::StopOrbitEffect(void)
{
	//エフェクト停止
	StopEffekseer3DEffect(mPlayOrbitEffectL);
	StopEffekseer3DEffect(mPlayOrbitEffectR);
}

bool Player::IsPlay(void)
{
	return mState == STATE::PLAY;
}

bool Player::IsWarpMove(void)
{
	return mState == STATE::WARP_MOVE;
}

bool Player::IsDead(void)
{
	return mState == STATE::DEAD;
}

std::vector<PlayerShot*>& Player::GetShots(void)
{
	return mShots;
}

SpeechBalloon* Player::GetSpeechBalloon(void) const
{
	return mSpeechBalloon;
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
	case Player::STATE::WARP_RESERVE:
		
		mJumpPow = AsoUtility::VECTOR_ZERO;

		//移動前と移動後の座標と回転の比率を渡す
		//ワープ準備開始時のプレイヤー情報
		mReserveStartQua = mTransform.quaRot;
		mReserveStartPos = mTransform.pos;

		//ポーズアニメーション
		mAnimationController->Play(
			static_cast<int>(Player::ANIM_TYPE::WARP_PAUSE));

		break;
	case Player::STATE::WARP_MOVE:

		//キャラクターが正面を向いている状態
		mPlayerRotY = Quaternion();

		//向きたい方向
		mGoalQuaRot = Quaternion();

		//飛ぶアニメーション
		mAnimationController->Play(
			static_cast<int>(Player::ANIM_TYPE::FLY));

		EffectWarpOrbit();

		break;
	case Player::STATE::DEAD:
		//飛ぶアニメーション
		mAnimationController->Play(
			static_cast<int>(Player::ANIM_TYPE::FLY));
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

void Player::CollisionCapsule(void)
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

			if (mIsJump)
			{
				//着地モーション
				mAnimationController->Play(
					static_cast<int>(ANIM_TYPE::JUMP), false,
					29.0f, 45.0f, false, true
				);
			}

			//ジャンプ終了
			mIsJump = false;
			mStepJump = 0.0f;
		}
	}
}

bool Player::IsEndLanding(void)
{
	//着地モーションが終わっている
	bool ret = true;

	if (mAnimationController->GetPlayType() 
		!= static_cast<int>(ANIM_TYPE::JUMP))
	{
		//ジャンプモーションじゃない(着地モーションが終わっている)

		return ret;
	}

	if (mAnimationController->IsEnd())
	{
		//ジャンプモーションが終了している
		return ret;
	}

	return false;
}

void Player::EffectFootSmoke(void)
{
	//円周：プレイヤーの足元から煙を0.3秒ごとに描画した、
	//		プレイヤーが動いている演出を行う。
	//		※プレイヤーが動いていないときは、煙を新たに発生させない
	// 
	//フレーム間の移動量：mMoveDiff
	//0.3秒ごと			：TIME_FOOT_SMOKE
	//エフェクトのHID	：mEffectSmoke

	//エフェクトを再生する関数
	//PlayEffekseer3DEffect

	//エフェクトの大きさを設定する関数
	//SetScalePlayingEffekseer3DEffect

	//エフェクトの位置を設定する関数
	//SetPosPlayingEffekseer3DEffect

	mMoveDiff.y = 0.0f;
	//移動量をfloat化
	float len = AsoUtility::SqrMagnitudeF(mMoveDiff);

	mStepFootSmoke -= SceneManager::GetInstance().GetDeltaTime();

	if (len != 0.0f)
	{
		if (mStepFootSmoke <= 0.0f)
		{
			mStepFootSmoke = TIME_FOOT_SMOKE;
			mPlayEffectSmoke = PlayEffekseer3DEffect(mEffectSmoke);

			float Scale = 10.0f;

			SetScalePlayingEffekseer3DEffect(mPlayEffectSmoke, Scale, Scale, Scale);

			SetPosPlayingEffekseer3DEffect(
				mPlayEffectSmoke,
				mTransform.pos.x, mTransform.pos.y, mTransform.pos.z
			);
		}
	}

	
}

void Player::EffectWarpOrbit(void)
{

	//①軌跡エフェクトの生成
	mPlayOrbitEffectL = PlayEffekseer3DEffect(mEffectWarpOrbit);
	mPlayOrbitEffectR = PlayEffekseer3DEffect(mEffectWarpOrbit);

	//②大きさの設定
	float scale = 10.0f;
	SetScalePlayingEffekseer3DEffect(mPlayOrbitEffectL, scale, scale, scale);
	SetScalePlayingEffekseer3DEffect(mPlayOrbitEffectR, scale, scale, scale);

	//③位置の設定(右手、左手)
	SyncWarpOrbitPos();

	//④エフェクトの回転
	VECTOR euler = mTransform.quaRot.ToEuler();
	SetRotationPlayingEffekseer3DEffect(
		mPlayOrbitEffectL, euler.x, euler.y, euler.z
	);

	SetRotationPlayingEffekseer3DEffect(
		mPlayOrbitEffectR, euler.x, euler.y, euler.z
	);
}

void Player::SyncWarpOrbitPos(void)
{
	//mFrameLeftHand
	//mFrameRightHand

	VECTOR pos;

	//左手の座標(対象モデルのフレーム番号の位置を取得)
	pos = MV1GetFramePosition(mTransform.modelId, mFrameLeftHand);

	SetPosPlayingEffekseer3DEffect(
		mPlayOrbitEffectL,
		pos.x, pos.y, pos.z
	);

	//左手の座標(対象モデルのフレーム番号の位置を取得)
	pos = MV1GetFramePosition(mTransform.modelId, mFrameRightHand);

	SetPosPlayingEffekseer3DEffect(
		mPlayOrbitEffectR,
		pos.x, pos.y, pos.z
	);
}

void Player::CreateShot(void)
{
	bool isCreate = false;

	VECTOR pos;

	//左手の座標(対象モデルのフレーム番号の位置を取得)
	pos = MV1GetFramePosition(mTransform.modelId, mFrameLeftHand);

	for (auto s : mShots)
	{
		if (!s->IsAlive())
		{
			//生存していなかったら
			//インスタンスを使いまわす
			s->Create(
				VGet(pos.x - 10.0f,pos.y,pos.z),
				mTransform.GetForward());
			isCreate = true;
		}
	}

	if (!isCreate)
	{
		//インスタンスを新しく作る
		auto newShot = new PlayerShot(&mTransform);
		newShot->Create(
			VGet(pos.x - 10.0f, pos.y, pos.z),
			mTransform.GetForward());

		//可変長配列に追加
		mShots.emplace_back(newShot);
	}
}
