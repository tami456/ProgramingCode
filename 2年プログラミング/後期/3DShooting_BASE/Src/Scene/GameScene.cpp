#include <cmath>
#include <DxLib.h>
#include "../Application.h"
#include "../Manager/ResourceManager.h"
#include "../Manager/InputManager.h"
#include "../Manager/SceneManager.h"
#include "../Manager/Camera.h"
#include "../Utility/AsoUtility.h"
#include "../Object/Stage.h"
#include "../Object/SpaceDome.h"
#include "../Object/PlayerShip.h"
#include "../Object/RockManager.h"
#include "../Object/Common/SpeechBalloon.h"
#include "../Object/Ship/PlayerShot.h"
#include "../Object/Ship/TurretShot.h"
#include "../Object/Ship/Turret.h"
#include "GameScene.h"

GameScene::GameScene(void)
{

}

GameScene::~GameScene(void)
{
}

void GameScene::Init(void)
{
	

	mPlayerShip = new PlayerShip();
	mPlayerShip->Init();

	mSpaceDome = new SpaceDome(&mPlayerShip->GetTransform());
	mSpaceDome->Init();
	
	


	mStage = new Stage();
	mStage->Init();

	//カメラモード(フリーカメラ)
	//SceneManager::GetInstance().GetCamera()->ChangeMode(Camera::MODE::FREE);

	//カメラモード(追従カメラ)
	SceneManager::GetInstance().GetCamera()->SetFollowTarget(&mPlayerShip->GetTransform());
	//SceneManager::GetInstance().GetCamera()->ChangeMode(Camera::MODE::FOLLOW);
	SceneManager::GetInstance().GetCamera()->ChangeMode(Camera::MODE::FOLLOW_SPRING);

	mRockManager = new RockManager(&mPlayerShip->GetTransform());
	mRockManager->Init();

	mStepShipdestroy = 0.0f;

}

void GameScene::Update(void)
{
	// シーン遷移
	auto& ins = InputManager::GetInstance();
	if (ins.IsTrgDown(KEY_INPUT_SPACE))
	{
		SceneManager::GetInstance().ChangeScene(SceneManager::SCENE_ID::EVENT, true);
	}
	
	mStage->Update();
	mSpaceDome->Update();
	mPlayerShip->Update();
	mRockManager->Update();

	if (mPlayerShip->IsDestroy())
	{
		//自機が破壊されている
		mStepShipdestroy += SceneManager::GetInstance().GetDeltaTime();
		if (mStepShipdestroy > TIME_RESTART)
		{
			//シーン遷移
			SceneManager::GetInstance().ChangeScene(SceneManager::SCENE_ID::TITLE, true);
		}
	}
	else
	{
		//自機とダンジョンの衝突判定
		auto info = MV1CollCheck_Sphere(
			mStage->GetModelDungeon(), -1,
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
	}

	//演習：ボス戦艦の周囲5000の距離に入ったら、イベントシーンに遷移するように
	//PGを作成してください
	
	if (AsoUtility::SqrMagnitude(mStage->GetPosCargoShip(), mPlayerShip->GetTransform().pos) < 5000 * 5000)
	{
		SceneManager::GetInstance().ChangeScene(SceneManager::SCENE_ID::EVENT, true);
	}

	//弾を更新
	auto& shots = mPlayerShip->GetShots();
	for (const auto s : shots)
	{
		s->Update();
	}

	

}

void GameScene::Draw(void)
{
	//背景・不透明なもの
	mSpaceDome->Draw();
	
	//オブジェクト(透明・半透明等)
	mStage->Draw();
	mRockManager->Draw();
	mPlayerShip->Draw();

	//弾を更新
	auto& shots = mPlayerShip->GetShots();
	for (const auto s : shots)
	{
		s->Draw();
	}

	//スクリーンエフェクト
	
	//UI(2D)関連
	mPlayerShip->GetSpeechBalloon()->Draw();

	//スクリーンエフェクト(UI混み)
}

void GameScene::Release(void)
{
	mStage->Release();
	delete mStage;

	mSpaceDome->Release();
	delete mSpaceDome;
	
	mPlayerShip->Release();
	delete mPlayerShip;
	
	mRockManager->Release();
	delete mRockManager;

}
