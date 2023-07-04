#include <DxLib.h>
#include "../Application.h"
#include "../Manager/InputManager.h"
#include "../Manager/ResourceManager.h"
#include "../Manager/SceneManager.h"
#include "../Manager/Camera.h"
#include "../Object/RockManager.h"
#include "../Object/SpaceDome.h"
#include "../Object/Ship/BossShip.h"
#include "../Object/PlayerShip.h"
#include "../Object/Ship/Turret.h"
#include "../Object/Common/SpeechBalloon.h"
#include "../Object/Ship/PlayerShot.h"
#include "../Object/Ship/TurretShot.h"
#include "../Utility/AsoUtility.h"
#include "BattleScene.h"

BattleScene::BattleScene(void)
{
}

BattleScene::~BattleScene(void)
{
}

void BattleScene::Init(void)
{
	mBossShip = new BossShip();
	mBossShip->Init();

	mPlayerShip = new PlayerShip();
	mPlayerShip->Init();

	const auto& trans = mPlayerShip->GetTransform();

	//背景岩
	mRockManager = new RockManager(&trans);
	mRockManager->Init();

	//背景(スカイドーム)
	mSpaceDome = new SpaceDome(&trans);
	mSpaceDome->Init();

	//カメラモードを追従型にする
	auto& ins = SceneManager::GetInstance();
	ins.GetCamera()->SetFollowTarget(&trans);
	ins.GetCamera()->ChangeMode(Camera::MODE::FOLLOW_SPRING);

	//自機の破壊演出時間の初期化
	mStepShipDestroy = 0.0f;

	//ゲームクリア後のロゴ
	mImgEndLogo = ResourceManager::GetInstance().Load(
		ResourceManager::SRC::END_LOGO).mHandleId;

	//ボスの破壊演出時間の初期化
	mStepBossDestroy = TIME_RESTART_BOSS_DESTROY;


}

void BattleScene::Update(void)
{
	mBossShip->Update();
	mPlayerShip->Update();
	mRockManager->Update();
	mSpaceDome->Update();
	
	//自機とボスとの当たり判定
	if (mPlayerShip->IsDestroy())
	{
		//自機が破壊されている
		mStepShipDestroy += SceneManager::GetInstance().GetDeltaTime();
		if (mStepShipDestroy > TIME_RESTART)
		{
			//シーン遷移
			SceneManager::GetInstance().ChangeScene(
				SceneManager::SCENE_ID::BATTLE, true);
		}
	}
	else
	{
		//ボスの生存判定
		if (mBossShip->IsAlive())
		{
			//自機とダンジョンの衝突判定
			auto info = MV1CollCheck_Sphere(
				mBossShip->GetTransform().modelId, -1,
				mPlayerShip->GetTransform().pos, PlayerShip::COLLISION_RADIUS);

			//if文は=がない方が早い
			if (info.HitNum > 0)
			{
				//衝突しているので自機を破壊
				mPlayerShip->Destroy();
			}

			//当たり判定用の結果情報の中で動的にメモリ確保しているようなので、
			//使い終わったら、必ずメモリの破棄・後始末をすること
			MV1CollResultPolyDimTerminate(info);

			//ボスの弾
			auto turrets = mBossShip->GetTurrets();
			for (const auto t : turrets)
				{
				auto ts = t->GetShots();
				for (auto s : ts)
				{
					if (!s->IsAlive())
					{
						continue;
					}

					if (AsoUtility::IsHitSpheres(
						mPlayerShip->GetTransform().pos, PlayerShip::COLLISION_RADIUS,
						s->GetPos(), s->GetCollisionRadius()))
					{
						//衝突しているので自機を破壊
						mPlayerShip->Destroy();
					}
				}
				
			}
		}

	}
	auto& shots = mPlayerShip->GetShots();
	for (const auto s : shots)
	{

		s->Update();

		if (!s->IsAlive())
		{
			//生存していない
			continue;
		}

		if (mBossShip->IsAlive())
		{
			//弾とボスの衝突判定
			auto info = MV1CollCheck_Sphere(
				mBossShip->GetTransform().modelId, -1,
				s->GetPos() , s->GetCollisionRadius());

			if (info.HitNum > 0)
			{
				//衝突していたら、弾を破壊する
				s->CreateExplosion(&mBossShip->GetTransform());
				continue;
			}

			//当たり判定用の結果情報の中で動的にメモリ確保しているようなので、
			//使い終わったら、必ずメモリの破棄・後始末をすること
			MV1CollResultPolyDimTerminate(info);
		}

		//自機の弾とタレットの衝突判定(球体と球体)
		auto turrets = mBossShip->GetTurrets();
		for (auto t : turrets)
		{
			if (!t->IsAlive())
			{
				continue;
			}

			//自機の弾とタレットの衝突判定
			/*auto info = MV1CollCheck_Sphere(
				t->GetPos(), t->COLLISION_RADIUS,
				s->GetPos(), s->GetCollisionRadius());*/

			if (AsoUtility::IsHitSpheres(
				s->GetPos(), s->GetCollisionRadius(),
				t->GetPos(), Turret::COLLISION_RADIUS))
			{
				//タレットにダメージ
				t->Damage();

				//弾を貫通させない
				s->CreateExplosion(nullptr);
				break;
			}


		}

	}

	//ボスの破壊判定
	if (mBossShip->IsEnd())
	{
		mStepBossDestroy -= SceneManager::GetInstance().GetDeltaTime();
		if (mStepBossDestroy < 0.0)
		{
			SceneManager::GetInstance().ChangeScene(
				SceneManager::SCENE_ID::TITLE, true
			);
		}
	}
	if (CheckHitKey(KEY_INPUT_L))
	{
		SceneManager::GetInstance().GetCamera()->ChangeMode(Camera::MODE::FOLLOW_SPRING);
	}
}

void BattleScene::Draw(void)
{
	//不透明(背景)
	mRockManager->Draw();
	mSpaceDome->Draw();

	//不透明(オブジェクト)
	mBossShip->Draw();
	mPlayerShip->Draw();

	auto& shots = mPlayerShip->GetShots();
	for (const auto s : shots)
	{
		s->Draw();
	}

	mPlayerShip->GetSpeechBalloon()->Draw();	

	if (mBossShip->IsDestroy() || mBossShip->IsEnd())
	{
		//クリアロゴ
		DrawRotaGraph(
			Application::SCREEN_SIZE_X / 2,
			Application::SCREEN_SIZE_Y / 2,
			1.0f, 0.0, mImgEndLogo, true);
	}
}

void BattleScene::Release(void)
{
	mBossShip->Release();
	delete mBossShip;
	mPlayerShip->Release();
	delete mPlayerShip;
	mRockManager->Release();
	delete mRockManager;
	mSpaceDome->Release();
	delete mSpaceDome;
}
