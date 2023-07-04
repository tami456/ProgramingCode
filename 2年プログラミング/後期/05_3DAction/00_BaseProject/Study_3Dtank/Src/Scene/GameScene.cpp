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
#include "../Object/Planet.h"
#include "GameScene.h"

GameScene::GameScene(void)
{
	mPlayer = nullptr;
	mSkyDome = nullptr;
	mStage = nullptr;
}

void GameScene::Init(void)
{

	mPlayer = new Player();
	GravityManager::GetInstance().SetPlayer(mPlayer);
	mPlayer->Init();

	// スカイドーム
	mSkyDome = new SkyDome(mPlayer->GetTransform());
	mSkyDome->Init();

	mStage = new Stage(mPlayer);
	mStage->Init();

	// ステージの設定
	mStage->ChangeStage(Stage::NAME::MAIN_PLANET);

	SceneManager::GetInstance().GetCamera()->SetTransform(mPlayer->GetTransform());
	SceneManager::GetInstance().GetCamera()->ChangeMode(Camera::MODE::FIXED_POINT);


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
	}
}

void GameScene::Draw(void)
{

	mSkyDome->Draw();

	mStage->Draw();

	mPlayer->Draw();

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
}
