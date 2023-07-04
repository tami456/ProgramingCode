#include <DxLib.h>
#include "../Utility/AsoUtility.h"
#include "../Manager/SceneManager.h"
#include "../Manager/Camera.h"
#include "../Manager/InputManager.h"
#include "../Manager/GravityManager.h"
#include "../Object/Common/Capsule.h"
#include "../Object/Common/Collider.h"
#include "../Object/Common/SpeechBalloon.h"
#include "../Object/SkyDome.h"
#include "../Object/Stage.h"
#include "../Object/Player.h"
#include "../Object/PlayerShot.h"
#include "../Object/Enemy.h"
#include "../Object/Mascot.h"
#include "../Object/Planet.h"
#include "GameScene.h"

GameScene::GameScene(void)
{
	mPlayer = nullptr;
	mEnemy = nullptr;
	mMascot = nullptr;
	mSkyDome = nullptr;
	mStage = nullptr;
}

void GameScene::Init(void)
{

	mPlayer = new Player();
	GravityManager::GetInstance().SetPlayer(mPlayer);
	mPlayer->Init();
	
	mEnemy = new Enemy(mPlayer);
	GravityManager::GetInstance().SetEnemy(mEnemy);
	mEnemy->Init();

	mMascot = new Mascot();
	mMascot->Init();

	std::string msg = "";

	auto name = GravityManager::GetInstance().GetActivePlanet()->GetName();

	if (mEnemy->IsDead())
	{
		msg = "星に向かおう！";
	}
	else if (!mEnemy->IsDead())
	{
		msg = "敵を倒そう！";
	}

	mPlayer->GetSpeechBalloon()->SetText(msg.c_str());

	// スカイドーム
	mSkyDome = new SkyDome(mPlayer->GetTransform());
	mSkyDome->Init();

	mStage = new Stage(mPlayer, mEnemy);
	mStage->Init();

	// ステージの設定
	mStage->ChangeStage(Stage::NAME::MAIN_PLANET);

	SceneManager::GetInstance().GetCamera()->SetTransform(mPlayer->GetTransform());
	SceneManager::GetInstance().GetCamera()->ChangeMode(Camera::MODE::FOLLOW);


}

void GameScene::Update(void)
{

	// シーン遷移
	InputManager& ins = InputManager::GetInstance();
	if (mPlayer->IsDead())
	{
		SceneManager::GetInstance().ChangeScene(SceneManager::SCENE_ID::GAMEOVER);
	}

	mSkyDome->Update();

	mStage->Update();

	mPlayer->Update();

	mEnemy->Update();

	mMascot->Update();

	

	//弾を更新
	auto& shots = mPlayer->GetShots();
	for (const auto s : shots)
	{

		s->Update();

		if (!s->IsAlive())
		{
			//生存していない
			continue;
		}

		if (mEnemy->IsAlive())
		{
			//弾とボスの衝突判定
			
			auto pos = mEnemy->GetTransform().pos;
			if (AsoUtility::IsHitSpheres(
				s->GetTransform().pos, s->GetCollisionRadius(),
				VGet(pos.x,pos.y + 80,pos.z), 100.0f))
			{
				//衝突していたら、弾を破壊する
				s->NonAlive();
				mEnemy->Damage();
				break;
			}
		}
	}
}

void GameScene::Draw(void)
{

	mSkyDome->Draw();

	mStage->Draw();

	mPlayer->Draw();

	mEnemy->Draw();

	mMascot->Draw();

	mPlayer->GetSpeechBalloon()->Draw();

	//弾を更新
	auto& shots = mPlayer->GetShots();
	for (const auto s : shots)
	{
		s->Draw();
	}

	// デバッグ表示
	DrawDebug();

	// ヘルプ
	DrawFormatString(840, 20, 0x000000, "移動　　：WASD");
	DrawFormatString(840, 40, 0x000000, "カメラ　：矢印キー");
	DrawFormatString(840, 60, 0x000000, "ダッシュ：左Shift");
	DrawFormatString(840, 80, 0x000000, "ジャンプ：＼(バクスラ)");
	DrawFormatString(840, 100, 0x000000, "攻撃    ：E");

}

void GameScene::DrawDebug(void)
{
}

void GameScene::Release(void)
{

	mSkyDome->Release();
	delete mSkyDome;

	mStage->Release();
	delete mStage;

	mPlayer->Release();
	delete mPlayer;
	
	mEnemy->Release();
	delete mEnemy;

	mMascot->Release();
	delete mMascot;
}
